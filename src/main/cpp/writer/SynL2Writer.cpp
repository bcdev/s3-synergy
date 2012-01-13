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
