/*
 * Vpr.cpp
 *
 *  Created on: Dec 01, 2011
 *      Author: thomasstorm
 */

#include "Vpr.h"

using std::abs;
using std::fill;

Vpr::Vpr() : BasicModule("VPR") {
}

Vpr::~Vpr() {
}

void Vpr::start(Context& context) {
    collocatedSegment = &context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
    vgtSegment = &context.addSegment(Constants::SEGMENT_VGT_PLATE_CARREE, LINE_COUNT, COL_COUNT, 1, 0, LINE_COUNT - 1);
}

void Vpr::process(Context& context) {



}

double Vpr::getLatitude(long l) {
    if(l < 0 || l >= LINE_COUNT) {
        BOOST_THROW_EXCEPTION(std::invalid_argument("l < 0 || l >= LINE_COUNT, l = " + lexical_cast<string>(l)));
    }
    const double pixelSize = 1.0 / PIXELS_PER_DEGREE;
    const double unnormalisedResult = l * pixelSize;
    return 90 - unnormalisedResult;
}

double Vpr::getLongitude(long m) {
    if(m < 0 || m >= COL_COUNT) {
        BOOST_THROW_EXCEPTION(std::invalid_argument("m < 0 || m >= COL_COUNT, m = " + lexical_cast<string>(m)));
    }
    const double pixelSize = 1.0 / PIXELS_PER_DEGREE;
    const double unnormalisedResult = m * pixelSize;
    return -180 + unnormalisedResult;
}
