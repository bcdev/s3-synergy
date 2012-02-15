
/*
 * File:   AbstractWriter.cpp
 * Author: thomass
 *
 * Created on Jan 11, 2012, 16:29
 */

#include <stdexcept>

#include "../core/NetCDF.h"
#include "../util/Utils.h"

#include "AbstractWriter.h"

using std::runtime_error;

AbstractWriter::AbstractWriter(const string& name, const string& productId) :
        BasicModule(name), productId(productId) {
}

AbstractWriter::~AbstractWriter() {
	pair<string, int> fileIdPair;
	reverse_foreach(fileIdPair, ncFileIdMap)
			{
				try {
					NetCDF::closeFile(fileIdPair.second);
				} catch (exception& ignored) {
				}
			}
}

void AbstractWriter::start(Context& context) {
	targetDirPath = path(context.getJobOrder().getIpfProcessors().at(0).getOutputList().at(0).getFileName());
	if (!targetDirPath.has_root_directory()) {
		targetDirPath = Constants::S3_SYNERGY_HOME / targetDirPath;
	}
	context.getLogging().info("target product path is '" + targetDirPath.string() + "'", getId());

	const Dictionary& dict = context.getDictionary();
	const ProductDescriptor& productDescriptor = dict.getProductDescriptor(productId);
	const vector<SegmentDescriptor*> segmentDescriptors = getSegmentDescriptors(productDescriptor);

	foreach (SegmentDescriptor* segmentDescriptor, segmentDescriptors)
			{
				const string segmentName = segmentDescriptor->getName();
				if (context.hasSegment(segmentName)) {
					const Segment& segment = context.getSegment(segmentName);
					const vector<VariableDescriptor*> variableDescriptors = segmentDescriptor->getVariableDescriptors();
					foreach (VariableDescriptor* variableDescriptor, variableDescriptors)
							{
								context.getLogging().info("Defining variable for " + variableDescriptor->toString(), getId());
								defineVariable(context, productDescriptor, *segmentDescriptor, *variableDescriptor, segment.getGrid());
							}
				}
			}

	pair<string, int> fileIdPair;
	foreach(fileIdPair, ncFileIdMap)
			{
				NetCDF::terminateFile(fileIdPair.second);
			}
}

void AbstractWriter::process(Context& context) {
	const ProductDescriptor& productDescriptor = context.getDictionary().getProductDescriptor(productId);
	const vector<SegmentDescriptor*> segmentDescriptors = getSegmentDescriptors(productDescriptor);

	foreach (const SegmentDescriptor* segmentDescriptor, segmentDescriptors)
			{
				const string segmentName = segmentDescriptor->getName();
				if (context.hasSegment(segmentName)) {
					const Segment& segment = context.getSegment(segmentName);
					const Grid& grid = segment.getGrid();
					const long firstL = segment.getGrid().getMinInMemoryL();
					context.getLogging().debug("Segment [" + segment.toString() + "]: firstL = " + lexical_cast<string>(firstL), getId());
					const long lastL = segment.getGrid().getMaxInMemoryL();
					context.getLogging().debug("Segment [" + segment.toString() + "]: lastL = " + lexical_cast<string>(lastL), getId());

					if (firstL <= lastL) {
						const vector<VariableDescriptor*> variableDescriptors = segmentDescriptor->getVariableDescriptors();
						foreach (const VariableDescriptor* variableDescriptor, variableDescriptors)
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
										const int fileId = ncFileIdMap[ncFileBasename];
										const valarray<int>& dimIds = ncDimIdMap[ncFileBasename];
										context.getLogging().progress("Writing variable '" + varName + "' of segment [" + segment.toString() + "]", getId());
										const Accessor& accessor = segment.getAccessor(varName);

										valarray<size_t> start;
										valarray<size_t> count;

										Utils::createStartVector(dimIds.size(), firstL, start);
										Utils::createCountVector(dimIds.size(), grid.getSizeK(), lastL - firstL + 1, grid.getSizeM(), count);
										NetCDF::putData(fileId, varId, start, count, accessor.getUntypedData());
										writeCoordinateVariables(context, fileId, productDescriptor, segmentName);
									}
								}
						context.setLastComputedL(segment, *this, lastL);
					}
				}
			}
}

