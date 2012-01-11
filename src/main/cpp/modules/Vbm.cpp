/*
 * Vbm.cpp
 *
 *  Created on: Nov 17, 2011
 *      Author: thomasstorm
 */

#include <algorithm>
#include <cmath>

#include "Aco.h"
#include "Vbm.h"

using std::abs;
using std::cos;
using std::fill;

Vbm::Vbm() :
        BasicModule("VBM"),
        hypSpectralResponses(VGT_CHANNEL_COUNT),
        synRadianceAccessors(SYN_CHANNEL_COUNT),
        synSolarIrradianceAccessors(SYN_CHANNEL_COUNT),
        vgtReflectanceAccessors(VGT_CHANNEL_COUNT),
        rho(HYP_CHANNEL_COUNT),
        ratm(HYP_CHANNEL_COUNT),
        ts(HYP_CHANNEL_COUNT),
        tv(HYP_CHANNEL_COUNT) {
}

Vbm::~Vbm() {
}

void Vbm::start(Context& context) {
    addVariables(context);

    prepareAccessors(context);
    prepareAuxdata(context);
}

void Vbm::addVariables(Context& context) {
    context.getLogging().info("Adding variables to segment", getId());

    Segment& collocatedSegment = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);

	vgtReflectanceAccessors[0] = &collocatedSegment.addVariable("B0", Constants::TYPE_DOUBLE);
	vgtReflectanceAccessors[1] = &collocatedSegment.addVariable("B2", Constants::TYPE_DOUBLE);
	vgtReflectanceAccessors[2] = &collocatedSegment.addVariable("B3", Constants::TYPE_DOUBLE);
	vgtReflectanceAccessors[3] = &collocatedSegment.addVariable("MIR", Constants::TYPE_DOUBLE);
    vgtFlagsAccessor = &collocatedSegment.addVariable("SM", Constants::TYPE_UBYTE);
}

void Vbm::prepareAccessors(Context& context) {
	const Segment& geoSegment = context.getSegment(Constants::SEGMENT_GEO);
	const Segment& collocatedSegment = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);

	latAccessor = &geoSegment.getAccessor("latitude");
	lonAccessor = &geoSegment.getAccessor("longitude");
	for (size_t i = 0; i < 18; i++) {
		const string index = lexical_cast<string>(i + 1);
		synRadianceAccessors[i] = &collocatedSegment.getAccessor("L_" + index);
		synSolarIrradianceAccessors[i] = &collocatedSegment.getAccessor("solar_irradiance_" + index);
	}
	for (size_t i = 21; i < 24; i++) {
		const string index = lexical_cast<string>(i + 1);
		synRadianceAccessors[i - 3] = &collocatedSegment.getAccessor("L_" + index);
		synSolarIrradianceAccessors[i - 3] = &collocatedSegment.getAccessor("solar_irradiance_" + index);
	}
	synFlagsAccessor = &collocatedSegment.getAccessor("SYN_flags");
}

