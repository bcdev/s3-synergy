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

Vpr::Vpr() : BasicModule("VPR"),
        synReflectanceAccessors(4),
        vgtReflectanceAccessors(4) {
}

Vpr::~Vpr() {
}

void Vpr::start(Context& context) {
    synSegment = &context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
    geoSegment = &context.getSegment(Constants::SEGMENT_GEO);
    olcSegment = &context.getSegment(Constants::SEGMENT_OLC);
    slnSegment = &context.getSegment(Constants::SEGMENT_SLN);
    sloSegment = &context.getSegment(Constants::SEGMENT_SLO);

    latAccessor = &geoSegment->getAccessor("latitude");
    lonAccessor = &geoSegment->getAccessor("longitude");

    context.getLogging().info("Adding segment '" + Constants::SEGMENT_VGP + "' to context.", getId());
    vgpSegment = &context.addSegment(Constants::SEGMENT_VGP, 400, COL_COUNT, 1, 0, LINE_COUNT - 1);

    setupAccessors();
}

void Vpr::setupAccessors() {
    synReflectanceAccessors[0] = &synSegment->getAccessor("B0");
    synReflectanceAccessors[1] = &synSegment->getAccessor("B2");
    synReflectanceAccessors[2] = &synSegment->getAccessor("B3");
    synReflectanceAccessors[3] = &synSegment->getAccessor("MIR");
    synFlagsAccessor = &synSegment->getAccessor("SM");

    vgtReflectanceAccessors[0] = &vgpSegment->addVariable("B0", Constants::TYPE_DOUBLE);
    vgtReflectanceAccessors[1] = &vgpSegment->addVariable("B2", Constants::TYPE_DOUBLE);
    vgtReflectanceAccessors[2] = &vgpSegment->addVariable("B3", Constants::TYPE_DOUBLE);
    vgtReflectanceAccessors[3] = &vgpSegment->addVariable("MIR", Constants::TYPE_DOUBLE);
    vgtFlagsAccessor = &vgpSegment->addVariable("SM", Constants::TYPE_UBYTE);
}

void Vpr::process(Context& context) {
    const long firstL = context.getFirstComputableL(*vgpSegment, *this);
    const long lastL = context.getLastComputableL(*vgpSegment, *this);

    double synMinLat = 90.0;
    double synMaxLat = -90.0;
    getMinMaxSynLat(synMinLat, synMaxLat);
    double vgtMinLat = 90.0;
    double vgtMaxLat = -90.0;
    getMinMaxVgtLat(firstL, lastL, vgtMinLat, vgtMaxLat);

    /*
     * if the minimum latitude of the current VGT segment is greater than the maximum latitude
     * of the current SYN segment, move the VGT segment forward and do not allow moving
     * of the SYN segment.
     */
    if (synMaxLat < vgtMinLat && context.getLastComputableL(*vgpSegment, *this) < vgpSegment->getGrid().getMaxL()) {
		context.setLastComputedL(*vgpSegment, *this, lastL);
		context.setFirstRequiredL(*synSegment, *this, synSegment->getGrid().getFirstL());
		// TODO - do this somewhere else
		context.setFirstRequiredL(*olcSegment, *this, olcSegment->getGrid().getFirstL());
		context.setFirstRequiredL(*slnSegment, *this, slnSegment->getGrid().getFirstL());
		context.setFirstRequiredL(*sloSegment, *this, sloSegment->getGrid().getFirstL());
		return;
	}

    /*
     * if the maximum latitude of the current VGT segment is less than the minimum latitude
     * of the current SYN segment, do not allow moving of the VGT segment and move
     * the SYN segment forward, if possible.
     */
    if (vgtMaxLat < synMinLat && context.getLastComputableL(*synSegment, *this) < synSegment->getGrid().getMaxL()) {
		context.setFirstRequiredL(*vgpSegment, *this, firstL);
		return;
	}

    /*
     * if the minimum latitude of the current VGT segment is less than the maximum latitude
     * of the current SYN segment AND the minimum VGT latitude is greater than the minumum
     * SYN latitude (i.e. the VGT segment and the SYN segment overlap), perform the projection
     * and set the first required line of the SYN segment to the line of the SYN segment nearest
     * to vgtMaxLat.
     */
    context.setLastComputedL(*vgpSegment, *this, lastL);
    context.setFirstRequiredL(*vgpSegment, *this, -1); // no particular requirement on VGP line
    context.setFirstRequiredL(*synSegment, *this, -1); // no particular requirement on SYN line
	// TODO - do this somewhere else
	context.setFirstRequiredL(*olcSegment, *this, -1);
	context.setFirstRequiredL(*slnSegment, *this, -1);
	context.setFirstRequiredL(*sloSegment, *this, -1);

    /*
    long l = findLineOfSynSegmentNearestTo(vgtMaxLat);
    context.setFirstRequiredL(*synSegment, *this, l);

    double synMinLon = 180.0;
    double synMaxLon = -180.0;
    minMaxSynLon(synMinLon, synMaxLon);

    const Grid& vgtGrid = vgtSegment->getGrid();
    const Grid& synGrid = synSegment->getGrid();
    valarray<long> synIndices(3);
    bool indicesFound = false;
    for(long l = firstL; l <= lastL; l++) {
        const double lat = getVgtLatitude(l);
        if(lat > synMaxLat || lat < synMinLat) {
            continue;
        }
        context.getLogging().info("Projecting vgt line " + lexical_cast<string>(l), getId());
        for(long m = vgtGrid.getFirstM(); m <= vgtGrid.getMaxM(); m++) {
            const double lon = getVgtLongitude(m);
            if(synMinLon > lon || synMaxLon < lon) {
                continue;
            }
            if(indicesFound) {
                findPixelPosAroundGivenIndices(lat, lon, synIndices);
            } else {
                findPixelPosInWholeGrid(lat, lon, synIndices);
            }
            const long synK = synIndices[0];
            const long synL = synIndices[1];
            const long synM = synIndices[2];
            if (synGrid.isValidPosition(synK, synL, synM)) {
                setValues(synK, synL, synM, l, m);
                indicesFound = true;
            } else {
                indicesFound = false;
                continue;
            }
        }
    }
    */
}

