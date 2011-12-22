/*
 * Vpr.cpp
 *
 *  Created on: Dec 01, 2011
 *      Author: thomasstorm
 */

#include "Vpr.h"

using std::abs;
using std::fill;
using std::invalid_argument;
using std::min;

Vpr::Vpr() : BasicModule("VPR") {
}

Vpr::~Vpr() {
}

void Vpr::start(Context& context) {
    collocatedSegment = &context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
    vgtSegment = &context.addSegment(Constants::SEGMENT_VGT_PLATE_CARREE, LINE_COUNT, COL_COUNT, 1, 0, LINE_COUNT - 1);
    const Segment& geoSegment = context.getSegment(Constants::SEGMENT_GEO);
    geoGrid = &geoSegment.getGrid();
    latAccessor = &geoSegment.getAccessor("latitude");
    lonAccessor = &geoSegment.getAccessor("longitude");
}

void Vpr::process(Context& context) {
    valarray<shared_ptr<Pixel> > pixels(5);
    for(size_t i = 0; i < 5; i++) {
        pixels[i] = shared_ptr<Pixel>(new Pixel());
    }

    valarray<long> synIndices(3);

    const Grid& vgtGrid = vgtSegment->getGrid();
    const Grid& synGrid = collocatedSegment->getGrid();

    const long firstL = context.getFirstComputableL(*vgtSegment, *this);
    const long lastL = context.getLastComputableL(*vgtSegment, *this);

    for(long l = firstL; l <= lastL; l++) {
        for(long m = vgtGrid.getFirstM(); m <= vgtGrid.getMaxM(); m++) {
            const double lat = getLatitude(l);
            const double lon = getLongitude(m);
            findPixelPos(lat, lon, synIndices);
            const long synK = synIndices[0];
            const long synL = synIndices[1];
            const long synM = synIndices[2];
            if (synGrid.isValidPosition(synK, synL, synM)) {
                setupPixel(pixels[synK], synK, synL, synM);
            } else {
                // todo - set last computed line and/or first required line and return
                return;
            }
            const Pixel& pixel = findClosestPixel(pixels, lat, lon);
            setValues(pixel, l, m);
        }
    }
}

void Vpr::findPixelPos(double lat, double lon, valarray<long>& synIndices) const {
    findPixelPos(lat, lon,
            geoGrid->getFirstK(), geoGrid->getMaxK(),
            geoGrid->getFirstL(), geoGrid->getMaxL(),
            geoGrid->getFirstM(), geoGrid->getMaxM(),
            synIndices);
}

void Vpr::findPixelPosAroundGivenIndices(double lat, double lon, valarray<long>& synIndices) const {
    const size_t radius = 10;
    const long k0 = max<long>(0, synIndices[0] - radius / 2);
    const long kMax = min<long>(geoGrid->getMaxK(), synIndices[0] + radius / 2);
    const long l0 = max<long>(0, synIndices[1] - radius / 2);
    const long lMax = min<long>(geoGrid->getMaxL(), synIndices[1] + radius / 2);
    const long m0 = max<long>(0, synIndices[2] - radius / 2);
    const long mMax = min<long>(geoGrid->getMaxM(), synIndices[2] + radius / 2);
    findPixelPos(lat, lon, k0, kMax, l0, lMax, m0, mMax, synIndices);
}

void Vpr::findPixelPos(double lat, double lon, long k0, long kMax, long l0, long lMax, long m0, long mMax, valarray<long>& synIndices) const {
    double synLat;
    double synLon;
    double delta = numeric_limits<double>::max();
    for (long k = k0; k <= kMax; k++) {
        for (long l = l0; l <= lMax; l++) {
            for (long m = m0; m <= mMax; m++) {
                const size_t index = geoGrid->getIndex(k, l, m);
                synLat = latAccessor->getDouble(index);
                synLon = lonAccessor->getDouble(index);
                double innerDelta = std::abs(lat - synLat) + std::abs(lon - synLon);
                if(innerDelta < delta) {
                    delta = innerDelta;
                    synIndices[0] = k;
                    synIndices[1] = l;
                    synIndices[2] = m;
                }
            }
        }
    }
}

void Vpr::setupPixel(shared_ptr<Pixel> p, long synK, long synL, long synM) const {
    // todo - implement
}

Pixel& Vpr::findClosestPixel(const valarray<shared_ptr<Pixel> >& pixels, double lat, double lon) const {
    if(pixels.size() == 0) {
        BOOST_THROW_EXCEPTION(invalid_argument("No pixels given."));
    }
    Pixel& closestPixel = *(pixels[0]);
    double delta = numeric_limits<double>::max();
    for(size_t i = 0; i < pixels.size(); i++) {
        const Pixel& currentPixel = *(pixels[i]);
        double innerDelta = std::abs(currentPixel.lat - lat) + std::abs(currentPixel.lon - lon);
        if(innerDelta < delta) {
            delta = innerDelta;
            closestPixel = currentPixel;
        }
    }

    return closestPixel;
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
