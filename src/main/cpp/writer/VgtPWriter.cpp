/*
 * File:   VgtPWriter.cpp
 * Author: thomass
 *
 * Created on January 11, 2012, 16:30
 */

#include "VgtPWriter.h"
#include "../util/IOUtils.h"

VgtPWriter::VgtPWriter() : AbstractWriter("VGP_WRITER"), fileSubsampledMap() {
}

VgtPWriter::~VgtPWriter() {}

const string& VgtPWriter::getProductDescriptorIdentifier() const {
    return Constants::PRODUCT_VGP;
}

const string& VgtPWriter::getSafeManifestName() const {
    return Constants::SAFE_MANIFEST_NAME_VGT_P;
}

const vector<SegmentDescriptor*> VgtPWriter::getSegmentDescriptors(const Dictionary& dict) const {
    const ProductDescriptor& productDescriptor = dict.getProductDescriptor(getProductDescriptorIdentifier());
    vector<SegmentDescriptor*> allSegmentDescriptors = productDescriptor.getSegmentDescriptors();
    vector<SegmentDescriptor*> nonCommonSegmentDescriptors;
    foreach(SegmentDescriptor* segmentDescriptor, allSegmentDescriptors) {
        if(!isCommonDescriptor(*segmentDescriptor)) {
            nonCommonSegmentDescriptors.push_back(segmentDescriptor);
        }
    }
    return nonCommonSegmentDescriptors;
}

void VgtPWriter::writeCommonVariables(const Context& context) {
    valarray<int> fileIds = getFileIds();
    for(size_t i = 0; i < fileIds.size(); i++) {
        const int fileId = fileIds[i];
        bool fileContainsSubsampledSegments = fileSubsampledMap[fileId];
        vector<VariableDescriptor*> commonVariables;
        if(fileContainsSubsampledSegments) {
            commonVariables = getSubsampledCommonVariables(context.getDictionary());
        } else {
            commonVariables = getNonSubsampledCommonVariables(context.getDictionary());
        }
        valarray<size_t> origin;
        valarray<size_t> shape;
        foreach(VariableDescriptor* commonVariable, commonVariables) {
            const int varId = NetCDF::getVariableId(fileId, commonVariable->getName());
            const Segment& segment = context.getSegment(commonVariable->getSegmentName());
            const Grid& grid = segment.getGrid();
            const Accessor& accessor = segment.getAccessor(commonVariable->getName());
            const size_t dimCount = commonVariable->getDimensions().size();
            IOUtils::createStartVector(dimCount, grid.getFirstL(), origin);
            IOUtils::createCountVector(dimCount, grid.getSizeK(), grid.getLastL() - grid.getFirstL() + 1, grid.getSizeM(), shape);
            if (accessor.canReturnDataPointer()) {
                NetCDF::putData(fileId, varId, origin, shape, accessor.getUntypedData());
            } else {
                valarray<size_t> indices(2);
                const long firstL = segment.getGrid().getFirstL();
                const long lastL = segment.getGrid().getLastL();
                for (long l = firstL; l <= lastL; l++) {
                    for (long m = grid.getFirstM(); m <= grid.getMaxM(); m++) {
                        const size_t index = grid.getIndex(0, l, m);
                        indices[0] = l;
                        indices[1] = m;
                        NetCDF::putValue(fileId, varId, indices, accessor.getUntypedValue(index));
                    }
                }
            }
        }
    }
}

void VgtPWriter::defineCommonDimensions(int fileId, const string& segmentName, const Dictionary& dict, map<const VariableDescriptor*, int>& commonDimIds) {
    vector<VariableDescriptor*> variables;
    if(isSubsampledSegment(segmentName)) {
        variables = getSubsampledCommonVariables(dict);
    } else {
        variables = getNonSubsampledCommonVariables(dict);
    }
    // utilize that there is exactly one dimension for each variable and vice versa
    for(size_t i = 0; i < variables.size(); i++) {
        const VariableDescriptor* variableDescriptor = variables[i];
        const Dimension* dim = variableDescriptor->getDimensions()[0];
        int dimId = NetCDF::findDimension(fileId, dim->getName());
        if(dimId == -1) {
            dimId = NetCDF::defineDimension(fileId, dim->getName(), dim->getSize());
        }
        commonDimIds[variableDescriptor] = dimId;
    }
}

void VgtPWriter::defineCommonVariables(int fileId, const string& segmentName, const Dictionary& dict, const map<const VariableDescriptor*, int>& commonDimIds) {
    vector<VariableDescriptor*> variables;
    if(isSubsampledSegment(segmentName)) {
        variables = getSubsampledCommonVariables(dict);
    } else {
        variables = getNonSubsampledCommonVariables(dict);
    }
    valarray<int> dimIds(1);
    foreach(VariableDescriptor* variableDescriptor, variables) {
        dimIds[0] = commonDimIds.at(variableDescriptor);
        NetCDF::defineVariable(fileId, variableDescriptor->getName(), variableDescriptor->getType(), dimIds);
    }
}

