/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

#include "Vco.h"
#include "../util/TimeConverter.h"

using boost::lexical_cast;

Vco::Vco() : BasicModule("VCO") {
}

Vco::~Vco() {
}

void Vco::start(Context& context) {
    synSegment = &context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
    geoSegment = &context.getSegment(Constants::SEGMENT_GEO);

    latAccessor = &geoSegment->getAccessor("latitude");
    lonAccessor = &geoSegment->getAccessor("longitude");

	// TODO - predefined coordinates for Europe; read from job order file
	maxTargetLat = 75;
	minTargetLat = 25;
	maxTargetLon = 62;
	minTargetLon = -11;

	if (!context.hasSegment(Constants::SEGMENT_VGT)) {
		addTargetSegments(context);
		addTargetVariables(context);
	}
}

void Vco::addTargetSegments(Context& context) {
    const int latCellCount = maxTargetLat - minTargetLat;
    const int lonCellCount = maxTargetLon - minTargetLon;
    const int rowCount = latCellCount * TARGET_PIXELS_PER_DEGREE;
    const int colCount = lonCellCount * TARGET_PIXELS_PER_DEGREE;

    context.getLogging().info("Adding segment '" + Constants::SEGMENT_VGT_LAT + "' to context.", getId());
    context.addSingleLineSegment(Constants::SEGMENT_VGT_LAT, rowCount);

    context.getLogging().info("Adding segment '" + Constants::SEGMENT_VGT_LON + "' to context.", getId());
    context.addSingleLineSegment(Constants::SEGMENT_VGT_LON, colCount);

    context.getLogging().info("Adding segment '" + Constants::SEGMENT_VGT_LAT_BNDS + "' to context.", getId());
    context.addSingleLineSegment(Constants::SEGMENT_VGT_LAT_BNDS, rowCount * 2);

    context.getLogging().info("Adding segment '" + Constants::SEGMENT_VGT_LON_BNDS + "' to context.", getId());
    context.addSingleLineSegment(Constants::SEGMENT_VGT_LON_BNDS, colCount * 2);

    context.getLogging().info("Adding segment '" + Constants::SEGMENT_VGT + "' to context.", getId());
    context.addMapSegment(Constants::SEGMENT_VGT, rowCount, colCount);
}

void Vco::addTargetVariables(Context& context) {
    const ProductDescriptor& pd = context.getDictionary().getProductDescriptor(Constants::PRODUCT_VGS);

	context.getSegment(Constants::SEGMENT_VGT_LAT).addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGT_LAT).getVariableDescriptor("lat"));
	context.getSegment(Constants::SEGMENT_VGT_LON).addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGT_LON).getVariableDescriptor("lon"));
	context.getSegment(Constants::SEGMENT_VGT_LAT_BNDS).addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGT_LAT_BNDS).getVariableDescriptor("lat_bnds"));
	context.getSegment(Constants::SEGMENT_VGT_LON_BNDS).addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGT_LON_BNDS).getVariableDescriptor("lon_bnds"));

	Segment& vgtSegment = context.getSegment(Constants::SEGMENT_VGT);
	vgtSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGT).getVariableDescriptor("B0"));
    vgtSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGT).getVariableDescriptor("B2"));
    vgtSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGT).getVariableDescriptor("B3"));
    vgtSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGT).getVariableDescriptor("MIR"));
    vgtSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGT).getVariableDescriptor("NDVI"));
    vgtSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGT).getVariableDescriptor("SM"));

	vgtSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGT).getVariableDescriptor("TG"));
	vgtSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGT).getVariableDescriptor("SAA"));
	vgtSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGT).getVariableDescriptor("SZA"));
	vgtSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGT).getVariableDescriptor("VAA"));
	vgtSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGT).getVariableDescriptor("VZA"));
}

