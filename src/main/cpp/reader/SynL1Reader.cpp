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

#include "SynL1Reader.h"

using std::getenv;
using std::min;
using std::max;

SynL1Reader::SynL1Reader() :
        BasicModule("SY1_READER") {
}

SynL1Reader::~SynL1Reader() {
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

long SynL1Reader::getSegmentSize(const string& segmentName, const long rowCount) const {
	if (segmentName.compare(Constants::SEGMENT_GEO) == 0) {
		return rowCount;
	}
	if (rowCount < segmentLineCount) {
		return rowCount;
	}
	return segmentLineCount;
}

void SynL1Reader::start(Context& context) {
	segmentLineCount = 400;
	const string segmentLineCountString = context.getJobOrder().getIpfConfiguration().getDynamicProcessingParameter("Segment_Line_Count");
	if (!segmentLineCountString.empty()) {
		segmentLineCount = lexical_cast<long>(segmentLineCountString);
	}
	context.getLogging().info("Segment line count is " + lexical_cast<string>(segmentLineCount), getId());

	const Dictionary& dict = context.getDictionary();
	const vector<SegmentDescriptor*> segmentDescriptors = dict.getProductDescriptor(Constants::PRODUCT_SY1).getSegmentDescriptors();

	sourceDirPath = path(context.getJobOrder().getIpfProcessors().at(0).getInputList().at(0).getFileNames().at(0));
	if (!sourceDirPath.has_root_directory()) {
		sourceDirPath = Constants::S3_SYNERGY_HOME / sourceDirPath;
	}
	context.getLogging().info("Source product path is '" + sourceDirPath.string() + "'", getId());

	foreach(SegmentDescriptor* segmentDescriptor, segmentDescriptors)
			{
				const vector<VariableDescriptor*> variableDescriptors = segmentDescriptor->getVariableDescriptors();
				const string& segmentName = segmentDescriptor->getName();

				foreach(VariableDescriptor* variableDescriptor, variableDescriptors)
						{
							const string varName = variableDescriptor->getName();
							const string ncVarName = variableDescriptor->getNcVarName();
							const string ncFileBasename = variableDescriptor->getNcFileBasename();
							const int fileId = getNcFile(ncFileBasename);
							const int varId = NetCDF::getVariableId(fileId, ncVarName.c_str());
							const int dimCount = NetCDF::getDimensionCount(fileId, varId);
							const valarray<int> dimIds = NetCDF::getDimensionIds(fileId, varId);

							long camCount;
							long rowCount;
							long colCount;

							// Set grid parameters and write them to dictionary
							switch (dimCount) {
							case 3:
								camCount = NetCDF::getDimensionLength(fileId, dimIds[0]);
								rowCount = NetCDF::getDimensionLength(fileId, dimIds[1]);
								colCount = NetCDF::getDimensionLength(fileId, dimIds[2]);
								variableDescriptor->addDimension(NetCDF::getDimensionName(fileId, dimIds[0])).setSize(camCount);
								variableDescriptor->addDimension(NetCDF::getDimensionName(fileId, dimIds[1])).setSize(rowCount);
								variableDescriptor->addDimension(NetCDF::getDimensionName(fileId, dimIds[2])).setSize(colCount);
								break;
							case 2:
								camCount = 1;
								rowCount = NetCDF::getDimensionLength(fileId, dimIds[0]);
								colCount = NetCDF::getDimensionLength(fileId, dimIds[1]);
								variableDescriptor->addDimension(NetCDF::getDimensionName(fileId, dimIds[0])).setSize(rowCount);
								variableDescriptor->addDimension(NetCDF::getDimensionName(fileId, dimIds[1])).setSize(colCount);
								break;
							case 1:
								camCount = 1;
								rowCount = 1;
								colCount = NetCDF::getDimensionLength(fileId, dimIds[0]);
								variableDescriptor->addDimension(NetCDF::getDimensionName(fileId, dimIds[0])).setSize(colCount);
								break;
							default:
								BOOST_THROW_EXCEPTION( runtime_error("Invalid number of dimensions for variable '" + ncVarName + "'."));
								break;
							}
							// Create a new segment, if necessary
							if (!context.hasSegment(segmentName)) {
								const long sizeL = getSegmentSize(segmentName, rowCount);
								context.getLogging().info("Adding segment '" + segmentName + "' to context", getId());
								context.addSwathSegment(segmentName, sizeL, colCount, camCount, 0, rowCount - 1);
							}

							// Copy variable attributes to dictionary
							const int type = NetCDF::getVariableType(fileId, varId);
							variableDescriptor->setType(type);
							const size_t attrCount = NetCDF::getAttributeCount(fileId, varId);
							for (size_t i = 0; i < attrCount; i++) {
								const string attrName = NetCDF::getAttributeName(fileId, varId, i);
								const Attribute attr = NetCDF::getAttribute(fileId, varId, attrName);
								context.getLogging().info("Adding attribute '" + attr.toString() + "' to variable '" + varName + "'", getId());
								variableDescriptor->addAttribute(attr);
							}
							// Add variable to segment
							Segment& segment = context.getSegment(segmentName);
							context.getLogging().info("Adding accessor for " + variableDescriptor->toString() + " to segment " + segment.toString(), getId());
							segment.addVariable(*variableDescriptor);
							ncVarIdMap[varName] = varId;
						}
			}
}

void SynL1Reader::stop(Context& context) {
	pair<string, int> fileIdPair;

	foreach(fileIdPair, ncFileIdMap)
			{
				context.getLogging().info("Closing netCDF file '" + fileIdPair.first + ".nc'", getId());
				NetCDF::closeFile(fileIdPair.second);
			}
	ncVarIdMap.clear();
	ncFileIdMap.clear();
}

void SynL1Reader::process(Context& context) {
	const Dictionary& dict = context.getDictionary();
	const vector<SegmentDescriptor*> segmentDescriptors = dict.getProductDescriptor(Constants::PRODUCT_SY1).getSegmentDescriptors();

	valarray<size_t> starts;
	valarray<size_t> counts;

	foreach(SegmentDescriptor* segmentDescriptor, segmentDescriptors)
			{
				const Segment& segment = context.getSegment(segmentDescriptor->getName());
				const Grid& grid = segment.getGrid();
				if (!context.hasLastComputedL(segment, *this) || context.getLastComputedL(segment, *this) < grid.getFirstL() + grid.getSizeL() - 1) {
					const vector<VariableDescriptor*> variableDescriptors = segmentDescriptor->getVariableDescriptors();
					const long firstL = segment.getGrid().getFirstL();
					context.getLogging().debug("Segment [" + segment.toString() + "]: firstL = " + lexical_cast<string>(firstL), getId());
					const long lastL = segment.getGrid().getLastL();
					context.getLogging().debug("Segment [" + segment.toString() + "]: lastL = " + lexical_cast<string>(lastL), getId());

					foreach(VariableDescriptor* variableDescriptor, variableDescriptors)
							{
								const string varName = variableDescriptor->getName();
								const string ncFileName = variableDescriptor->getNcFileBasename();

								if (!contains(ncVarIdMap, varName)) {
									BOOST_THROW_EXCEPTION(logic_error("Unknown variable '" + varName + "'."));
								}
								if (!contains(ncFileIdMap, ncFileName)) {
									BOOST_THROW_EXCEPTION(logic_error("Unknown netCDF file '" + ncFileName + "'."));
								}
								const int varId = ncVarIdMap[varName];
								const int fileId = ncFileIdMap[ncFileName];
								const size_t dimCount = variableDescriptor->getDimensions().size();
								IOUtils::createStartVector(dimCount, firstL, starts);
								IOUtils::createCountVector(dimCount, grid.getSizeK(), lastL - firstL + 1, grid.getSizeM(), counts);
								context.getLogging().progress("Reading variable '" + varName + "' into segment " + segment.toString(), getId());
								const Accessor& accessor = segment.getAccessor(varName);
								NetCDF::getVariableData(fileId, varId, starts, counts, accessor.getUntypedData());
							}
					context.setLastComputedL(segment, *this, lastL);
				}
			}
}

int SynL1Reader::getNcFile(const string& ncFileBasename) {
	if (contains(ncFileIdMap, ncFileBasename)) {
		return ncFileIdMap[ncFileBasename];
	}
	const path ncFilePath = sourceDirPath / (ncFileBasename + ".nc");
	const int fileId = NetCDF::openFile(ncFilePath.string());
	ncFileIdMap[ncFileBasename] = fileId;

	return fileId;
}
