/*
 * File:   SynL2Writer.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 3:40 PM
 */

#include <stdexcept>

#include "../util/IOUtils.h"
#include "../util/NetCDF.h"

#include "SynL2Writer.h"

using std::runtime_error;

SynL2Writer::SynL2Writer() :
		Writer("SY2_WRITER") {
}

SynL2Writer::~SynL2Writer() {
	pair<string, int> fileIdPair;
	foreach(fileIdPair, ncFileIdMap)
			{
				try {
					NetCDF::closeFile(fileIdPair.second);
				} catch (exception& ignored) {
				}
			}
}

void SynL2Writer::process(Context& context) {
	const Dictionary& dict = *context.getDictionary();
	const vector<SegmentDescriptor*> segmentDescriptors = dict.getProductDescriptor(Constants::PRODUCT_SY2).getSegmentDescriptors();

	foreach(const SegmentDescriptor* segmentDescriptor, segmentDescriptors)
			{
				const string segmentName = segmentDescriptor->getName();
				if (context.hasSegment(segmentName)) {
					const Segment& segment = context.getSegment(segmentName);
					const Grid& grid = segment.getGrid();
					const vector<VariableDescriptor*> variableDescriptors = segmentDescriptor->getVariableDescriptors();
					const long firstWritableL = context.getFirstComputableL(segment, *this);
					const long lastWritableL = context.getLastComputableL(segment, *this);

					if (firstWritableL <= lastWritableL) {
						foreach(const VariableDescriptor* variableDescriptor, variableDescriptors)
								{
									const string varName = variableDescriptor->getName();
									if (segment.hasVariable(varName)) {
										const string ncFileBasename = variableDescriptor->getNcFileBasename();
										if (!contains(ncVarIdMap, varName)) {
											BOOST_THROW_EXCEPTION( logic_error("Unknown variable '" + varName + "'."));
										}
										if (!contains(ncFileIdMap, ncFileBasename)) {
											BOOST_THROW_EXCEPTION( logic_error("Unknown netCDF file '" + ncFileBasename + "'."));
										}
										if (!contains(ncDimIdMap, ncFileBasename)) {
											BOOST_THROW_EXCEPTION( logic_error("Unknown netCDF file '" + ncFileBasename + "'."));
										}
										const int varId = ncVarIdMap[varName];
										const int ncId = ncFileIdMap[ncFileBasename];
										const valarray<int>& dimIds = ncDimIdMap[ncFileBasename];
										const valarray<size_t> starts = IOUtils::createStartVector(dimIds.size(), firstWritableL);
										const valarray<size_t> sizes = IOUtils::createCountVector(dimIds.size(), grid.getSizeK(), lastWritableL - firstWritableL + 1, grid.getSizeM());
										context.getLogging()->progress("Writing variable " + varName + " of segment [" + segment.toString() + "]", getId());
										const Accessor& accessor = segment.getAccessor(varName);
										NetCDF::putData(ncId, varId, starts, sizes, accessor.getUntypedData());
									}
								}
						context.setLastComputedL(segment, *this, lastWritableL);
					}
				}
			}
}

void SynL2Writer::start(Context& context) {
	targetDirPath = path(context.getJobOrder()->getIpfProcessors().at(0).getOutputList().at(0).getFileName());
	if (!targetDirPath.has_root_directory()) {
		targetDirPath = getInstallationPath() / targetDirPath;
	}
	context.getLogging()->info("target product path is '" + targetDirPath.string() + "'", getId());

	const Dictionary& dict = *context.getDictionary();
	const ProductDescriptor& productDescriptor = dict.getProductDescriptor(Constants::PRODUCT_SY2);
	const vector<SegmentDescriptor*> segmentDescriptors = productDescriptor.getSegmentDescriptors();

	foreach(SegmentDescriptor* segmentDescriptor, segmentDescriptors)
			{
				const string segmentName = segmentDescriptor->getName();
				if (context.hasSegment(segmentName)) {
					const Segment& segment = context.getSegment(segmentName);
					const vector<VariableDescriptor*> variableDescriptors = segmentDescriptor->getVariableDescriptors();

					foreach(VariableDescriptor* variableDescriptor, variableDescriptors)
							{
								if (segment.hasVariable(variableDescriptor->getName())) {
									createNcVar(productDescriptor, *segmentDescriptor, *variableDescriptor, segment.getGrid());
								}
							}
				}
			}

	pair<string, int> fileIdPair;

	foreach(fileIdPair, ncFileIdMap)
			{
				NetCDF::terminateFile(fileIdPair.second);
			}
}

