/*
 * File:   SynL2Writer.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 3:40 PM
 */

#include <fstream>
#include <stdexcept>

#include "../core/Config.h"
#include "../util/IOUtils.h"
#include "../util/NetCDF.h"

#include "SynL2Writer.h"

using std::runtime_error;

SynL2Writer::SynL2Writer() : AbstractWriter("SY2_WRITER") {

}

SynL2Writer::~SynL2Writer() {}

const string& SynL2Writer::getProductDescriptorIdentifier() const {
    return Constants::PRODUCT_SY2;
}

const string& SynL2Writer::getSafeManifestName() const {
    return Constants::SAFE_MANIFEST_NAME_SYN;
}

const vector<SegmentDescriptor*> SynL2Writer::getSegmentDescriptors(const Dictionary& dict) const {
    const ProductDescriptor& productDescriptor = dict.getProductDescriptor(getProductDescriptorIdentifier());
    return productDescriptor.getSegmentDescriptors();
}

void SynL2Writer::writeCommonVariables(const Context& context) {
    // for SYN, we don't need to write common variables
}

void SynL2Writer::defineCommonDimensions(int fileId, const string& segmentName, const Dictionary& dict, map<const VariableDescriptor*, valarray<int> >& commonDimIds) {
    // for SYN, we don't need to define common dimensions
}
void SynL2Writer::defineCommonVariables(int fileId, const string& segmentName, const Dictionary& dict, const map<const VariableDescriptor*, valarray<int> >& commonDimIds) {
    // for SYN, we don't need to define common variables
}

void SynL2Writer::resolveSubsampling(int fileId, const string& segmentName) {
    // for SYN, we don't need to care about subsampling
}