void Vpr::getMinMaxSynLat(double& minLat, double& maxLat) const {
	const Grid& geoGrid = geoSegment->getGrid();
	const Grid& synGrid = synSegment->getGrid();
	for (long k = synGrid.getFirstK(); k <= synGrid.getMaxK(); k++) {
		for (long l = synGrid.getFirstL(); l <= synGrid.getMaxL(); l++) {
			for (long m = synGrid.getFirstM(); m <= synGrid.getMaxM(); m++) {
				const size_t index = geoGrid.getIndex(k, l, m);
				const double lat = latAccessor->getDouble(index);
				if (lat < minLat) {
					minLat = lat;
				}
				if (lat > maxLat) {
					maxLat = lat;
				}
			}
		}
	}
}

void Vpr::minMaxSynLon(double& minLon, double& maxLon) const {
	const Grid& geoGrid = geoSegment->getGrid();
	const Grid& synGrid = synSegment->getGrid();
    for(long k = synGrid.getFirstK(); k <= synGrid.getMaxK(); k++) {
        for(long l = synGrid.getFirstL(); l <= synGrid.getMaxL(); l++) {
            for(long m = synGrid.getFirstM(); m <= synGrid.getMaxM(); m++) {
                const size_t index = geoGrid.getIndex(k, l , m);
                const double lon = lonAccessor->getDouble(index);
                if(lon < minLon) {
                    minLon = lon;
                }
                if(lon > maxLon) {
                    maxLon = lon;
                }
            }
        }
    }
}

void Vpr::getMinMaxVgtLat(long firstL, long lastL, double& minLat, double& maxLat) const {
    for(long l = firstL; l <= lastL; l++) {
        const double lat = getVgtLatitude(l);
        if(lat < minLat) {
            minLat = lat;
        }
        if(lat > maxLat) {
            maxLat = lat;
        }
    }
}