void Vco::process(Context& context) {
	using std::floor;
	using std::min;

	const Segment& syn = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	const Segment& vgt = context.getSegment(Constants::SEGMENT_VGT);

	setMapLats(context);
	setMapLons(context);
	setMapLatBounds(context);
	setMapLonBounds(context);

	const Grid& sourceGrid = syn.getGrid();
	const Grid& targetGrid = vgt.getGrid();

    valarray<Accessor*> sourceAccessors(11);
    valarray<Accessor*> targetAccessors(11);

	sourceAccessors[0] = &syn.getAccessor("B0");
    sourceAccessors[1] = &syn.getAccessor("B2");
    sourceAccessors[2] = &syn.getAccessor("B3");
    sourceAccessors[3] = &syn.getAccessor("MIR");
    sourceAccessors[4] = &syn.getAccessor("SM");
    sourceAccessors[5] = &syn.getAccessor("NDVI");
    sourceAccessors[6] = &syn.getAccessor("SAA");
    sourceAccessors[7] = &syn.getAccessor("SZA");
    sourceAccessors[8] = &syn.getAccessor("VAA");
    sourceAccessors[9] = &syn.getAccessor("VZA");
    sourceAccessors[10] = &context.getSegment(Constants::SEGMENT_OLC_TIME).getAccessor("time");

	targetAccessors[0] = &vgt.getAccessor("B0");
	targetAccessors[1] = &vgt.getAccessor("B2");
	targetAccessors[2] = &vgt.getAccessor("B3");
	targetAccessors[3] = &vgt.getAccessor("MIR");
	targetAccessors[4] = &vgt.getAccessor("SM");
	targetAccessors[5] = &vgt.getAccessor("NDVI");
	targetAccessors[6] = &vgt.getAccessor("SAA");
	targetAccessors[7] = &vgt.getAccessor("SZA");
	targetAccessors[8] = &vgt.getAccessor("VAA");
	targetAccessors[9] = &vgt.getAccessor("VZA");
	targetAccessors[10] = &vgt.getAccessor("TG");

	const long firstTargetL = context.getFirstComputableL(vgt, *this);
	context.getLogging().debug("Segment [" + vgt.toString() + "]: firstComputableL = " + lexical_cast<string>(firstTargetL), getId());
	long lastTargetL = context.getLastComputableL(vgt, *this);
	context.getLogging().debug("Segment [" + vgt.toString() + "]: lastComputableL = " + lexical_cast<string>(lastTargetL), getId());

    double minSourceLat = 90.0;
    double maxSourceLat = -90.0;
    double minTargetLat = 90.0;
    double maxTargetLat = -90.0;

    getMinMaxSourceLat(minSourceLat, maxSourceLat);
    getMinMaxTargetLat(minTargetLat, maxTargetLat, firstTargetL, lastTargetL);

    // Is the target region north of the source region, without overlap?
    if (minTargetLat - DEGREES_PER_TARGET_PIXEL * 0.5 > maxSourceLat) {
    	// Yes. Processing is completed.
		context.setLastComputedL(vgt, *this, lastTargetL);
		return;
	}

    // Is the target region south of the source region, without overlap?
    if (maxTargetLat + DEGREES_PER_TARGET_PIXEL * 0.5 < minSourceLat && context.getLastComputableL(syn, *this) < sourceGrid.getMaxL()) {
    	// Yes. Processing will be completed later.
    	return;
	}

	const long lastComputedSourceL = context.getLastComputableL(syn, *this);

	long sourceK = 0;
	long sourceL = 0;
	long sourceM = 0;
	long firstRequiredSourceL = 0;

	PixelFinder pixelFinder(*this, DEGREES_PER_TARGET_PIXEL);
	const TimeConverter tc(context.getJobOrder().getIpfConfiguration().getSensingTimeStart());

	for (long l = firstTargetL; l <= lastTargetL; l++) {
		context.getLogging().progress("Processing line l = " + lexical_cast<string>(l), getId());

		firstRequiredSourceL = sourceGrid.getMaxInMemoryL() + 1;

		for (long k = targetGrid.getMinK(); k <= targetGrid.getMaxK(); k++) {
			for (long m = targetGrid.getMinM(); m <= targetGrid.getMaxM(); m++) {
				const double targetLat = getTargetLat(l);
				const double targetLon = getTargetLon(m);
				const bool sourcePixelFound = pixelFinder.findSourcePixel(targetLat, targetLon, sourceK, sourceL, sourceM);

				// 1. Is there a source pixel for the target pixel?
				if (!sourcePixelFound) {
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

				const size_t sourceIndex = sourceGrid.getIndex(sourceK, sourceL, sourceM);
				const size_t targetIndex = targetGrid.getIndex(k, l, m);
				const double sourceNdvi = sourceAccessors[5]->getDouble(sourceIndex);
				const double targetNdvi = targetAccessors[5]->getDouble(targetIndex);

				if (!sourceAccessors[5]->isFillValue(sourceIndex) && (sourceNdvi > targetNdvi || targetAccessors[5]->isFillValue(targetIndex))) {
					// 4. Set the samples of the target pixel
					for (size_t i = 0; i < targetAccessors.size() - 1; i++) {
						Accessor* sourceAccessor = sourceAccessors[i];
						Accessor* targetAccessor = targetAccessors[i];

						if (!sourceAccessor->isFillValue(sourceIndex)) {
							try {
								setValue(sourceAccessor, targetAccessor, sourceIndex, targetIndex);
							} catch (std::exception& e) {
								context.getLogging().error("sourceAccessorIndex = " + boost::lexical_cast<string>(i), getId());
								context.getLogging().error("sourceAccessorValue = " + boost::lexical_cast<string>(sourceAccessor->getDouble(i)), getId());
								throw e;
							}
						}
					}
					const int64_t sourceTime = sourceAccessors[10]->getLong(sourceL);
					const int16_t targetTime = tc.getMinutesSinceStartTime(sourceTime);
					targetAccessors[10]->setShort(targetIndex, targetTime);
				}
			}
		}
	}

	context.setFirstRequiredL(syn, *this, firstRequiredSourceL);
	context.setLastComputedL(vgt, *this, lastTargetL);
}

void Vco::getMinMaxSourceLat(double& minLat, double& maxLat) const {
	const Grid& geoGrid = geoSegment->getGrid();
	const Grid& synGrid = synSegment->getGrid();

	for (long k = synGrid.getMinK(); k <= synGrid.getMaxK(); k++) {
		for (long l = synGrid.getMinInMemoryL(); l <= synGrid.getMaxInMemoryL(); l++) {
			for (long m = synGrid.getMinM(); m <= synGrid.getMaxM(); m++) {
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

void Vco::getMinMaxTargetLat(double& minLat, double& maxLat, long firstL, long lastL) const {
	maxLat = getTargetLat(firstL);
	minLat = getTargetLat(lastL);
}

double Vco::getTargetLat(long l) const {
    return maxTargetLat - l * DEGREES_PER_TARGET_PIXEL;
}

double Vco::getTargetLon(long m) const {
    return minTargetLon + m * DEGREES_PER_TARGET_PIXEL;
}

void Vco::setValue(Accessor* sourceAccessor, Accessor* targetAccessor, size_t sourceIndex, size_t targetIndex) const {
	switch (sourceAccessor->getType()) {
	case Constants::TYPE_BYTE: {
		targetAccessor->setByte(targetIndex, sourceAccessor->getByte(sourceIndex));
		break;
	}
	case Constants::TYPE_UBYTE: {
		targetAccessor->setUByte(targetIndex, sourceAccessor->getUByte(sourceIndex));
		break;
	}
	case Constants::TYPE_SHORT: {
		targetAccessor->setShort(targetIndex, sourceAccessor->getShort(sourceIndex));
		break;
	}
	case Constants::TYPE_USHORT: {
		targetAccessor->setUShort(targetIndex, sourceAccessor->getUShort(sourceIndex));
		break;
	}
	case Constants::TYPE_INT: {
		targetAccessor->setInt(targetIndex, sourceAccessor->getInt(sourceIndex));
		break;
	}
	case Constants::TYPE_UINT: {
		targetAccessor->setUInt(targetIndex, sourceAccessor->getUInt(sourceIndex));
		break;
	}
	case Constants::TYPE_LONG: {
		targetAccessor->setLong(targetIndex, sourceAccessor->getLong(sourceIndex));
		break;
	}
	case Constants::TYPE_ULONG: {
		targetAccessor->setULong(targetIndex, sourceAccessor->getULong(sourceIndex));
		break;
	}
	case Constants::TYPE_FLOAT: {
		targetAccessor->setFloat(targetIndex, sourceAccessor->getFloat(sourceIndex));
		break;
	}
	case Constants::TYPE_DOUBLE: {
		targetAccessor->setDouble(targetIndex, sourceAccessor->getDouble(sourceIndex));
		break;
	}
	default:
	    BOOST_THROW_EXCEPTION(runtime_error("Unsupported variable type."));
	    break;
	}
}

void Vco::setMapLats(Context& context) const {
	const Segment& s = context.getSegment(Constants::SEGMENT_VGT_LAT);
	const Grid& g = s.getGrid();

	Accessor& a = s.getAccessor("lat");

	for (int m = 0; m < g.getSizeM(); m++) {
		a.setDouble(g.getIndex(0, 0, m), getTargetLat(m));
	}
}

void Vco::setMapLons(Context& context) const {
	const Segment& s = context.getSegment(Constants::SEGMENT_VGT_LON);
	const Grid& g = s.getGrid();

	Accessor& a = s.getAccessor("lon");

	for (int m = 0; m < g.getSizeM(); m++) {
		a.setDouble(g.getIndex(0, 0, m), getTargetLon(m));
	}
}

void Vco::setMapLatBounds(Context& context) const {
	const Segment& s = context.getSegment(Constants::SEGMENT_VGT_LAT_BNDS);
	const Grid& g = s.getGrid();

	Accessor& a = s.getAccessor("lat_bnds");

	for (int m = 0; m < g.getSizeM(); m += 2) {
		const size_t index = g.getIndex(0, 0, m);
		const double lat = getTargetLat(m / 2);

		a.setDouble(index + 0, lat - DEGREES_PER_TARGET_PIXEL * 0.5);
		a.setDouble(index + 1, lat + DEGREES_PER_TARGET_PIXEL * 0.5);
	}
}

void Vco::setMapLonBounds(Context& context) const {
	const Segment& s = context.getSegment(Constants::SEGMENT_VGT_LON_BNDS);
	const Grid& g = s.getGrid();

	Accessor& a = s.getAccessor("lon_bnds");

	for (int m = 0; m < g.getSizeM(); m += 2) {
		const size_t index = g.getIndex(0, 0, m);
		const double lon = getTargetLon(m / 2);

		a.setDouble(index + 0, lon - DEGREES_PER_TARGET_PIXEL * 0.5);
		a.setDouble(index + 1, lon + DEGREES_PER_TARGET_PIXEL * 0.5);
	}
}
