/*
 * File:   SynL2Writer.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 3:40 PM
 */

#include <fstream>
#include <stdexcept>

#include "../core/Config.h"
#include "../core/NetCDF.h"

#include "SynL2Writer.h"

using std::runtime_error;

SynL2Writer::SynL2Writer() : AbstractWriter("SY2_WRITER") {

}

SynL2Writer::~SynL2Writer() {}

const string& SynL2Writer::getProductId() const {
    return Constants::PRODUCT_SY2;
}

const string& SynL2Writer::getSafeManifestName() const {
    return Constants::SAFE_MANIFEST_NAME_SYN;
}

const vector<SegmentDescriptor*> SynL2Writer::getSegmentDescriptors(const Dictionary& dict) const {
    return dict.getProductDescriptor(getProductId()).getSegmentDescriptors();
}

void SynL2Writer::writeCoordinateVariables(Context& context) {
    // for SYN, we don't need to write coordinate variables
}

void SynL2Writer::defineCoordinateDimensions(int fileId, const string& segmentName, const Dictionary& dict, map<const VariableDescriptor*, valarray<int> >& commonDimIds) {
    // for SYN, we don't need to define coordinate dimensions
}
void SynL2Writer::defineCoordinateVariables(int fileId, const string& segmentName, const Dictionary& dict, const map<const VariableDescriptor*, valarray<int> >& commonDimIds) {
    // for SYN, we don't need to define coordinate variables
}