void Vbm::prepareAuxdata(Context& context) {
    const AuxdataProvider& vgtRadiativeTransferAuxdata = getAuxdataProvider(context, Constants::AUX_ID_VPRTAX);
    vgtRadiativeTransferAuxdata.getUShort("AMIN", aerosolModel);

    const string synLookupTableFile = "S3__SY_2_" + Constants::AUX_ID_SYRTAX + ".nc";
    synLutRho = &getLookupTable(context, synLookupTableFile, "rho_atm");
    olcLutRatm = &getLookupTable(context, synLookupTableFile, "OLC_R_atm");
    slnLutRatm = &getLookupTable(context, synLookupTableFile, "SLN_R_atm");
    synLutT = &getLookupTable(context, synLookupTableFile, "t");
    getAuxdataProvider(context, Constants::AUX_ID_SYRTAX).getVectorDouble("C_O3", synCO3);

    const string vgtLookupTableFile = "S3__SY_2_" + Constants::AUX_ID_VPRTAX + ".nc";
    hypLutRho = &getLookupTable(context, vgtLookupTableFile, "rho_atm");
    hypLutRatm = &getLookupTable(context, vgtLookupTableFile, "R_atm");
    hypLutT = &getLookupTable(context, vgtLookupTableFile, "t");
    getAuxdataProvider(context, Constants::AUX_ID_VPRTAX).getVectorDouble("C_O3", hypCO3);

    const AuxdataProvider& vgtSpectralResponseAuxdata = getAuxdataProvider(context, Constants::AUX_ID_VPSRAX);
    vgtSpectralResponseAuxdata.getVectorDouble("B0_SRF", hypSpectralResponses[0]);
    vgtSpectralResponseAuxdata.getVectorDouble("B2_SRF", hypSpectralResponses[1]);
    vgtSpectralResponseAuxdata.getVectorDouble("B3_SRF", hypSpectralResponses[2]);
    vgtSpectralResponseAuxdata.getVectorDouble("MIR_SRF", hypSpectralResponses[3]);
    vgtSpectralResponseAuxdata.getVectorDouble("solar_irradiance", hypSolarIrradiances);
    vgtSpectralResponseAuxdata.getVectorDouble("wavelength", hypWavelengths);
}

void Vbm::prepareTiePointData(Context& context) {
	if (tiePointInterpolatorOlc.get() == 0 || tiePointInterpolatorSln.get() == 0) {
		const Segment& olcTiePointSegment = context.getSegment(Constants::SEGMENT_OLC_TP);
		const Segment& slnTiePointSegment = context.getSegment(Constants::SEGMENT_SLN_TP);

		copy(olcTiePointSegment.getAccessor("SZA").getDoubles(), szaOlcTiePoints);
		copy(olcTiePointSegment.getAccessor("SAA").getDoubles(), saaOlcTiePoints);
		copy(olcTiePointSegment.getAccessor("OLC_VZA").getDoubles(), vzaOlcTiePoints);
		copy(olcTiePointSegment.getAccessor("OLC_VAA").getDoubles(), vaaOlcTiePoints);
		copy(slnTiePointSegment.getAccessor("SLN_VZA").getDoubles(), vzaSlnTiePoints);
		copy(slnTiePointSegment.getAccessor("SLN_VAA").getDoubles(), vaaSlnTiePoints);

		copy(olcTiePointSegment.getAccessor("air_pressure").getDoubles(), airPressureTiePoints);
		copy(olcTiePointSegment.getAccessor("ozone").getDoubles(), ozoneTiePoints);
		if (olcTiePointSegment.hasVariable("water_vapour")) {
			copy(olcTiePointSegment.getAccessor("water_vapour").getDoubles(), waterVapourTiePoints);
		}

		const valarray<double>& olcLons = olcTiePointSegment.getAccessor("OLC_TP_lon").getDoubles();
		const valarray<double>& olcLats = olcTiePointSegment.getAccessor("OLC_TP_lat").getDoubles();
		const valarray<double>& slnLons = slnTiePointSegment.getAccessor("SLN_TP_lon").getDoubles();
		const valarray<double>& slnLats = slnTiePointSegment.getAccessor("SLN_TP_lat").getDoubles();

		tiePointInterpolatorOlc = shared_ptr<TiePointInterpolator<double> >(new TiePointInterpolator<double>(olcLons, olcLats));
		tiePointInterpolatorSln = shared_ptr<TiePointInterpolator<double> >(new TiePointInterpolator<double>(slnLons, slnLats));
	}
}