void VgtPWriter::resolveSubsampling(int fileId, const string& segmentName) {
    fileSubsampledMap[fileId] = isSubsampledSegment(segmentName);
}

valarray<int> VgtPWriter::getFileIds() {
    valarray<int> fileIds(fileSubsampledMap.size());
    typedef pair<int, bool> MapInput;
    int i = 0;
    foreach(MapInput mapInput, fileSubsampledMap) {
        fileIds[i] = mapInput.first;
        i++;
    }
    return fileIds;
}

const vector<VariableDescriptor*> VgtPWriter::getSubsampledCommonVariables(const Dictionary& dict) const {
    vector<SegmentDescriptor*> commonSegments = getCommonSegments(dict);
    vector<VariableDescriptor*> commonVariablesResult;
    foreach(SegmentDescriptor* commonSegment, commonSegments) {
        if (isSubsampledCommonSegment(commonSegment->getName())) {
            vector<VariableDescriptor*> commonVariables = commonSegment->getVariableDescriptors();
            foreach(VariableDescriptor* commonVariable, commonVariables) {
                commonVariablesResult.push_back(commonVariable);
            }
        }
    }
    return commonVariablesResult;
}

const vector<VariableDescriptor*> VgtPWriter::getNonSubsampledCommonVariables(const Dictionary& dict) const {
    vector<SegmentDescriptor*> commonSegments = getCommonSegments(dict);
    vector<VariableDescriptor*> nonSubsampledCommonVariables;
    foreach(SegmentDescriptor* commonSegment, commonSegments) {
        if (!isSubsampledCommonSegment(commonSegment->getName())) {
            vector<VariableDescriptor*> commonVariables = commonSegment->getVariableDescriptors();
            foreach(VariableDescriptor* commonVariable, commonVariables) {
                nonSubsampledCommonVariables.push_back(commonVariable);
            }
        }
    }
    return nonSubsampledCommonVariables;
}

const vector<SegmentDescriptor*> VgtPWriter::getCommonSegments(const Dictionary& dict) const {
    const ProductDescriptor& productDescriptor = dict.getProductDescriptor(getProductDescriptorIdentifier());
    vector<SegmentDescriptor*> allSegmentDescriptors = productDescriptor.getSegmentDescriptors();
    vector<SegmentDescriptor*> commonSegmentDescriptors;
    foreach(SegmentDescriptor* segmentDescriptor, allSegmentDescriptors) {
        if(isCommonDescriptor(*segmentDescriptor)) {
            commonSegmentDescriptors.push_back(segmentDescriptor);
        }
    }
    return commonSegmentDescriptors;
}

bool VgtPWriter::isSubsampledCommonSegment(const string& segmentName) const {
    return segmentName.compare(Constants::SEGMENT_VGP_LAT_TP) == 0
            || segmentName.compare(Constants::SEGMENT_VGP_LON_TP) == 0
            || segmentName.compare(Constants::SEGMENT_VGP_LAT_BNDS) == 0
            || segmentName.compare(Constants::SEGMENT_VGP_LON_BNDS) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LAT_TP) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LON_TP) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LAT_BNDS) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LON_BNDS) == 0;
}

bool VgtPWriter::isSubsampledSegment(const string& segmentName) const {
    return segmentName.compare(Constants::SEGMENT_OLC_TP) == 0
            || segmentName.compare(Constants::SEGMENT_OLC_TP) == 0
            || segmentName.compare(Constants::SEGMENT_SLN_TP) == 0
            || segmentName.compare(Constants::SEGMENT_SLO_TP) == 0;
}

bool VgtPWriter::isCommonDescriptor(const SegmentDescriptor& segmentDescriptor) const {
    const string& segmentName = segmentDescriptor.getName();
    return segmentName.compare(Constants::SEGMENT_VGP_LAT) == 0
            || segmentName.compare(Constants::SEGMENT_VGP_LAT_TP) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LAT) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LAT_TP) == 0
            || segmentName.compare(Constants::SEGMENT_VGP_LON) == 0
            || segmentName.compare(Constants::SEGMENT_VGP_LON_TP) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LON) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LON_TP) == 0
            || segmentName.compare(Constants::SEGMENT_VGP_LAT_BNDS) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LAT_BNDS) == 0
            || segmentName.compare(Constants::SEGMENT_VGP_LAT_TP_BNDS) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LAT_TP_BNDS) == 0
            || segmentName.compare(Constants::SEGMENT_VGP_LON_BNDS) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LON_BNDS) == 0
            || segmentName.compare(Constants::SEGMENT_VGP_LON_TP_BNDS) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LON_TP_BNDS) == 0;
}