void Vpr::findPixelPosInWholeGrid(double lat, double lon, valarray<long>& synIndices) const {
	const Grid& geoGrid = geoSegment->getGrid();

    findPixelPos(lat, lon,
            geoGrid.getFirstK(), geoGrid.getMaxK(),
            geoGrid.getFirstL(), geoGrid.getMaxL(),
            geoGrid.getFirstM(), geoGrid.getMaxM(),
            synIndices);
}

void Vpr::findPixelPosAroundGivenIndices(double lat, double lon, valarray<long>& synIndices) const {
	const Grid& geoGrid = geoSegment->getGrid();
    const long k0 = max<long>(0, synIndices[0] - PIXEL_SEARCH_RADIUS / 2);
    const long kMax = min<long>(geoGrid.getMaxK(), synIndices[0] + PIXEL_SEARCH_RADIUS / 2);
    const long l0 = max<long>(0, synIndices[1] - PIXEL_SEARCH_RADIUS / 2);
    const long lMax = min<long>(geoGrid.getMaxL(), synIndices[1] + PIXEL_SEARCH_RADIUS / 2);
    const long m0 = max<long>(0, synIndices[2] - PIXEL_SEARCH_RADIUS / 2);
    const long mMax = min<long>(geoGrid.getMaxM(), synIndices[2] + PIXEL_SEARCH_RADIUS / 2);
    findPixelPos(lat, lon, k0, kMax, l0, lMax, m0, mMax, synIndices);
}

void Vpr::findPixelPos(double lat, double lon, long k0, long kMax, long l0, long lMax, long m0, long mMax, valarray<long>& synIndices) const {
	const Grid& geoGrid = geoSegment->getGrid();
    double synLat;
    double synLon;
    double delta = numeric_limits<double>::max();
    for (long k = k0; k <= kMax; k++) {
        for (long l = l0; l <= lMax; l++) {
            for (long m = m0; m <= mMax; m++) {
                const size_t index = geoGrid.getIndex(k, l, m);
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

void Vpr::setValues(long synK, long synL, long synM, long l, long m) {
    const Grid& collocatedGrid = synSegment->getGrid();
    const Grid& vgtGrid = vgpSegment->getGrid();
    const size_t collocatedIndex = collocatedGrid.getIndex(synK, synL, synM);
    const size_t vgtIndex = vgtGrid.getIndex(0, l, m);
    for(size_t i = 0; i < synReflectanceAccessors.size(); i++) {
        const double value = synReflectanceAccessors[i]->getDouble(collocatedIndex);
        vgtReflectanceAccessors[i]->setDouble(vgtIndex, value);
    }
    const uint8_t flags = synFlagsAccessor->getUByte(collocatedIndex);
    vgtFlagsAccessor->setUByte(vgtIndex, flags);
}

double Vpr::getVgtLatitude(long l) {
    if(l < 0 || l >= LINE_COUNT) {
        BOOST_THROW_EXCEPTION(std::invalid_argument("l < 0 || l >= LINE_COUNT, l = " + lexical_cast<string>(l)));
    }
    const double unnormalisedResult = l * PIXEL_SIZE;
    return 75 - unnormalisedResult;
}

double Vpr::getVgtLongitude(long m) {
    if(m < 0 || m >= COL_COUNT) {
        BOOST_THROW_EXCEPTION(std::invalid_argument("m < 0 || m >= COL_COUNT, m = " + lexical_cast<string>(m)));
    }
    const double unnormalisedResult = m * PIXEL_SIZE;
    return -180 + unnormalisedResult;
}

long Vpr::findLineOfSynSegmentNearestTo(double vgtMaxLat) const {
	const Grid& geoGrid = geoSegment->getGrid();
    long result;
    double delta = numeric_limits<double>::max();
    for (long k = geoGrid.getFirstK(); k <= geoGrid.getMaxK(); k++) {
        for (long l = geoGrid.getFirstL(); l <= geoGrid.getMaxL(); l++) {
            for (long m = geoGrid.getFirstM(); m <= geoGrid.getMaxM(); m++) {
                const size_t index = geoGrid.getIndex(k, l, m);
                double synLat = latAccessor->getDouble(index);
                double innerDelta = std::abs(vgtMaxLat - synLat);
                if(innerDelta < delta) {
                    delta = innerDelta;
                    result = l;
                }
            }
        }
    }
    return result;
}