void Vbm::process(Context& context) {
    // todo - create tie point segment and fill it with data

	prepareTiePointData(context);

    Segment& collocatedSegment = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
    const Grid& collocatedGrid = collocatedSegment.getGrid();

    const long firstL = context.getFirstComputableL(collocatedSegment, *this);
    const long lastL = context.getLastComputableL(collocatedSegment, *this);

    valarray<double> synSurfaceReflectances(21);
    valarray<double> hypSurfaceReflectances(914);
    valarray<double> hypToaReflectances(914);
    valarray<double> vgtToaReflectances(4);
    valarray<double> synWavelengths(21);

    synWavelengths[0] = 400.0;
    synWavelengths[1] = 412.5;
    synWavelengths[2] = 442.5;
    synWavelengths[3] = 490.5;
    synWavelengths[4] = 510.0;
    synWavelengths[5] = 560.0;
    synWavelengths[6] = 620.0;
    synWavelengths[7] = 665.0;
    synWavelengths[8] = 673.75;
    synWavelengths[9] = 681.25;
    synWavelengths[10] = 708.75;
    synWavelengths[11] = 753.75;
    synWavelengths[12] = 761.25;
    synWavelengths[13] = 778.75;
    synWavelengths[14] = 865.0;
    synWavelengths[15] = 885.0;
    synWavelengths[16] = 900.0;
    synWavelengths[17] = 1020.0;
    synWavelengths[18] = 1375.0;
    synWavelengths[19] = 1610.0;
    synWavelengths[20] = 2250.0;

    Pixel p;

    valarray<double> f(hypLutRatm->getDimensionCount());
    valarray<double> workspace(hypLutRatm->getVectorWorkspaceSize());
    valarray<double> coordinates(hypLutRatm->getDimensionCount());
    valarray<double> tpiWeights(1);
    valarray<size_t> tpiIndexes(1);

    for (long l = firstL; l <= lastL; l++) {
        context.getLogging().progress("Processing line l = " + lexical_cast<string>(l), getId());
        for (long k = collocatedGrid.getFirstK(); k <= collocatedGrid.getMaxK(); k++) {
            for (long m = collocatedGrid.getFirstM(); m <= collocatedGrid.getMaxM(); m++) {
                const size_t index = collocatedGrid.getIndex(k, l, m);
                setupPixel(p, index, tpiWeights, tpiIndexes);
                performDownscaling(p, synSurfaceReflectances, coordinates, f, workspace);
                performHyperspectralInterpolation(synWavelengths, synSurfaceReflectances, hypSurfaceReflectances);
                performHyperspectralUpscaling(hypSurfaceReflectances, p, hypToaReflectances, coordinates, f, workspace);
                performHyperspectralFiltering(hypToaReflectances, vgtToaReflectances);
                const uint8_t flags = performQualityFlagging(p, vgtToaReflectances);
                setValues(index, flags, vgtToaReflectances);
            }
        }
    }
    context.setLastComputedL(collocatedSegment, *this, lastL);
}

void Vbm::setupPixel(Pixel& p, size_t index, valarray<double>& tpiWeights, valarray<size_t>& tpiIndexes) {
    for(size_t i = 0; i < 21; i++) {
        p.radiances[i] = synRadianceAccessors[i]->isFillValue(index) ? Constants::FILL_VALUE_DOUBLE : synRadianceAccessors[i]->getDouble(index);
        p.solarIrradiances[i] = synSolarIrradianceAccessors[i]->isFillValue(index) ? Constants::FILL_VALUE_DOUBLE : synSolarIrradianceAccessors[i]->getDouble(index);
    }
    p.synFlags = synFlagsAccessor->getUShort(index);
    p.lat = latAccessor->getDouble(index);
    p.lon = lonAccessor->getDouble(index);

    tiePointInterpolatorOlc->prepare(p.lon, p.lat, tpiWeights, tpiIndexes);

    p.sza = tiePointInterpolatorOlc->interpolate(szaOlcTiePoints, tpiWeights, tpiIndexes);
    p.saa = tiePointInterpolatorOlc->interpolate(saaOlcTiePoints, tpiWeights, tpiIndexes);
    p.vzaOlc = tiePointInterpolatorOlc->interpolate(vzaOlcTiePoints, tpiWeights, tpiIndexes);
    p.vaaOlc = tiePointInterpolatorOlc->interpolate(vaaOlcTiePoints, tpiWeights, tpiIndexes);

    p.airPressure = tiePointInterpolatorOlc->interpolate(airPressureTiePoints, tpiWeights, tpiIndexes);
    p.ozone = tiePointInterpolatorOlc->interpolate(ozoneTiePoints, tpiWeights, tpiIndexes);
    if (waterVapourTiePoints.size() != 0) {
        p.waterVapour = tiePointInterpolatorOlc->interpolate(waterVapourTiePoints, tpiWeights, tpiIndexes);
    } else {
        p.waterVapour = 0.2;
    }

    tiePointInterpolatorSln->prepare(p.lon, p.lat, tpiWeights, tpiIndexes);
    p.vzaSln = tiePointInterpolatorSln->interpolate(vzaSlnTiePoints, tpiWeights, tpiIndexes);
    p.vaaSln = tiePointInterpolatorSln->interpolate(vaaSlnTiePoints, tpiWeights, tpiIndexes);

    p.aot = aerosolOpticalThickness(p.lat);
    p.aerosolModel = aerosolModel;
    p.vgtFlags = 0;
}

