/*
 * Vpr.cpp
 *
 *  Created on: Dec 01, 2011
 *      Author: thomasstorm
 */

#include "Vpr.h"

Vpr::Vpr() : BasicModule("VPR"),
        synReflectanceAccessors(4),
        vgtReflectanceAccessors(4) {
}

Vpr::~Vpr() {
}

void Vpr::start(Context& context) {
    synSegment = &context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
    geoSegment = &context.getSegment(Constants::SEGMENT_GEO);

    latAccessor = &geoSegment->getAccessor("latitude");
    lonAccessor = &geoSegment->getAccessor("longitude");

    context.getLogging().info("Adding segment '" + Constants::SEGMENT_VGP + "' to context.", getId());
    vgpSegment = &context.addMapSegment(Constants::SEGMENT_VGP, LINE_COUNT, COL_COUNT);

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
	using std::floor;
	using std::min;

	const Segment& s = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	const Segment& t = context.getSegment(Constants::SEGMENT_VGP);
	const Grid& sourceGrid = s.getGrid();
	const Grid& targetGrid = t.getGrid();

	const long firstTargetL = context.getFirstComputableL(t, *this);
	context.getLogging().debug("Segment [" + t.toString() + "]: firstComputableL = " + lexical_cast<string>(firstTargetL), getId());
	long lastTargetL = context.getLastComputableL(t, *this);
	context.getLogging().debug("Segment [" + t.toString() + "]: lastComputableL = " + lexical_cast<string>(lastTargetL), getId());

    double minSourceLat = 90.0;
    double maxSourceLat = -90.0;
    double minTargetLat = 90.0;
    double maxTargetLat = -90.0;
    double maxSourceLon = -180.0;
    getMinMaxSourceLat(minSourceLat, maxSourceLat);
    getMinMaxTargetLat(minTargetLat, maxTargetLat, firstTargetL, lastTargetL);

    double minSourceLon = 180.0;
    getMinMaxSourceLon(minSourceLon, maxSourceLon);

    // Is the target region north of the source region, without overlap?
    if (minTargetLat > maxSourceLat) {
    	// Yes. Processing is completed.
		context.setLastComputedL(t, *this, lastTargetL);
		return;
	}

    // Is the target region south of the source region, without overlap?
    if (maxTargetLat < minSourceLat && context.getLastComputableL(s, *this) < sourceGrid.getMaxL()) {
    	// Yes. Processing will be completed later.
    	return;
	}

	const long lastComputedSourceL = context.getLastComputableL(s, *this);
	long firstRequiredSourceL = sourceGrid.getLastL() + 1;

	long sourceK = 0;
	long sourceL = 0;
	long sourceM = 0;

	PixelFinder pixelFinder(*this, TARGET_PIXEL_SIZE);

	for (long l = firstTargetL; l <= lastTargetL; l++) {
		context.getLogging().progress("Processing line l = " + lexical_cast<string>(l), getId());

		const double targetLat = getTargetLat(l);
		if (targetLat > maxSourceLat) {
			continue;
		}
		if (targetLat < minSourceLat) {
			continue;
		}

		for (long k = targetGrid.getFirstK(); k < targetGrid.getFirstK() + targetGrid.getSizeK(); k++) {
			for (long m = targetGrid.getFirstM(); m < targetGrid.getFirstM() + targetGrid.getSizeM(); m++) {
				const size_t targetIndex = targetGrid.getIndex(k, l, m);

				const double targetLon = getTargetLon(m);
				if (targetLon < minSourceLon || targetLon > maxSourceLon) {
					continue;
				}

				const bool sourcePixelFound = pixelFinder.findSourcePixel(targetLat, targetLon, sourceK, sourceL, sourceM);
				// 1. Is there a source pixel for the target pixel?
				if (!sourcePixelFound) {
					// No.
					continue;
				}

				// 2. Update first required sourceL
				firstRequiredSourceL = min(sourceL, firstRequiredSourceL);

				// 3. Is the current source line beyond the last computed source line?
				if (sourceL > lastComputedSourceL) {
					// Yes.
					lastTargetL = min(l - 1, lastTargetL);
					continue;
				}

				Accessor& sourceAccessor = *(synReflectanceAccessors[0]);
				Accessor& targetAccessor = *(vgtReflectanceAccessors[0]);

				const size_t sourceIndex = sourceGrid.getIndex(sourceK, sourceL, sourceM);
				if (sourceAccessor.isFillValue(sourceIndex)) {
					continue;
				}

				switch (sourceAccessor.getType()) {
				case Constants::TYPE_BYTE: {
					targetAccessor.setByte(targetIndex, sourceAccessor.getByte(sourceIndex));
					break;
				}
				case Constants::TYPE_UBYTE: {
					targetAccessor.setUByte(targetIndex, sourceAccessor.getUByte(sourceIndex));
					break;
				}
				case Constants::TYPE_SHORT: {
					targetAccessor.setShort(targetIndex, sourceAccessor.getShort(sourceIndex));
					break;
				}
				case Constants::TYPE_USHORT: {
					targetAccessor.setUShort(targetIndex, sourceAccessor.getUShort(sourceIndex));
					break;
				}
				case Constants::TYPE_INT: {
					targetAccessor.setInt(targetIndex, sourceAccessor.getInt(sourceIndex));
					break;
				}
				case Constants::TYPE_UINT: {
					targetAccessor.setUInt(targetIndex, sourceAccessor.getUInt(sourceIndex));
					break;
				}
				case Constants::TYPE_LONG: {
					targetAccessor.setLong(targetIndex, sourceAccessor.getLong(sourceIndex));
					break;
				}
				case Constants::TYPE_ULONG: {
					targetAccessor.setULong(targetIndex, sourceAccessor.getULong(sourceIndex));
					break;
				}
				case Constants::TYPE_FLOAT: {
					targetAccessor.setFloat(targetIndex, sourceAccessor.getFloat(sourceIndex));
					break;
				}
				case Constants::TYPE_DOUBLE: {
					targetAccessor.setDouble(targetIndex, sourceAccessor.getDouble(sourceIndex));
					break;
				}
				default:
					break;
				}
			}
		}
	}

	context.setFirstRequiredL(s, *this, firstRequiredSourceL);
	context.setLastComputedL(t, *this, lastTargetL);
}

