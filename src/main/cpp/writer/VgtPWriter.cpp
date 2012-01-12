/*
 * File:   VgtPWriter.cpp
 * Author: thomass
 *
 * Created on January 11, 2012, 16:30
 */

#include <fstream>
#include <stdexcept>

#include "VgtPWriter.h"

VgtPWriter::VgtPWriter() : AbstractWriter("VGP_WRITER") {

}

const string VgtPWriter::getProductDescriptorIdentifier() const {
    return Constants::PRODUCT_VGTP;
}

void VgtPWriter::createSafeProduct(const Context& context) {
	// don't create a SAFE product from VGT
}