void Vbm::performDownscaling(const Pixel& p, valarray<double>& synSurfaceReflectances, valarray<double>& coordinates, valarray<double>& f, valarray<double>& w) {
    fill(&synSurfaceReflectances[0], &synSurfaceReflectances[synSurfaceReflectances.size()], Constants::FILL_VALUE_DOUBLE);

    if (isSet(p.synFlags, Constants::SY2_LAND_FLAG)) {
		coordinates[0] = p.airPressure;
		coordinates[1] = p.waterVapour;
		coordinates[2] = p.aot;
		coordinates[3] = p.aerosolModel;
		synLutRho->getVector(&coordinates[0], rho, f, w);

		coordinates[0] = abs(p.vaaOlc - p.saa);
		coordinates[1] = p.sza;
		coordinates[2] = p.vzaOlc;
		coordinates[3] = p.airPressure;
		coordinates[4] = p.waterVapour;
		coordinates[5] = p.aot;
		coordinates[6] = p.aerosolModel;
		olcLutRatm->getVector(&coordinates[0], ratm, f, w);

		coordinates[0] = p.sza;
		coordinates[1] = p.airPressure;
		coordinates[2] = p.waterVapour;
		coordinates[3] = p.aot;
		coordinates[4] = p.aerosolModel;
		synLutT->getVector(&coordinates[0], ts, f, w);

		coordinates[0] = p.vzaOlc;
		synLutT->getVector(&coordinates[0], tv, f, w);

#pragma omp parallel for
		for (size_t i = 0; i < 18; i++) {
			if (p.radiances[i] != Constants::FILL_VALUE_DOUBLE) {
				synSurfaceReflectances[i] = surfaceReflectance(p.ozone, p.vzaOlc, p.sza, p.solarIrradiances[i], p.radiances[i], synCO3[i], rho[i], ratm[i], ts[i], tv[i]);
			}
		}

		coordinates[0] = abs(p.vaaSln - p.saa);
		coordinates[1] = p.sza;
		coordinates[2] = p.vzaSln;
		coordinates[3] = p.airPressure;
		coordinates[4] = p.waterVapour;
		coordinates[5] = p.aot;
		coordinates[6] = p.aerosolModel;
		slnLutRatm->getVector(&coordinates[0], ratm, f, w);

		coordinates[0] = p.vzaSln;
		coordinates[1] = p.airPressure;
		coordinates[2] = p.waterVapour;
		coordinates[3] = p.aot;
		coordinates[4] = p.aerosolModel;
		synLutT->getVector(&coordinates[0], tv, f, w);

#pragma omp parallel for
		for (size_t i = 21; i < 24; i++) {
			if (p.radiances[i - 3] != Constants::FILL_VALUE_DOUBLE) {
				synSurfaceReflectances[i - 3] = surfaceReflectance(p.ozone, p.vzaSln, p.sza, p.solarIrradiances[i - 3], p.radiances[i - 3], synCO3[i], rho[i], ratm[i - 18], ts[i], tv[i]);
			}
		}
	}
}

