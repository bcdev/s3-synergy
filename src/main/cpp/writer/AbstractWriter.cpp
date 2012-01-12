/*
 * File:   AbstractWriter.cpp
 * Author: thomass
 *
 * Created on Jan 11, 2012, 16:29
 */

#include <fstream>
#include <stdexcept>

#include "../core/Config.h"
#include "../util/IOUtils.h"
#include "../util/NetCDF.h"

#include "AbstractWriter.h"

using std::runtime_error;

AbstractWriter::AbstractWriter(const string& name) :
        BasicModule(name) {
}

AbstractWriter::~AbstractWriter() {
	pair<string, int> fileIdPair;
	foreach(fileIdPair, ncFileIdMap)
			{
				try {
					NetCDF::closeFile(fileIdPair.second);
				} catch (exception& ignored) {
				}
			}
}

const ProductDescriptor& AbstractWriter::getProductDescriptor(const Context& context) const {
    const Dictionary& dict = context.getDictionary();
    return dict.getProductDescriptor(getProductDescriptorIdentifier());
}

const vector<SegmentDescriptor*> AbstractWriter::getSegmentDescriptors(const Context& context) const {
    const ProductDescriptor& sy2ProductDescriptor = getProductDescriptor(context);
    return sy2ProductDescriptor.getSegmentDescriptors();
}

void AbstractWriter::process(Context& context) {
	const vector<SegmentDescriptor*> segmentDescriptors = getSegmentDescriptors(context);

	foreach(const SegmentDescriptor* segmentDescriptor, segmentDescriptors) {
	    const string segmentName = segmentDescriptor->getName();
	    if (context.hasSegment(segmentName)) {
	        const Segment& segment = context.getSegment(segmentName);
	        const Grid& grid = segment.getGrid();
	        const vector<VariableDescriptor*> variableDescriptors = segmentDescriptor->getVariableDescriptors();
	        const long firstL = segment.getGrid().getFirstL();
	        context.getLogging().debug("Segment [" + segment.toString() + "]: firstL = " + lexical_cast<string>(firstL), getId());
	        const long lastL = segment.getGrid().getLastL();
	        context.getLogging().debug("Segment [" + segment.toString() + "]: lastL = " + lexical_cast<string>(lastL), getId());

	        if (firstL <= lastL) {
	            foreach(const VariableDescriptor* variableDescriptor, variableDescriptors) {
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
	                    const valarray<size_t> starts = IOUtils::createStartVector(dimIds.size(), firstL);
	                    const valarray<size_t> sizes = IOUtils::createCountVector(dimIds.size(), grid.getSizeK(), lastL - firstL + 1, grid.getSizeM());
	                    context.getLogging().progress("Writing variable " + varName + " of segment [" + segment.toString() + "]", getId());
	                    const Accessor& accessor = segment.getAccessor(varName);
	                    NetCDF::putData(ncId, varId, starts, sizes, accessor.getUntypedData());
	                }
	            }
	            context.setLastComputedL(segment, *this, lastL);
	        }
	    }
	}
}

void AbstractWriter::start(Context& context) {
	targetDirPath = path(context.getJobOrder().getIpfProcessors().at(0).getOutputList().at(0).getFileName());
	if (!targetDirPath.has_root_directory()) {
		targetDirPath = Constants::S3_SYNERGY_HOME / targetDirPath;
	}
	context.getLogging().info("target product path is '" + targetDirPath.string() + "'", getId());

	const ProductDescriptor& productDescriptor = getProductDescriptor(context);
	const vector<SegmentDescriptor*> segmentDescriptors = getSegmentDescriptors(context);

	foreach(SegmentDescriptor* segmentDescriptor, segmentDescriptors) {
	    const string segmentName = segmentDescriptor->getName();
	    if (context.hasSegment(segmentName)) {
	        const Segment& segment = context.getSegment(segmentName);
	        const vector<VariableDescriptor*> variableDescriptors = segmentDescriptor->getVariableDescriptors();
	        foreach(VariableDescriptor* variableDescriptor, variableDescriptors) {
	            context.getLogging().info("Defining variable for " + variableDescriptor->toString(), getId());
	            createNcVar(productDescriptor, *segmentDescriptor, *variableDescriptor, segment.getGrid());
	        }
	    }
	}

	pair<string, int> fileIdPair;

	foreach(fileIdPair, ncFileIdMap) {
	    NetCDF::terminateFile(fileIdPair.second);
	}
}

void AbstractWriter::stop(Context& context) {
	createSafeProduct(context);

	ncVarIdMap.clear();
	ncDimIdMap.clear();
	ncFileIdMap.clear();
}

void AbstractWriter::createNcVar(const ProductDescriptor& productDescriptor, const SegmentDescriptor& segmentDescriptor, const VariableDescriptor& variableDescriptor, const Grid& grid) {
	const string ncFileBasename = variableDescriptor.getNcFileBasename();

	if (!contains(ncFileIdMap, ncFileBasename)) {
		if (!boost::filesystem::exists(targetDirPath)) {
			if (!boost::filesystem::create_directories(targetDirPath)) {
				BOOST_THROW_EXCEPTION( runtime_error("Cannot create directory '" + targetDirPath.string() + "'."));
			}
		}
		const path ncFilePath = targetDirPath / (ncFileBasename + ".nc");
		const int fileId = NetCDF::createFile(ncFilePath.string());

		foreach(const Attribute* attribute, productDescriptor.getAttributes()) {
		    NetCDF::putGlobalAttribute(fileId, *attribute);
		}

		foreach(const Attribute* attribute, segmentDescriptor.getAttributes()) {
		    NetCDF::putGlobalAttribute(fileId, *attribute);
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
			BOOST_THROW_EXCEPTION( logic_error("AbstractWriter::createNcVar(): invalid number of dimensions (" + variableDescriptor.getName() + ")"));
			break;
		}

		ncFileIdMap[ncFileBasename] = fileId;
		ncDimIdMap.insert(make_pair(ncFileBasename, dimIds));
	}
	const int fileId = ncFileIdMap[ncFileBasename];
	const valarray<int>& dimIds = ncDimIdMap[ncFileBasename];
	const int varId = NetCDF::defineVariable(fileId, variableDescriptor.getNcVarName(), variableDescriptor.getType(), dimIds);
	ncVarIdMap[variableDescriptor.getName()] = varId;

	foreach(const Attribute* attribute, variableDescriptor.getAttributes()) {
	    NetCDF::putAttribute(fileId, varId, *attribute);
	}
}
