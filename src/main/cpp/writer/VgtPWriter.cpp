/*
 * File:   VgtPWriter.cpp
 * Author: thomass
 *
 * Created on January 11, 2012, 16:30
 */

#include "VgtPWriter.h"

VgtPWriter::VgtPWriter() : AbstractWriter("VGP_WRITER") {
}

VgtPWriter::~VgtPWriter() {}

const string& VgtPWriter::getProductDescriptorIdentifier() const {
    return Constants::PRODUCT_VGTP;
}

const string& VgtPWriter::getSafeManifestName() const {
    return Constants::SAFE_MANIFEST_NAME_VGT_P;
}

const vector<SegmentDescriptor*> VgtPWriter::getSegmentDescriptors(const Context& context) const {
    const ProductDescriptor& productDescriptor = getProductDescriptor(context);
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
    vector<SegmentDescriptor*> commonSegments = getCommonSegments(context);
    foreach(SegmentDescriptor* commonSegment, commonSegments) {
        vector<VariableDescriptor*> commonVariables = commonSegment->getVariableDescriptors();
        foreach(VariableDescriptor* commonVariable, commonVariables) {
            // todo - implement
        }
    }
}

void VgtPWriter::defineCommonDimensions(int fileId, bool isSubsampled) {
    // todo - implement
}

void VgtPWriter::defineCommonVariables(int fileId, bool isSubsampled) {
    // todo - implement
}

const vector<SegmentDescriptor*> VgtPWriter::getCommonSegments(const Context& context) const {
    const ProductDescriptor& productDescriptor = getProductDescriptor(context);
    vector<SegmentDescriptor*> allSegmentDescriptors = productDescriptor.getSegmentDescriptors();
    vector<SegmentDescriptor*> commonSegmentDescriptors;
    foreach(SegmentDescriptor* segmentDescriptor, allSegmentDescriptors) {
        if(isCommonDescriptor(*segmentDescriptor)) {
            commonSegmentDescriptors.push_back(segmentDescriptor);
        }
    }
    return commonSegmentDescriptors;
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
