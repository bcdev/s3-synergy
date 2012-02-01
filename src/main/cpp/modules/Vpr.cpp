/*
 * Vpr.cpp
 *
 *  Created on: Dec 01, 2011
 *      Author: thomasstorm
 */

#include "Vpr.h"

Vpr::Vpr() : BasicModule("VPR") {
}

Vpr::~Vpr() {
}

void Vpr::start(Context& context) {
    synSegment = &context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
    geoSegment = &context.getSegment(Constants::SEGMENT_GEO);

    latAccessor = &geoSegment->getAccessor("latitude");
    lonAccessor = &geoSegment->getAccessor("longitude");

	// TODO - predefined coordinates for Europe; read from job order file
	maxTargetLat = 75;
	minTargetLat = 25;
	maxTargetLon = 62;
	minTargetLon = -11;

	addTargetSegments(context);
    addTargetVariables(context);
}

void Vpr::addTargetSegments(Context& context) {
    const int latCellCount = maxTargetLat - minTargetLat;
    const int lonCellCount = maxTargetLon - minTargetLon;
    const int rowCount = latCellCount * TARGET_PIXELS_PER_DEGREE;
    const int colCount = lonCellCount * TARGET_PIXELS_PER_DEGREE;

    context.getLogging().info("Adding segment '" + Constants::SEGMENT_VGP_LAT + "' to context.", getId());
    context.addSingleLineSegment(Constants::SEGMENT_VGP_LAT, rowCount);

    context.getLogging().info("Adding segment '" + Constants::SEGMENT_VGP_LON + "' to context.", getId());
    context.addSingleLineSegment(Constants::SEGMENT_VGP_LON, colCount);

    context.getLogging().info("Adding segment '" + Constants::SEGMENT_VGP_LAT_BNDS + "' to context.", getId());
    context.addSingleLineSegment(Constants::SEGMENT_VGP_LAT_BNDS, rowCount * 2);

    context.getLogging().info("Adding segment '" + Constants::SEGMENT_VGP_LON_BNDS + "' to context.", getId());
    context.addSingleLineSegment(Constants::SEGMENT_VGP_LON_BNDS, colCount * 2);

    context.getLogging().info("Adding segment '" + Constants::SEGMENT_VGP + "' to context.", getId());
    context.addMapSegment(Constants::SEGMENT_VGP, rowCount, colCount);

    const int subsampledRowCount = latCellCount * SUBSAMPLED_TARGET_PIXELS_PER_DEGREE;
    const int subsampledColCount = lonCellCount * SUBSAMPLED_TARGET_PIXELS_PER_DEGREE;

    context.getLogging().info("Adding segment '" + Constants::SEGMENT_VGP_LAT_TP + "' to context.", getId());
    context.addSingleLineSegment(Constants::SEGMENT_VGP_LAT_TP, subsampledRowCount);

    context.getLogging().info("Adding segment '" + Constants::SEGMENT_VGP_LON_TP + "' to context.", getId());
    context.addSingleLineSegment(Constants::SEGMENT_VGP_LON_TP, subsampledColCount);

    context.getLogging().info("Adding segment '" + Constants::SEGMENT_VGP_LAT_TP_BNDS + "' to context.", getId());
    context.addSingleLineSegment(Constants::SEGMENT_VGP_LAT_TP_BNDS, subsampledRowCount * 2);

    context.getLogging().info("Adding segment '" + Constants::SEGMENT_VGP_LON_TP_BNDS + "' to context.", getId());
    context.addSingleLineSegment(Constants::SEGMENT_VGP_LON_TP_BNDS, subsampledColCount * 2);

    context.getLogging().info("Adding segment '" + Constants::SEGMENT_VGP_TP + "' to context.", getId());
    context.addMapSegment(Constants::SEGMENT_VGP_TP, subsampledRowCount, subsampledColCount);
}

