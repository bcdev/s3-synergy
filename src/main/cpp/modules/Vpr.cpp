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
	return;

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

double Vpr::getTargetLat(long l) {
    return 75.0 - l * TARGET_PIXEL_SIZE;
}

double Vpr::getTargetLon(long m) {
    return m * TARGET_PIXEL_SIZE - 180.0;
}
