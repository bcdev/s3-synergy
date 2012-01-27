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

void VgtPWriter::writeCommonVariables(Context& context) {
    valarray<int> fileIds = getFileIds();
    for (size_t i = 0; i < fileIds.size(); i++) {
        const int fileId = fileIds[i];
        foreach(SegmentDescriptor* sd, getCommonSegments(context.getDictionary())) {
            Segment& segment = context.getSegment(sd->getName());
            const vector<string> varNames = segment.getVariableNames();
            foreach(string var, varNames) {
                const int varId = NetCDF::getVariableId(fileId, var);
                if(varId == -1) {
                    continue;
                }
                context.getLogging().progress("Writing common variable '" + var + "'.", getId());
                const Accessor& accessor = segment.getAccessor(var);
                NetCDF::putData(fileId, varId, accessor.getUntypedData());
            }
            context.setLastComputedL(segment, *this, segment.getGrid().getMaxL());
        }
    }

//    valarray<int> fileIds = getFileIds();
//    for (size_t i = 0; i < fileIds.size(); i++) {
//        const int fileId = fileIds[i];
//        bool fileContainsSubsampledSegments = fileSubsampledMap[fileId];
//        vector<VariableDescriptor*> commonVariables;
//        if (fileContainsSubsampledSegments) {
//            commonVariables = getSubsampledCommonVariables(context.getDictionary());
//        } else {
//            commonVariables = getNonSubsampledCommonVariables(context.getDictionary());
//        }
//        valarray<size_t> origin;
//        valarray<size_t> shape;
//        foreach (VariableDescriptor* commonVariable, commonVariables) {
//            context.getLogging().debug("Writing common variable '" + commonVariable->getName() + "'.", getId());
//            const int varId = NetCDF::getVariableId(fileId, commonVariable->getName());
//            const Segment& segment = context.getSegment(commonVariable->getSegmentName());
//            const Accessor& accessor = segment.getAccessor(commonVariable->getName());
//            NetCDF::putData(fileId, varId, accessor.getUntypedData());
//        }
//    }
}

void VgtPWriter::defineCommonDimensions(int fileId, const string& segmentName, const Dictionary& dict, map<const VariableDescriptor*, valarray<int> >& commonDimIds) {
    vector<VariableDescriptor*> variables;
    if(isSubsampledSegment(segmentName)) {
        variables = getSubsampledCommonVariables(dict);
    } else {
        variables = getNonSubsampledCommonVariables(dict);
    }
    for(size_t i = 0; i < variables.size(); i++) {
        const VariableDescriptor* variableDescriptor = variables[i];
        valarray<int> dimIds(variableDescriptor->getDimensions().size());
        for(size_t j = 0; j < variableDescriptor->getDimensions().size(); j++) {
        	const Dimension* dim = variableDescriptor->getDimensions()[j];
        	int dimId = NetCDF::findDimension(fileId, dim->getName());
        	if(dimId == -1) {
        		dimId = NetCDF::defineDimension(fileId, dim->getName(), dim->getSize());
        	}
        	dimIds[j] = dimId;
        }
        commonDimIds[variableDescriptor] = dimIds;
    }
}

void VgtPWriter::defineCommonVariables(int fileId, const string& segmentName, const Dictionary& dict, const map<const VariableDescriptor*, valarray<int> >& commonDimIds) {
    vector<VariableDescriptor*> variables;
    if(isSubsampledSegment(segmentName)) {
        variables = getSubsampledCommonVariables(dict);
    } else {
        variables = getNonSubsampledCommonVariables(dict);
    }
    foreach(VariableDescriptor* variableDescriptor, variables) {
        NetCDF::defineVariable(fileId, variableDescriptor->getName(), variableDescriptor->getType(), commonDimIds.at(variableDescriptor));
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