void Vpr::addTargetVariables(Context& context) {
    const ProductDescriptor& pd = context.getDictionary().getProductDescriptor(Constants::PRODUCT_VGP);

	context.getSegment(Constants::SEGMENT_VGP_LAT).addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP_LAT).getVariableDescriptor("lat"));
	context.getSegment(Constants::SEGMENT_VGP_LON).addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP_LON).getVariableDescriptor("lon"));
	context.getSegment(Constants::SEGMENT_VGP_LAT_BNDS).addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP_LAT_BNDS).getVariableDescriptor("lat_bnds"));
	context.getSegment(Constants::SEGMENT_VGP_LON_BNDS).addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP_LON_BNDS).getVariableDescriptor("lon_bnds"));

	Segment& vgtSegment = context.getSegment(Constants::SEGMENT_VGP);
	vgtSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP).getVariableDescriptor("B0"));
    vgtSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP).getVariableDescriptor("B2"));
    vgtSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP).getVariableDescriptor("B3"));
    vgtSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP).getVariableDescriptor("MIR"));
    vgtSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP).getVariableDescriptor("SM"));

	context.getSegment(Constants::SEGMENT_VGP_LAT_TP).addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP_LAT_TP).getVariableDescriptor("lat"));
	context.getSegment(Constants::SEGMENT_VGP_LON_TP).addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP_LON_TP).getVariableDescriptor("lon"));
	context.getSegment(Constants::SEGMENT_VGP_LAT_TP_BNDS).addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP_LAT_TP_BNDS).getVariableDescriptor("lat_bnds"));
	context.getSegment(Constants::SEGMENT_VGP_LON_TP_BNDS).addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP_LON_TP_BNDS).getVariableDescriptor("lon_bnds"));

	Segment& vgtTiePointSegment = context.getSegment(Constants::SEGMENT_VGP_TP);
	vgtTiePointSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP_TP).getVariableDescriptor("AG"));
	vgtTiePointSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP_TP).getVariableDescriptor("OG"));
	vgtTiePointSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP_TP).getVariableDescriptor("SAA"));
	vgtTiePointSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP_TP).getVariableDescriptor("SZA"));
	vgtTiePointSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP_TP).getVariableDescriptor("VAA"));
	vgtTiePointSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP_TP).getVariableDescriptor("VZA"));
	vgtTiePointSegment.addVariable(pd.getSegmentDescriptor(Constants::SEGMENT_VGP_TP).getVariableDescriptor("WVG"));
}

