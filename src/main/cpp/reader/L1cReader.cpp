/*
 * File:   L1cReader.cpp
 * Author: thomass
 *
 * Created on November 18, 2010, 2:08 PM
 */

#include <cstdlib>

#include "../util/IOUtils.h"
#include "../util/NetCDF.h"
#include "../core/JobOrder.h"

#include "L1cReader.h"

using std::getenv;
using std::min;
using std::max;

L1cReader::L1cReader() :
		BasicModule("L1C_READER") {
}

L1cReader::~L1cReader() {
	pair<string, int> fileIdPair;
	foreach(fileIdPair, ncFileIdMap)
			{
				try {
					NetCDF::closeFile(fileIdPair.second);
				} catch (exception& ignored) {
					// ok
				}
			}
}

void L1cReader::start(Context& context) {
	segmentLineCount = 400;
	const string segmentLineCountString =
			context.getJobOrder()->getIpfConfiguration().getDynamicProcessingParameter(
					"Segment_Line_Count");
	if (!segmentLineCountString.empty()) {
		segmentLineCount = lexical_cast<size_t>(segmentLineCountString);
	}
	context.getLogging()->info(
			"segment line count is " + lexical_cast<string>(segmentLineCount),
			getId());

	const Dictionary& dict = *context.getDictionary();
	const vector<SegmentDescriptor*> segmentDescriptors =
			dict.getProductDescriptor(Constants::PRODUCT_L1C).getSegmentDescriptors();

	sourceDirPath =
			path(
					context.getJobOrder()->getIpfProcessors().at(0).getInputList().at(
							0).getFileNames().at(0));
	if (!sourceDirPath.has_root_directory()) {
		sourceDirPath = getInstallationPath() / sourceDirPath;
	}
	context.getLogging()->info(
			"source product path is '" + sourceDirPath.string() + "'", getId());

	foreach(SegmentDescriptor* segmentDescriptor, segmentDescriptors)
			{
				const vector<VariableDescriptor*> variableDescriptors =
						segmentDescriptor->getVariableDescriptors();
				const string& segmentName = segmentDescriptor->getName();

				foreach(VariableDescriptor* variableDescriptor, variableDescriptors)
						{
							const string varName =
									variableDescriptor->getName();
							const string ncVarName =
									variableDescriptor->getNcVarName();
							const string ncFileBasename =
									variableDescriptor->getNcFileBasename();
							const int fileId = getNcFile(ncFileBasename);
							const int varId = NetCDF::getVariableId(fileId,
									ncVarName.c_str());
							const int dimCount = NetCDF::getDimensionCount(
									fileId, varId);
							const valarray<int> dimIds =
									NetCDF::getDimensionIds(fileId, varId);

							size_t camCount;
							size_t rowCount;
							size_t colCount;

							switch (dimCount) {
							case 3:
								camCount = NetCDF::getDimensionLength(fileId,
										dimIds[0]);
								rowCount = NetCDF::getDimensionLength(fileId,
										dimIds[1]);
								colCount = NetCDF::getDimensionLength(fileId,
										dimIds[2]);
								variableDescriptor->addDimension(
										NetCDF::getDimensionName(fileId,
												dimIds[0])).setSize(camCount);
								variableDescriptor->addDimension(
										NetCDF::getDimensionName(fileId,
												dimIds[1])).setSize(rowCount);
								variableDescriptor->addDimension(
										NetCDF::getDimensionName(fileId,
												dimIds[2])).setSize(colCount);
								break;
							case 2:
								camCount = 1;
								rowCount = NetCDF::getDimensionLength(fileId,
										dimIds[0]);
								colCount = NetCDF::getDimensionLength(fileId,
										dimIds[1]);
								variableDescriptor->addDimension(
										NetCDF::getDimensionName(fileId,
												dimIds[0])).setSize(rowCount);
								variableDescriptor->addDimension(
										NetCDF::getDimensionName(fileId,
												dimIds[1])).setSize(colCount);
								break;
							case 1:
								camCount = 1;
								rowCount = 1;
								colCount = NetCDF::getDimensionLength(fileId,
										dimIds[0]);
								variableDescriptor->addDimension(
										NetCDF::getDimensionName(fileId,
												dimIds[0])).setSize(colCount);
								break;
							default:
								BOOST_THROW_EXCEPTION(
										runtime_error("Invalid number of dimensions for variable '" + ncVarName + "'."));
								break;
							}
							if (!context.hasSegment(segmentName)) {
								const size_t sizeL = min(segmentLineCount,
										rowCount);
								context.getLogging()->info(
										"adding segment '" + segmentName
												+ "' to context", getId());
								context.addSegment(segmentName, sizeL, colCount,
										camCount, 0, rowCount - 1);
							}
							const int type = NetCDF::getVariableType(fileId,
									varId);
							variableDescriptor->setType(type);

							context.getLogging()->info(
									"adding variable '" + varName
											+ "' to segment '" + segmentName
											+ "'", getId());
							context.getSegment(segmentName).addVariable(varName,
									type);
							ncVarIdMap[varName] = varId;
						}
			}
}

