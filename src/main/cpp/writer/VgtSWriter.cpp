/*
 * File:   VgtSWriter.cpp
 * Author: thomass
 *
 * Created on January 13, 2012, 11:34
 */

#include "VgtSWriter.h"

VgtSWriter::VgtSWriter() : AbstractWriter("VGS_WRITER") {

}

VgtSWriter::~VgtSWriter() {}

const string VgtSWriter::getProductDescriptorIdentifier() const {
    return Constants::PRODUCT_VGTS;
}

const string VgtSWriter::getSafeManifestName() const {
    return Constants::SAFE_MANIFEST_NAME_VGT_S;
}