void Vbm::performHyperspectralInterpolation(const valarray<double>& synWavelengths, const valarray<double>& synSurfaceReflectances, valarray<double>& hypSurfaceReflectances) {
	fill(&hypSurfaceReflectances[0], &hypSurfaceReflectances[hypSurfaceReflectances.size()], Constants::FILL_VALUE_DOUBLE);

		for (size_t i = 0; i < synWavelengths.size() - 1; i++) {
		const double y0 = synSurfaceReflectances[i];
		const double y1 = synSurfaceReflectances[i + 1];
		if (y0 != Constants::FILL_VALUE_DOUBLE && y1 != Constants::FILL_VALUE_DOUBLE) {
			const double x0 = synWavelengths[i];
			const double x1 = synWavelengths[i + 1];
			for (size_t h = 0; h < hypWavelengths.size(); h++) {
				const double wh = hypWavelengths[h];
				if (wh < x0) {
					continue;
				}
				if (wh > x1) {
					break;
				}
				const double w = (wh - x0) / (x1 - x0);
				hypSurfaceReflectances[h] = y0 + w * (y1 - y0);
			}
		}
	}
}

void Vbm::performHyperspectralUpscaling(const valarray<double>& hypSurfaceReflectances, const Pixel& p, valarray<double>& hypToaReflectances, valarray<double>& coordinates, valarray<double>& f, valarray<double>& w) {
	fill(&hypToaReflectances[0], &hypToaReflectances[hypToaReflectances.size()], Constants::FILL_VALUE_DOUBLE);

	if (isSet(p.synFlags, Constants::SY2_LAND_FLAG)) {
		coordinates[0] = p.airPressure;
		coordinates[1] = p.waterVapour;
		coordinates[2] = p.aot;
		coordinates[3] = p.aerosolModel;
		hypLutRho->getVector(&coordinates[0], rho, f, w);

		coordinates[0] = abs(p.vaaOlc - p.saa);
		coordinates[1] = p.sza;
		coordinates[2] = p.vzaOlc;
		coordinates[3] = p.airPressure;
		coordinates[4] = p.waterVapour;
		coordinates[5] = p.aot;
		coordinates[6] = p.aerosolModel;
		hypLutRatm->getVector(&coordinates[0], ratm, f, w);

		coordinates[0] = p.sza;
		coordinates[1] = p.airPressure;
		coordinates[2] = p.waterVapour;
		coordinates[3] = p.aot;
		coordinates[4] = p.aerosolModel;
		hypLutT->getVector(&coordinates[0], ts, f, w);

		coordinates[0] = p.vzaOlc;
		hypLutT->getVector(&coordinates[0], tv, f, w);

		const double airMass = 0.5 * (1.0 / cos(p.sza * D2R) + 1.0 / cos(p.vzaOlc * D2R));

#pragma omp parallel for
		for (size_t h = 0; h < hypSurfaceReflectances.size(); h++) {
			if (hypSurfaceReflectances[h] != Constants::FILL_VALUE_DOUBLE) {
				hypToaReflectances[h] = toaReflectance(p.ozone, airMass, hypSurfaceReflectances[h], hypCO3[h], rho[h], ratm[h], ts[h], tv[h]);
			}
		}
	}
}

void Vbm::performHyperspectralFiltering(const valarray<double>& hypToaReflectances, valarray<double>& vgtToaReflectances) const {
	fill(&vgtToaReflectances[0], &vgtToaReflectances[vgtToaReflectances.size()], Constants::FILL_VALUE_DOUBLE);

#pragma omp parallel for
	for (size_t b = 0; b < 4; b++) {
		const valarray<double>& hypSpectralResponse = hypSpectralResponses[b];
		double rs = 0.0;
		double ws = 0.0;

		for (size_t h = 0; h < 914; h++) {
			const double r = hypSpectralResponse[h];
			if (r > 0.0) {
				const double hypSolarIrradiance = hypSolarIrradiances[h];
				const double hypToaReflectance = hypToaReflectances[h];
				if (hypToaReflectance != Constants::FILL_VALUE_DOUBLE) {
					const double w = r * hypSolarIrradiance;
					rs += w * hypToaReflectance;
					ws += w;
				}
			}
		}
		if (ws != 0.0) {
			vgtToaReflectances[b] = rs / ws;
		}
	}
}

