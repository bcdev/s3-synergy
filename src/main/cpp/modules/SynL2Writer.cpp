/*
 * File:   SynL2Writer.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 3:40 PM
 */

#include <stdexcept>

#include "../util/Utils.h"
#include "../util/NetCDF.h"
#include "SynL2Writer.h"

using std::make_pair;
using std::runtime_error;

SynL2Writer::SynL2Writer(const string& targetDirPath) : Writer("SYN_L2_WRITER"), targetDirPath(targetDirPath) {
}

SynL2Writer::~SynL2Writer() {
    pair<string, int> fileIdPair;
    foreach(fileIdPair, ncFileIdMap) {
        try {
            NetCDF::closeFile(fileIdPair.second);
        } catch (exception& ignored) {
        }
    }
}

void SynL2Writer::process(Context& context) {
    const Dictionary& dict = *context.getDictionary();
    const vector<SegmentDescriptor*> segmentDescriptors =
            dict.getProductDescriptor(Constants::PRODUCT_SYL2).getSegmentDescriptors();

    foreach(const SegmentDescriptor* segmentDescriptor, segmentDescriptors) {
        const string segmentName = segmentDescriptor->getName();
        if (context.hasSegment(segmentName)) {
            const Segment& segment = context.getSegment(segmentName);
            const Grid& grid = segment.getGrid();
            const vector<VariableDescriptor*> variableDescriptors =
                    segmentDescriptor->getVariableDescriptors();
            const size_t firstLWritable = context.getFirstLComputable(segment, *this);
            const size_t lastLWritable = context.getLastLWritable(segment, *this);

            foreach(const VariableDescriptor* variableDescriptor, variableDescriptors) {
                const string varName = variableDescriptor->getName();
                if (segment.hasVariable(varName)) {
                    const string ncFileBasename = variableDescriptor->getNcFileBasename();
                    if (!contains(ncVarIdMap, varName)) {
                        BOOST_THROW_EXCEPTION(logic_error("Unknown variable '" + varName + "'."));
                    }
                    if (!contains(ncFileIdMap, ncFileBasename)) {
                        BOOST_THROW_EXCEPTION(logic_error("Unknown netCDF file '" + ncFileBasename + "'."));
                    }
                    if (!contains(ncDimIdMap, ncFileBasename)) {
                        BOOST_THROW_EXCEPTION(logic_error("Unknown netCDF file '" + ncFileBasename + "'."));
                    }
                    const int varId = ncVarIdMap[varName];
                    const int ncId = ncFileIdMap[ncFileBasename];
                    const valarray<int>& dimIds = ncDimIdMap[ncFileBasename];
                    const valarray<size_t> starts = Utils::createStartVector(dimIds.size(), firstLWritable);
                    const valarray<size_t> sizes = Utils::createCountVector(dimIds.size(), grid.getSizeK(), lastLWritable - firstLWritable + 1, grid.getSizeM());
                    context.getLogging().progress("Writing variable " + varName + " of segment [" + segment.toString() + "]", getId());
                    const Accessor& accessor = segment.getAccessor(varName);
                    NetCDF::putData(ncId, varId, starts, sizes, accessor.getUntypedData());
                }
            }
            context.setLastLComputed(segment, *this, lastLWritable);
        }
    }
}

void SynL2Writer::start(Context& context) {
    const Dictionary& dict = *context.getDictionary();
    const ProductDescriptor& productDescriptor = dict.getProductDescriptor(Constants::PRODUCT_SYL2);
    const vector<SegmentDescriptor*> segmentDescriptors = productDescriptor.getSegmentDescriptors();

    foreach(SegmentDescriptor* segmentDescriptor, segmentDescriptors) {
        const string segmentName = segmentDescriptor->getName();
        if (context.hasSegment(segmentName)) {
            const Segment& segment = context.getSegment(segmentName);
            const vector<VariableDescriptor*> variableDescriptors =
                    segmentDescriptor->getVariableDescriptors();

            foreach(VariableDescriptor* variableDescriptor, variableDescriptors) {
                if (segment.hasVariable(variableDescriptor->getName())) {
                    createNcVar(productDescriptor, *segmentDescriptor, *variableDescriptor, segment.getGrid());
                }
            }
        }
    }

    pair<string, int> fileIdPair;

    foreach(fileIdPair, ncFileIdMap) {
        NetCDF::setDefinitionPhaseFinished(fileIdPair.second);
    }
}

void SynL2Writer::stop(Context& context) {
    pair<string, int> fileIdPair;

    foreach(fileIdPair, ncFileIdMap) {
        NetCDF::closeFile(fileIdPair.second);
    }
    ncVarIdMap.clear();
    ncDimIdMap.clear();
    ncFileIdMap.clear();
}

void SynL2Writer::createNcVar(
        const ProductDescriptor& productDescriptor,
        const SegmentDescriptor& segmentDescriptor,
        const VariableDescriptor& variableDescriptor,
        const Grid& grid) {
    const string varName = variableDescriptor.getName();
    const string ncFileBasename = variableDescriptor.getNcFileBasename();

    if (!contains(ncFileIdMap, ncFileBasename)) {
        if (!boost::filesystem::exists(targetDirPath)) {
            if (!boost::filesystem::create_directories(targetDirPath)) {
                BOOST_THROW_EXCEPTION(runtime_error("Cannot create directory '" + targetDirPath.string() + "'."));
            }
        }
        const path ncFilePath = targetDirPath / (variableDescriptor.getNcFileBasename() + ".nc");
        const int fileId = NetCDF::createFile(ncFilePath);

        foreach(const Attribute* attribute, productDescriptor.getAttributes()) {
            NetCDF::addAttribute(fileId, NC_GLOBAL, *attribute);
        }

        foreach(const Attribute* attribute, segmentDescriptor.getAttributes()) {
            NetCDF::addAttribute(fileId, NC_GLOBAL, *attribute);
        }

        const size_t sizeK = grid.getSizeK();
        const size_t sizeL = grid.getMaxL() - grid.getMinL() + 1;
        const size_t sizeM = grid.getSizeM();
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
                BOOST_THROW_EXCEPTION(logic_error("SynL2Writer::createNcVar(): invalid number of dimensions"));
        }

        ncFileIdMap[ncFileBasename] = fileId;
        ncDimIdMap.insert(make_pair(ncFileBasename, dimIds));
    }
    const int fileId = ncFileIdMap[ncFileBasename];
    const valarray<int>& dimIds = ncDimIdMap[ncFileBasename];
    const int varId = NetCDF::defineVariable(fileId, variableDescriptor.getNcVarName(), variableDescriptor.getType(), dimIds);
    ncVarIdMap[varName] = varId;

    foreach(const Attribute* attribute, variableDescriptor.getAttributes()) {
        NetCDF::addAttribute(fileId, varId, *attribute);
    }
}