void AbstractWriter::stop(Context& context) {
	pair<string, int> entry;
	reverse_foreach(entry, ncFileIdMap)
			{
				try {
					NetCDF::closeFile(entry.second);
				} catch (exception& ignored) {
				}
			}
	ncVarIdMap.clear();
	ncDimIdMap.clear();
	ncFileIdMap.clear();
}

void AbstractWriter::defineVariable(const Context& context, const ProductDescriptor& productDescriptor, const SegmentDescriptor& segmentDescriptor, const VariableDescriptor& variableDescriptor, const Grid& grid) {
	const string ncFileBasename = variableDescriptor.getNcFileBasename();

	const bool fileForVariableExists = contains(ncFileIdMap, ncFileBasename);
	if (!fileForVariableExists) {
		if (!boost::filesystem::exists(targetDirPath)) {
			if (!boost::filesystem::create_directories(targetDirPath)) {
				BOOST_THROW_EXCEPTION( runtime_error("Cannot create directory '" + targetDirPath.string() + "'."));
			}
		}
		const path ncFilePath = targetDirPath / (ncFileBasename + ".nc");
		const int fileId = NetCDF::createFile(ncFilePath.string());
		const string& variableName = variableDescriptor.getName();

		valarray<int> dimIds;
		NetCDF::defineDimensions(fileId, variableName, variableDescriptor.getDimensions(), grid, dimIds);
		ncFileIdMap.insert(make_pair(ncFileBasename, fileId));
		ncDimIdMap.insert(make_pair(ncFileBasename, dimIds));

		putGlobalAttributes(fileId, ncFileBasename, productDescriptor.getAttributes());
		defineCoordinateVariables(context, fileId, productDescriptor, segmentDescriptor.getName());
	}
	const int fileId = ncFileIdMap.at(ncFileBasename);
	const valarray<int>& dimIds = ncDimIdMap.at(ncFileBasename);
	const int varId = NetCDF::defineVariable(fileId, variableDescriptor.getNcVarName(), variableDescriptor.getType(), dimIds);
	ncVarIdMap.insert(make_pair(variableDescriptor.getName(), varId));

	foreach (const Attribute* attribute, variableDescriptor.getAttributes())
			{
				NetCDF::putAttribute(fileId, varId, *attribute);
			}
}

void AbstractWriter::putGlobalAttributes(int fileId, const string& ncFileBasename, const vector<Attribute*>& attributes) const {
	foreach (Attribute* attribute, attributes)
			{
				const string& attributeName = attribute->getName();
				if (attributeName.compare("title") == 0) {
					// TODO - define
					attribute->setValue("TBD");
				} else if (attributeName.compare("comment") == 0) {
					// TODO - define
					attribute->setValue("TBD");
				} else if (attributeName.compare("processor_version") == 0) {
					attribute->setValue(Constants::PROCESSOR_VERSION);
				} else if (attributeName.compare("dataset_name") == 0) {
					attribute->setValue(ncFileBasename);
				} else if (attributeName.compare("dataset_version") == 0) {
					attribute->setValue(Constants::DATASET_VERSION);
				} else if (attributeName.compare("package_name") == 0) {
					attribute->setValue(targetDirPath.filename().string());
				} else if (attributeName.compare("creation_time") == 0) {
					// TODO - make helper class
					time_t currentTime;
					time(&currentTime);
					struct tm* currentTimeStructure = gmtime(&currentTime);
					char buffer[80];
					strftime(buffer, 80, "UTC=%Y-%m-%dT%H:%M:%S.000000", currentTimeStructure);
					attribute->setValue(string(buffer));
				}
				NetCDF::putGlobalAttribute(fileId, *attribute);
			}
}
