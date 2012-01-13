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