void L1cReader::stop(Context& context) {
	pair<string, int> fileIdPair;

	foreach(fileIdPair, ncFileIdMap)
			{
				context.getLogging()->info(
						"closing netCDF file '" + fileIdPair.first + "'",
						getId());
				NetCDF::closeFile(fileIdPair.second);
			}
	ncVarIdMap.clear();
	ncFileIdMap.clear();
}

void L1cReader::process(Context& context) {
	const Dictionary& dict = *context.getDictionary();
	const vector<SegmentDescriptor*> segmentDescriptors =
			dict.getProductDescriptor(Constants::PRODUCT_L1C).getSegmentDescriptors();

	foreach(SegmentDescriptor* segmentDescriptor, segmentDescriptors)
			{
				const Segment& segment = context.getSegment(
						segmentDescriptor->getName());
				const Grid& grid = segment.getGrid();
				if (!context.hasLastLComputed(segment, *this)
						|| context.getLastComputedL(segment, *this)
								< grid.getFirstL() + grid.getSizeL() - 1) {
					const vector<VariableDescriptor*> variableDescriptors =
							segmentDescriptor->getVariableDescriptors();
					const size_t firstLComputable = context.getFirstComputableL(
							segment, *this);
					const size_t lastLComputable = context.getLastComputableL(
							segment);

					foreach(VariableDescriptor* variableDescriptor, variableDescriptors)
							{
								const string varName =
										variableDescriptor->getName();
								const string ncFileName =
										variableDescriptor->getNcFileBasename();

								if (!contains(ncVarIdMap, varName)) {
									BOOST_THROW_EXCEPTION(
											logic_error("Unknown variable '" + varName + "'."));
								}
								if (!contains(ncFileIdMap, ncFileName)) {
									BOOST_THROW_EXCEPTION(
											logic_error("Unknown netCDF file '" + ncFileName + "'."));
								}
								const int varId = ncVarIdMap[varName];
								const int fileId = ncFileIdMap[ncFileName];
								const size_t dimCount =
										variableDescriptor->getDimensions().size();
								const valarray<size_t> starts =
										IOUtils::createStartVector(dimCount,
												firstLComputable);
								const valarray<size_t> counts =
										IOUtils::createCountVector(
												dimCount,
												grid.getSizeK(),
												lastLComputable
														- firstLComputable + 1,
												grid.getSizeM());
								context.getLogging()->progress(
										"Reading variable '"
												+ variableDescriptor->getNcVarName()
												+ "' into segment '"
												+ segment.toString() + "'",
										getId());
								const Accessor& accessor = segment.getAccessor(
										varName);
								NetCDF::getData(fileId, varId, starts, counts,
										accessor.getUntypedData());
							}
					context.setLastComputedL(segment, *this, lastLComputable);
				}
			}
}

int L1cReader::getNcFile(const string& ncFileBasename) {
	if (contains(ncFileIdMap, ncFileBasename)) {
		return ncFileIdMap[ncFileBasename];
	}
	const path ncFilePath = sourceDirPath / (ncFileBasename + ".nc");
	const int fileId = NetCDF::openFile(ncFilePath.string());
	ncFileIdMap[ncFileBasename] = fileId;

	return fileId;
}
