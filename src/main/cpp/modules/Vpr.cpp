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
    /*
     * depending on the current latitude value, move VGT or SYN segment, not both
     *
     * if latitude of last (unsure; or first) valid pixel of current VGT segment is greater than the latitude of the
     * last (first) valid pixel of the current SYN segment, move only the VGT segment by setting its last computed line
     *
     * if latitude of last (unsure; or first) valid pixel of current VGT segment is less than the latitude of the
     * last (first) valid pixel of the current SYN segment, move only the SYN segment by setting its first required line
     */
    valarray<shared_ptr<Pixel> > pixels(5);
    for(size_t i = 0; i < 5; i++) {
        pixels[i] = shared_ptr<Pixel>(new Pixel());
    }

    valarray<long> synIndices(2);

    const Grid& vgtGrid = vgtSegment->getGrid();
    const Grid& synGrid = collocatedSegment->getGrid();

    const long firstL = context.getFirstComputableL(*vgtSegment, *this);
    const long lastL = context.getLastComputableL(*vgtSegment, *this);

    for(long l = firstL; l <= lastL; l++) {
        for(long m = vgtGrid.getFirstM(); m <= vgtGrid.getMaxM(); m++) {
            const double lat = getLatitude(l);
            const double lon = getLongitude(m);
            for(long synK = 0; synK < 5; synK++) {
                getPixelPos(lat, lon, synIndices);
                const long synL = synIndices[0];
                const long synM = synIndices[1];
                if(synGrid.isValidPosition(synK, synL, synM)) {
                    setupPixel(pixels[synK], synK, synL, synM);
                } else {
                    // todo - set last computed line and/or first required line and return
                    return;
                }
            }
            const Pixel& pixel = findClosestPixel(pixels, lat, lon);
            setValues(pixel, l, m);
        }
    }
}

void Vpr::getPixelPos(double lat, double lon, valarray<long>& synIndices) {
    // todo - implement IGL algorithm
}

void Vpr::setupPixel(shared_ptr<Pixel> p, long synK, long synL, long synM) {
    // todo - implement
}

Pixel& Vpr::findClosestPixel(const valarray<shared_ptr<Pixel> >& pixels, double lat, double lon) {
    // todo - implement
}

void Vpr::setValues(const Pixel& p, long l, long m) {
    // todo - implement
}

double Vpr::getLatitude(long l) {
    if(l < 0 || l >= LINE_COUNT) {
        BOOST_THROW_EXCEPTION(std::invalid_argument("l < 0 || l >= LINE_COUNT, l = " + lexical_cast<string>(l)));
    }
    const double unnormalisedResult = l * PIXEL_SIZE;
    return 75 - unnormalisedResult;
}

double Vpr::getLongitude(long m) {
    if(m < 0 || m >= COL_COUNT) {
        BOOST_THROW_EXCEPTION(std::invalid_argument("m < 0 || m >= COL_COUNT, m = " + lexical_cast<string>(m)));
    }
    const double unnormalisedResult = m * PIXEL_SIZE;
    return -180 + unnormalisedResult;
}