void SynL2Writer::stop(Context& context) {
	pair<string, int> fileIdPair;

	foreach(fileIdPair, ncFileIdMap)
			{
				NetCDF::closeFile(fileIdPair.second);
			}
	ncVarIdMap.clear();
	ncDimIdMap.clear();
	ncFileIdMap.clear();
}

void SynL2Writer::createNcVar(const ProductDescriptor& productDescriptor, const SegmentDescriptor& segmentDescriptor, const VariableDescriptor& variableDescriptor, const Grid& grid) {
	const string varName = variableDescriptor.getName();
	const string ncFileBasename = variableDescriptor.getNcFileBasename();

	if (!contains(ncFileIdMap, ncFileBasename)) {
		if (!boost::filesystem::exists(targetDirPath)) {
			if (!boost::filesystem::create_directories(targetDirPath)) {
				BOOST_THROW_EXCEPTION( runtime_error("Cannot create directory '" + targetDirPath.string() + "'."));
			}
		}
		const path ncFilePath = targetDirPath / (variableDescriptor.getNcFileBasename() + ".nc");
		const int fileId = NetCDF::createFile(ncFilePath.string());

		foreach(const Attribute* attribute, productDescriptor.getAttributes())
				{
					NetCDF::putAttribute(fileId, NC_GLOBAL, *attribute);
				}

		foreach(const Attribute* attribute, segmentDescriptor.getAttributes())
				{
					NetCDF::putAttribute(fileId, NC_GLOBAL, *attribute);
				}

		const long sizeK = grid.getSizeK();
		const long sizeL = grid.getMaxL() - grid.getMinL() + 1;
		const long sizeM = grid.getSizeM();
		const vector<Dimension*> dimensions = variableDescriptor.getDimensions();
		const size_t dimCount = dimensions.size();

		valarray<int> dimIds(dimCount);
		switch (dimCount) {
		case 3:
			dimIds[0] = NetCDF::defineDimension(fileId, dimensions[0]->getName(), sizeK);
			dimIds[1] = NetCDF::defineDimension(fileId, dimensions[1]->getName(), sizeL);
			dimIds[2] = NetCDF::defineDimension(fileId, dimensions[2]->getName(), sizeM);
			break;
		case 2:
			dimIds[0] = NetCDF::defineDimension(fileId, dimensions[0]->getName(), sizeL);
			dimIds[1] = NetCDF::defineDimension(fileId, dimensions[1]->getName(), sizeM);
			break;
		case 1:
			dimIds[0] = NetCDF::defineDimension(fileId, dimensions[0]->getName(), sizeM);
			break;
		default:
			BOOST_THROW_EXCEPTION( logic_error("SynL2Writer::createNcVar(): invalid number of dimensions (" + variableDescriptor.getName() + ")"));
			break;
		}

		ncFileIdMap[ncFileBasename] = fileId;
		ncDimIdMap.insert(make_pair(ncFileBasename, dimIds));
	}
	const int fileId = ncFileIdMap[ncFileBasename];
	const valarray<int>& dimIds = ncDimIdMap[ncFileBasename];
	const int varId = NetCDF::defineVariable(fileId, variableDescriptor.getNcVarName(), variableDescriptor.getType(), dimIds);
	ncVarIdMap[varName] = varId;

	foreach(const Attribute* attribute, variableDescriptor.getAttributes())
			{
				NetCDF::putAttribute(fileId, varId, *attribute);
			}
}