void Vpr::process2(Context& context) {
    const long firstL = context.getFirstComputableL(*vgpSegment, *this);
    const long lastL = context.getLastComputableL(*vgpSegment, *this);

    double synMinLat = 90.0;
    double synMaxLat = -90.0;
    getMinMaxSourceLat(synMinLat, synMaxLat);
    double vgtMinLat = 90.0;
    double vgtMaxLat = -90.0;
    //getMinMaxTargetLat(firstL, lastL, vgtMinLat, vgtMaxLat);

    /*
     * if the minimum latitude of the current VGT segment is greater than the maximum latitude
     * of the current SYN segment, move the VGT segment forward and do not allow moving
     * of the SYN segment.
     */
    if (synMaxLat < vgtMinLat && context.getLastComputableL(*vgpSegment, *this) < vgpSegment->getGrid().getMaxL()) {
		context.setLastComputedL(*vgpSegment, *this, lastL);
		context.setFirstRequiredL(*synSegment, *this, synSegment->getGrid().getFirstL());
		// TODO - do we need this?
		//context.setFirstRequiredL(*olcSegment, *this, olcSegment->getGrid().getFirstL());
		//context.setFirstRequiredL(*slnSegment, *this, slnSegment->getGrid().getFirstL());
		//context.setFirstRequiredL(*sloSegment, *this, sloSegment->getGrid().getFirstL());
		return;
	}

    /*
     * if the maximum latitude of the current VGT segment is less than the minimum latitude
     * of the current SYN segment, do not allow moving of the VGT segment and move
     * the SYN segment forward, if possible.
     */
    if (vgtMaxLat < synMinLat && context.getLastComputableL(*synSegment, *this) < synSegment->getGrid().getMaxL()) {
		return;
	}

    /*
     * if the minimum latitude of the current VGT segment is less than the maximum latitude
     * of the current SYN segment AND the minimum VGT latitude is greater than the minumum
     * SYN latitude (i.e. the VGT segment and the SYN segment overlap), perform the projection
     * and set the first required line of the SYN segment to the line of the SYN segment nearest
     * to vgtMaxLat.
     */
	// TODO - do we need this?
	//context.setFirstRequiredL(*olcSegment, *this, -1);
	//context.setFirstRequiredL(*slnSegment, *this, -1);
	//context.setFirstRequiredL(*sloSegment, *this, -1);

    double synMinLon = 180.0;
	double synMaxLon = -180.0;
	getMinMaxSourceLon(synMinLon, synMaxLon);

	const Grid& vgpGrid = vgpSegment->getGrid();
	const Grid& synGrid = synSegment->getGrid();
	valarray<long> synIndices(3);
	bool indicesFound = false;
	for (long l = firstL; l <= lastL; l++) {
		const double lat = getTargetLat(l);
		if (lat > synMaxLat) {
			continue;
		}
		if (lat < synMinLat) {
			continue;
		}
		context.getLogging().info("Projecting vgt line " + lexical_cast<string>(l), getId());
		for (long m = vgpGrid.getFirstM(); m <= vgpGrid.getMaxM(); m++) {
			const double lon = getTargetLon(m);
			if (synMinLon > lon || synMaxLon < lon) {
				continue;
			}
			if (indicesFound) {
				//findPixelPosAroundGivenIndices(lat, lon, synIndices);
			} else {
				//findPixelPosInWholeGrid(lat, lon, synIndices);
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
	context.setLastComputedL(*vgpSegment, *this, lastL);
}

void Vpr::getMinMaxSourceLat(double& minLat, double& maxLat) const {
	const Grid& geoGrid = geoSegment->getGrid();
	const Grid& synGrid = synSegment->getGrid();

	for (long k = synGrid.getFirstK(); k <= synGrid.getMaxK(); k++) {
		for (long l = synGrid.getFirstL(); l <= synGrid.getMaxL(); l++) {
			for (long m = synGrid.getFirstM(); m <= synGrid.getMaxM(); m++) {
				const size_t index = geoGrid.getIndex(k, l, m);
				if (!latAccessor->isFillValue(index)) {
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
}

void Vpr::getMinMaxSourceLon(double& minLon, double& maxLon) const {
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

void Vpr::getMinMaxTargetLat(double& minLat, double& maxLat, long firstL, long lastL) const {
	maxLat = getTargetLat(firstL);
	minLat = getTargetLat(lastL);
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

double Vpr::getTargetLat(long l) {
    return 75.0 - l * TARGET_PIXEL_SIZE;
}

double Vpr::getTargetLon(long m) {
    return m * TARGET_PIXEL_SIZE - 180.0;
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