void Vpr::process(Context& context) {
	using std::floor;
	using std::min;

	const Segment& syn = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	const Segment& vgp = context.getSegment(Constants::SEGMENT_VGP);
	const Segment& vgpTiePoint = context.getSegment(Constants::SEGMENT_VGP_TP);

	setMapLats(context);
	setMapLats(context);
	setMapLatBounds(context);
	setMapLonBounds(context);
	setTpLats(context);
	setTpLons(context);
	setTpLatBounds(context);
	setTpLonBounds(context);

	const Grid& sourceGrid = syn.getGrid();
	const Grid& targetGrid = vgp.getGrid();
	const Grid& subsampledTargetGrid = vgpTiePoint.getGrid();

    valarray<Accessor*> sourceAccessors(12);
    valarray<Accessor*> targetAccessors(12);

	sourceAccessors[0] = &syn.getAccessor("B0");
    sourceAccessors[1] = &syn.getAccessor("B2");
    sourceAccessors[2] = &syn.getAccessor("B3");
    sourceAccessors[3] = &syn.getAccessor("MIR");
    sourceAccessors[4] = &syn.getAccessor("SM");
    sourceAccessors[5] = &syn.getAccessor("AG");
    sourceAccessors[6] = &syn.getAccessor("OG");
    sourceAccessors[7] = &syn.getAccessor("WVG");
    sourceAccessors[8] = &syn.getAccessor("SAA");
    sourceAccessors[9] = &syn.getAccessor("SZA");
    sourceAccessors[10] = &syn.getAccessor("VAA");
    sourceAccessors[11] = &syn.getAccessor("VZA");

	targetAccessors[0] = &vgp.getAccessor("B0");
	targetAccessors[1] = &vgp.getAccessor("B2");
	targetAccessors[2] = &vgp.getAccessor("B3");
	targetAccessors[3] = &vgp.getAccessor("MIR");
	targetAccessors[4] = &vgp.getAccessor("SM");
	targetAccessors[5] = &vgpTiePoint.getAccessor("AG");
	targetAccessors[6] = &vgpTiePoint.getAccessor("OG");
	targetAccessors[7] = &vgpTiePoint.getAccessor("WVG");
	targetAccessors[8] = &vgpTiePoint.getAccessor("SAA");
	targetAccessors[9] = &vgpTiePoint.getAccessor("SZA");
	targetAccessors[10] = &vgpTiePoint.getAccessor("VAA");
	targetAccessors[11] = &vgpTiePoint.getAccessor("VZA");

	const long firstTargetL = context.getFirstComputableL(vgp, *this);
	context.getLogging().debug("Segment [" + vgp.toString() + "]: firstComputableL = " + lexical_cast<string>(firstTargetL), getId());
	long lastTargetL = context.getLastComputableL(vgp, *this);
	context.getLogging().debug("Segment [" + vgp.toString() + "]: lastComputableL = " + lexical_cast<string>(lastTargetL), getId());

    double minSourceLat = 90.0;
    double maxSourceLat = -90.0;
    double minTargetLat = 90.0;
    double maxTargetLat = -90.0;

    getMinMaxSourceLat(minSourceLat, maxSourceLat);
    getMinMaxTargetLat(minTargetLat, maxTargetLat, firstTargetL, lastTargetL);

    // Is the target region north of the source region, without overlap?
    if (minTargetLat - DEGREES_PER_TARGET_PIXEL * 0.5 > maxSourceLat) {
    	// Yes. Processing is completed.
		context.setLastComputedL(vgp, *this, lastTargetL);
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

	for (long l = firstTargetL; l <= lastTargetL; l++) {
		context.getLogging().progress("Processing line l = " + lexical_cast<string>(l), getId());

		firstRequiredSourceL = sourceGrid.getLastL() + 1;

		for (long k = targetGrid.getFirstK(); k < targetGrid.getFirstK() + targetGrid.getSizeK(); k++) {
			for (long m = targetGrid.getFirstM(); m < targetGrid.getFirstM() + targetGrid.getSizeM(); m++) {
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

				// 4. Set the samples of the target pixel
				for (size_t i = 0; i < 5; i++) {
					Accessor* sourceAccessor = sourceAccessors[i];
					Accessor* targetAccessor = targetAccessors[i];

					if (!sourceAccessor->isFillValue(sourceIndex)) {
						setValue(sourceAccessor, targetAccessor, sourceIndex, targetGrid.getIndex(k, l, m));
					}
				}
				// 5. Is the target pixel in the sub-sampled grid?
				if (l % 8 == 0 && m % 8 == 0) {
					// Yes, set the samples of the sub-sampled target pixel
					for (size_t i = 5; i < targetAccessors.size(); i++) {
						const size_t targetIndex = subsampledTargetGrid.getIndex(k, l / 8, m / 8);

						Accessor* sourceAccessor = sourceAccessors[i];
						Accessor* targetAccessor = targetAccessors[i];

						if (!sourceAccessor->isFillValue(sourceIndex)) {
							setValue(sourceAccessor, targetAccessor, sourceIndex, targetIndex);
						} else {
							targetAccessor->setFillValue(targetIndex);
						}
					}
				}
			}
		}
	}

	context.setFirstRequiredL(syn, *this, firstRequiredSourceL);
	context.setLastComputedL(vgp, *this, lastTargetL);
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

void Vpr::getMinMaxTargetLat(double& minLat, double& maxLat, long firstL, long lastL) const {
	maxLat = getTargetLat(firstL);
	minLat = getTargetLat(lastL);
}

double Vpr::getTargetLat(long l) const {
    return maxTargetLat - l * DEGREES_PER_TARGET_PIXEL;
}

double Vpr::getTargetLon(long m) const {
    return minTargetLon + m * DEGREES_PER_TARGET_PIXEL;
}

double Vpr::getSubsampledTargetLat(long l) const {
    return maxTargetLat - l * DEGREES_PER_SUBSAMPLED_TARGET_PIXEL;
}

double Vpr::getSubsampledTargetLon(long m) const {
    return minTargetLon + m * DEGREES_PER_SUBSAMPLED_TARGET_PIXEL;
}

void Vpr::setValue(Accessor* sourceAccessor, Accessor* targetAccessor, size_t sourceIndex, size_t targetIndex) const {
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

void Vpr::setMapLats(Context& context) const {
	const Segment& s = context.getSegment(Constants::SEGMENT_VGP_LAT);
	const Grid& g = s.getGrid();

	Accessor& a = s.getAccessor("lat");

	for (int m = 0; m < g.getSizeM(); m++) {
		a.setDouble(g.getIndex(0, 0, m), getTargetLat(m));
	}
}

void Vpr::setMapLons(Context& context) const {
	const Segment& s = context.getSegment(Constants::SEGMENT_VGP_LON);
	const Grid& g = s.getGrid();

	Accessor& a = s.getAccessor("lon");

	for (int m = 0; m < g.getSizeM(); m++) {
		a.setDouble(g.getIndex(0, 0, m), getTargetLon(m));
	}
}

void Vpr::setMapLatBounds(Context& context) const {
	const Segment& s = context.getSegment(Constants::SEGMENT_VGP_LAT_BNDS);
	const Grid& g = s.getGrid();

	Accessor& a = s.getAccessor("lat_bnds");

	for (int m = 0; m < g.getSizeM(); m += 2) {
		const size_t index = g.getIndex(0, 0, m);
		const double lat = getTargetLat(m / 2);

		a.setDouble(index + 0, lat - DEGREES_PER_TARGET_PIXEL * 0.5);
		a.setDouble(index + 1, lat + DEGREES_PER_TARGET_PIXEL * 0.5);
	}
}

void Vpr::setMapLonBounds(Context& context) const {
	const Segment& s = context.getSegment(Constants::SEGMENT_VGP_LON_BNDS);
	const Grid& g = s.getGrid();

	Accessor& a = s.getAccessor("lon_bnds");

	for (int m = 0; m < g.getSizeM(); m += 2) {
		const size_t index = g.getIndex(0, 0, m);
		const double lon = getTargetLon(m / 2);

		a.setDouble(index + 0, lon - DEGREES_PER_TARGET_PIXEL * 0.5);
		a.setDouble(index + 1, lon + DEGREES_PER_TARGET_PIXEL * 0.5);
	}
}

void Vpr::setTpLats(Context& context) const {
	const Segment& s = context.getSegment(Constants::SEGMENT_VGP_LAT_TP);
	const Grid& g = s.getGrid();

	Accessor& a = s.getAccessor("lat");

	for (int m = 0; m < g.getSizeM(); m++) {
		a.setDouble(g.getIndex(0, 0, m), getSubsampledTargetLat(m));
	}
}

void Vpr::setTpLons(Context& context) const {
	const Segment& s = context.getSegment(Constants::SEGMENT_VGP_LON_TP);
	const Grid& g = s.getGrid();

	Accessor& a = s.getAccessor("lon");

	for (int m = 0; m < g.getSizeM(); m++) {
		a.setDouble(g.getIndex(0, 0, m), getSubsampledTargetLon(m));
	}
}

void Vpr::setTpLatBounds(Context& context) const {
	const Segment& s = context.getSegment(Constants::SEGMENT_VGP_LAT_TP_BNDS);
	const Grid& g = s.getGrid();

	Accessor& a = s.getAccessor("lat_bnds");

	for (int m = 0; m < g.getSizeM(); m += 2) {
		const size_t index = g.getIndex(0, 0, m);
		const double lat = getTargetLat(m / 2);

		a.setDouble(index + 0, lat - DEGREES_PER_SUBSAMPLED_TARGET_PIXEL * 0.5);
		a.setDouble(index + 1, lat + DEGREES_PER_SUBSAMPLED_TARGET_PIXEL * 0.5);
	}
}

void Vpr::setTpLonBounds(Context& context) const {
	const Segment& s = context.getSegment(Constants::SEGMENT_VGP_LON_TP_BNDS);
	const Grid& g = s.getGrid();

	Accessor& a = s.getAccessor("lon_bnds");

	for (int m = 0; m < g.getSizeM(); m += 2) {
		const size_t index = g.getIndex(0, 0, m);
		const double lon = getTargetLon(m / 2);

		a.setDouble(index + 0, lon - DEGREES_PER_SUBSAMPLED_TARGET_PIXEL * 0.5);
		a.setDouble(index + 1, lon + DEGREES_PER_SUBSAMPLED_TARGET_PIXEL * 0.5);
	}
}