uint8_t Vbm::performQualityFlagging(Pixel& p, const valarray<double>& vgtToaReflectances) const {
    uint8_t flags = 0;
    if (vgtToaReflectances[0] != Constants::FILL_VALUE_DOUBLE &&
    		p.radiances[1] != Constants::FILL_VALUE_DOUBLE &&
    		p.radiances[2] != Constants::FILL_VALUE_DOUBLE) {
        flags |= Constants::VGT_B0_GOOD_FLAG;
    }
    if (vgtToaReflectances[1] != Constants::FILL_VALUE_DOUBLE &&
    		p.radiances[5] != Constants::FILL_VALUE_DOUBLE &&
    		p.radiances[6] != Constants::FILL_VALUE_DOUBLE &&
    		p.radiances[7] != Constants::FILL_VALUE_DOUBLE &&
    		p.radiances[8] != Constants::FILL_VALUE_DOUBLE &&
    		p.radiances[9] != Constants::FILL_VALUE_DOUBLE) {
        flags |= Constants::VGT_B2_GOOD_FLAG;
    }
    if (vgtToaReflectances[2] != Constants::FILL_VALUE_DOUBLE &&
    		p.radiances[13] != Constants::FILL_VALUE_DOUBLE &&
    		p.radiances[14] != Constants::FILL_VALUE_DOUBLE &&
    		p.radiances[15] != Constants::FILL_VALUE_DOUBLE &&
    		p.radiances[16] != Constants::FILL_VALUE_DOUBLE &&
    		p.radiances[17] != Constants::FILL_VALUE_DOUBLE) {
        flags |= Constants::VGT_B3_GOOD_FLAG;
    }
    if (vgtToaReflectances[3] != Constants::FILL_VALUE_DOUBLE &&
    		p.radiances[19] != Constants::FILL_VALUE_DOUBLE &&
    		p.radiances[20] != Constants::FILL_VALUE_DOUBLE) {
        flags |= Constants::VGT_MIR_GOOD_FLAG;
    }
    if (isSet(p.synFlags, Constants::SY2_LAND_FLAG)) {
    	p.vgtFlags |= Constants::VGT_LAND_FLAG;
    }
    return flags;
}

void Vbm::setValues(size_t index, uint8_t flags, const valarray<double>& vgtToaReflectances) {
    for (size_t i = 0; i < vgtReflectanceAccessors.size(); i++) {
        if (vgtToaReflectances[i] != Constants::FILL_VALUE_DOUBLE) {
            vgtReflectanceAccessors[i]->setDouble(index, vgtToaReflectances[i]);
        } else {
            vgtReflectanceAccessors[i]->setFillValue(index);
        }
    }
    vgtFlagsAccessor->setUByte(index, flags);
}

double Vbm::aerosolOpticalThickness(double lat) {
	const double c = cos(lat * D2R);

    return 0.2 * (c - 0.25) * (c * c * c) + 0.05;
}

double Vbm::surfaceReflectance(double nO3, double vza, double sza, double f0, double ltoa, double cO3, double rho, double ratm, double ts, double tv) {
	// Eq. 2-1
	const double rtoa = Aco::toaReflectance(ltoa, f0, sza);
	// Eq. 2-2
	const double tO3 = Aco::ozoneTransmission(cO3, sza, vza, nO3);
	// Eq. 2-3
	return Aco::surfaceReflectance(rtoa, ratm, ts, tv, rho, tO3);
}

double Vbm::toaReflectance(double nO3, double airMass, double surfaceReflectance, double cO3, double rho, double ratm, double ts, double tv) {
    const double tO3 = exp(-airMass * nO3 * cO3);
    const double g = ts * tv;

    return tO3 * (ratm + (g * surfaceReflectance) / ((1.0 - rho) * surfaceReflectance));
}
