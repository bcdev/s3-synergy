/*
 * Vbm.cpp
 *
 *  Created on: Nov 17, 2011
 *      Author: thomasstorm
 */

#include "Vbm.h"

using std::abs;
using std::fill;

Vbm::Vbm() :
        BasicModule("VBM"), vgtBSurfaceReflectanceWeights(4), vgtSolarIrradiances(914), hyperWavelengths(914), synRadianceAccessors(30), synSolarIrradianceAccessors(30),
        szaOlcTiePoints(0), saaOlcTiePoints(0), vzaOlcTiePoints(0), vzaSlnTiePoints(0), waterVapourTiePoints(0), airPressureTiePoints(0),
        ozoneTiePoints(0), coordinates(7), f(8), w(500000), vgtRhoAtm(914), vgtRAtm(914), vgtTSun(914), vgtTView(914), synRhoAtm(30), synRAtmOlc(18), synTSun(30),
        synTViewOlc(30), synRAtmSln(6), synTViewSln(30), wavelengthIndices_0(), wavelengthIndices_1(), targetAccessors(4) {
}

Vbm::~Vbm() {
}

void Vbm::start(Context& context) {
    collocatedSegment = &context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
    olciInfoSegment = &context.getSegment(Constants::SEGMENT_OLC_INFO);

    addVariables(context);

    prepareAccessors();
    prepareAuxdata(context);
}

void Vbm::prepareAccessors() {
    synLatitudeAccessor = &collocatedSegment->getAccessor("latitude");
    synLongitudeAccessor = &collocatedSegment->getAccessor("longitude");
    for(size_t i = 0;i < 30;i++){
        const string index = lexical_cast<string>(i + 1);
        synRadianceAccessors[i] = &collocatedSegment->getAccessor("L_" + index);
        synSolarIrradianceAccessors[i] = &collocatedSegment->getAccessor("solar_irradiance_" + index);
    }
    targetAccessors[0] = vgtB0Accessor;
    targetAccessors[1] = vgtB2Accessor;
    targetAccessors[2] = vgtB3Accessor;
    targetAccessors[3] = vgtMirAccessor;
}

void Vbm::prepareAuxdata(Context& context) {
    AuxdataProvider& radiativeTransfer = getAuxdataProvider(context, Constants::AUX_ID_VPRTAX);
    amin = radiativeTransfer.getShort("AMIN");

    synLutRhoAtm = &getLookupTable(context, "S3__SY_2_" + Constants::AUX_ID_SYRTAX + ".nc", "rho_atm");
    synLutOlcRatm = &getLookupTable(context, "S3__SY_2_" + Constants::AUX_ID_SYRTAX + ".nc", "OLC_R_atm");
    synLutSlnRatm = &getLookupTable(context, "S3__SY_2_" + Constants::AUX_ID_SYRTAX + ".nc", "SLN_R_atm");
    synLutT = &getLookupTable(context, "S3__SY_2_" + Constants::AUX_ID_SYRTAX + ".nc", "t");
    synCo3 = &getAuxdataProvider(context, Constants::AUX_ID_SYRTAX).getVectorDouble("C_O3");

    vgtLutRhoAtm = &getLookupTable(context, "S3__SY_2_" + Constants::AUX_ID_VPRTAX + ".nc", "rho_atm");
    vgtLutRAtm = &getLookupTable(context, "S3__SY_2_" + Constants::AUX_ID_VPRTAX + ".nc", "R_atm");
    vgtLutT = &getLookupTable(context, "S3__SY_2_" + Constants::AUX_ID_VPRTAX + ".nc", "t");
    vgtCo3 = &getAuxdataProvider(context, Constants::AUX_ID_VPRTAX).getVectorDouble("C_O3");

    const AuxdataProvider& vpsraxAuxdata = getAuxdataProvider(context, Constants::AUX_ID_VPSRAX);

    const valarray<double> b0Srf = vpsraxAuxdata.getVectorDouble("B0_SRF");
    vgtBSurfaceReflectanceWeights[0] = valarray<double>(b0Srf.size());
    copy(b0Srf, vgtBSurfaceReflectanceWeights[0]);

    const valarray<double> b2Srf = vpsraxAuxdata.getVectorDouble("B2_SRF");
    vgtBSurfaceReflectanceWeights[1] = valarray<double>(b2Srf.size());
    copy(b2Srf, vgtBSurfaceReflectanceWeights[1]);

    const valarray<double> b3Srf = vpsraxAuxdata.getVectorDouble("B3_SRF");
    vgtBSurfaceReflectanceWeights[2] = valarray<double>(b3Srf.size());
    copy(b3Srf, vgtBSurfaceReflectanceWeights[2]);

    const valarray<double> mirSrf = vpsraxAuxdata.getVectorDouble("MIR_SRF");
    vgtBSurfaceReflectanceWeights[3] = valarray<double>(mirSrf.size());
    copy(mirSrf, vgtBSurfaceReflectanceWeights[3]);

    copy(vpsraxAuxdata.getVectorDouble("solar_irradiance"), vgtSolarIrradiances);

    copy(getAuxdataProvider(context, Constants::AUX_ID_VPRTAX).getVectorDouble("wavelength"), hyperWavelengths);
}

void Vbm::prepareTiePointData(Context& context) {
    const Segment& olciTiepointSegment = context.getSegment(Constants::SEGMENT_OLC_TP);
    const Segment& slnTiepointSegment = context.getSegment(Constants::SEGMENT_SLN_TP);

    copy(olciTiepointSegment.getAccessor("SZA").getDoubles(), szaOlcTiePoints);
    copy(olciTiepointSegment.getAccessor("SAA").getDoubles(), saaOlcTiePoints);
    copy(olciTiepointSegment.getAccessor("OLC_VZA").getDoubles(), vzaOlcTiePoints);
    copy(slnTiepointSegment.getAccessor("SLN_VZA").getDoubles(), vzaSlnTiePoints);

    copy(olciTiepointSegment.getAccessor("ozone").getDoubles(), ozoneTiePoints);
    copy(olciTiepointSegment.getAccessor("air_pressure").getDoubles(), airPressureTiePoints);

    if (olciTiepointSegment.hasVariable("water_vapour")) {
        copy(olciTiepointSegment.getAccessor("water_vapour").getDoubles(), waterVapourTiePoints);
    }

    const valarray<double> olciLats = olciTiepointSegment.getAccessor("OLC_TP_lat").getDoubles();
    const valarray<double> olciLons = olciTiepointSegment.getAccessor("OLC_TP_lon").getDoubles();
    const valarray<double> slnLats = slnTiepointSegment.getAccessor("SLN_TP_lat").getDoubles();
    const valarray<double> slnLons = slnTiepointSegment.getAccessor("SLN_TP_lon").getDoubles();
    tiePointInterpolatorOlc = shared_ptr<TiePointInterpolator<double > >(new TiePointInterpolator<double>(olciLons, olciLats));
    tiePointInterpolatorSln = shared_ptr<TiePointInterpolator<double > >(new TiePointInterpolator<double>(slnLons, slnLats));
}

void Vbm::addVariables(Context& context) {
    context.getLogging().info("Adding variables to context", getId());
    vgtFlagsAccessor = &collocatedSegment->addVariable("SM", Constants::TYPE_UBYTE);
    vgtB0Accessor = &collocatedSegment->addVariable("B0", Constants::TYPE_UBYTE);
    vgtB2Accessor = &collocatedSegment->addVariable("B2", Constants::TYPE_UBYTE);
    vgtB3Accessor = &collocatedSegment->addVariable("B3", Constants::TYPE_UBYTE);
    vgtMirAccessor = &collocatedSegment->addVariable("MIR", Constants::TYPE_UBYTE);
}

void Vbm::process(Context& context) {
    prepareTiePointData(context);

    const Grid& collocatedGrid = collocatedSegment->getGrid();

    // todo - verify
    long firstL = context.getFirstComputableL(*collocatedSegment, *this);
    long lastL = context.getLastComputableL(*collocatedSegment, *this);

    valarray<double> surfaceReflectances(21);
    valarray<double> hyperSpectralReflectances(914);
    valarray<double> toaReflectances(914);
    valarray<double> vgtToaReflectances(4);

    valarray<double> channelWavelengths(surfaceReflectances.size());

    Pixel p;

    context.getLogging().info("Performing band mapping...", getId());

    for (long m = collocatedGrid.getFirstM(); m <= collocatedGrid.getMaxM(); m++) {
        context.getLogging().info("...for column " + lexical_cast<string>(m + 1) + "/" + lexical_cast<string>(collocatedGrid.getMaxM() + 1), getId());
        for (long k = collocatedGrid.getFirstK(); k <= collocatedGrid.getMaxK(); k++) {
            computeChannelWavelengths(k, m, channelWavelengths);
            computeInterpolationIndices(channelWavelengths, surfaceReflectances);
            for (long l = firstL; l <= lastL; l++) {
                const size_t index = collocatedGrid.getIndex(k, l, m);
                setupPixel(p, index);
                performDownscaling(p, surfaceReflectances);
                performHyperspectralInterpolation(channelWavelengths, surfaceReflectances, hyperSpectralReflectances);
                performHyperspectralUpscaling(hyperSpectralReflectances, p, toaReflectances);
                performHyperspectralFiltering(toaReflectances, vgtToaReflectances);
                const uint8_t flags = getFlagsAndFill(p, vgtToaReflectances);
                setValues(index, flags, vgtToaReflectances);
            }
        }
    }
}

void Vbm::computeChannelWavelengths(long k, long m, valarray<double>& channelWavelengths) const {
    for(size_t channel = 0; channel < channelWavelengths.size(); channel++) {
        if(channel < 18) {
            const size_t index = olciInfoSegment->getGrid().getIndex(k, channel, m);
            channelWavelengths[channel] = olciInfoSegment->getAccessor("wavelength").getDouble(index);
        } else {
            // ignoring channel indices 18, 19, 20
            channelWavelengths[channel] = getSlnWavelength(channel + 3);
        }
    }
}

double Vbm::getSlnWavelength(size_t channel) {
    switch (channel) {
    case 21:
        return 1375;
    case 22:
        return 1610;
    case 23:
        return 2250;
    }
    BOOST_THROW_EXCEPTION(logic_error("invalid channel index '" + lexical_cast<string>(channel) + "'"));
}

void Vbm::computeInterpolationIndices(const valarray<double>& channelWavelengths, const valarray<double>& surfaceReflectances) {
    for(size_t h = 0; h < hyperWavelengths.size(); h++) {
        double x0Delta = numeric_limits<size_t>::max();
        double x1Delta = numeric_limits<size_t>::max();
        bool wavelengthIndex0Set = false;
        bool wavelengthIndex1Set = false;
        for (size_t i = 0; i < channelWavelengths.size(); i++) {
            double delta = abs(channelWavelengths[i] - hyperWavelengths[h]);
            if (channelWavelengths[i] <= hyperWavelengths[h]) {
                if(delta < x0Delta) {
                    wavelengthIndices_0[h] = i;
                    x0Delta = delta;
                    wavelengthIndex0Set = true;
                }
            } else {
                if(delta < x1Delta) {
                    wavelengthIndices_1[h] = i;
                    x1Delta = delta;
                    wavelengthIndex1Set = true;
                }
            }
        }
        if(!wavelengthIndex0Set) {
            wavelengthIndices_0[h] = wavelengthIndices_1[h];
        }
        if(!wavelengthIndex1Set) {
            wavelengthIndices_1[h] = wavelengthIndices_0[h];
        }
    }
}

void Vbm::setupPixel(Pixel& p, size_t index) {
    p.aerosolModel = amin;
    for(size_t i = 0; i < 30; i++) {
        p.radiances[i] = synRadianceAccessors[i]->isFillValue(index) ? Constants::FILL_VALUE_DOUBLE : synRadianceAccessors[i]->getDouble(index);
        p.solarIrradiances[i] = synSolarIrradianceAccessors[i]->isFillValue(index) ? Constants::FILL_VALUE_DOUBLE : synSolarIrradianceAccessors[i]->getDouble(index);
    }
    p.lat = synLatitudeAccessor->getDouble(index);
    p.lon = synLongitudeAccessor->getDouble(index);

    valarray<double> tpiWeights(1);
    valarray<size_t> tpiIndexes(1);

    tiePointInterpolatorOlc->prepare(p.lat, p.lon, tpiWeights, tpiIndexes);

    p.sza = tiePointInterpolatorOlc->interpolate(szaOlcTiePoints, tpiWeights, tpiIndexes);
    p.saa = tiePointInterpolatorOlc->interpolate(saaOlcTiePoints, tpiWeights, tpiIndexes);
    p.vzaOlc = tiePointInterpolatorOlc->interpolate(vzaOlcTiePoints, tpiWeights, tpiIndexes);

    p.airPressure = tiePointInterpolatorOlc->interpolate(airPressureTiePoints, tpiWeights, tpiIndexes);
    p.ozone = tiePointInterpolatorOlc->interpolate(ozoneTiePoints, tpiWeights, tpiIndexes);
    if (waterVapourTiePoints.size() != 0) {
        p.waterVapour = tiePointInterpolatorOlc->interpolate(waterVapourTiePoints, tpiWeights, tpiIndexes);
    } else {
        p.waterVapour = 0.2;
    }

    tiePointInterpolatorSln->prepare(p.lat, p.lon, tpiWeights, tpiIndexes);
    p.vzaSln = tiePointInterpolatorSln->interpolate(vzaSlnTiePoints, tpiWeights, tpiIndexes);
    p.aot = computeT550(p.lat);
}

void Vbm::performDownscaling(const Pixel& p, valarray<double>& surfReflNadirSyn) {
    coordinates[0] = p.airPressure;
    coordinates[1] = p.waterVapour;
    coordinates[2] = p.aot;
    coordinates[3] = p.aerosolModel;

    synLutRhoAtm->getVector(&coordinates[0], synRhoAtm, f, w);

    coordinates[0] = abs(p.vaaOlc - p.saa);
    coordinates[1] = p.sza;
    coordinates[2] = p.vzaOlc;
    coordinates[3] = p.airPressure;
    coordinates[4] = p.waterVapour;
    coordinates[5] = p.aot;
    coordinates[6] = p.aerosolModel;

    synLutOlcRatm->getVector(&coordinates[0], synRAtmOlc, f, w);

    coordinates[0] = p.sza;
    coordinates[1] = p.airPressure;
    coordinates[2] = p.waterVapour;
    coordinates[3] = p.aot;
    coordinates[4] = p.aerosolModel;

    synLutT->getVector(&coordinates[0], synTSun, f, w);

    coordinates[0] = p.vzaOlc;
    synLutT->getVector(&coordinates[0], synTViewOlc, f, w);

    for(size_t i = 0; i < 18; i++) {
        surfReflNadirSyn[i] = surfaceReflectance(
                p.ozone,
                p.vzaOlc,
                p.sza,
                p.solarIrradiances[i],
                p.radiances[i],
                (*synCo3)[i],  // todo - verify!
                synRhoAtm[i],
                synRAtmOlc[i],
                synTSun[i],
                synTViewOlc[i]);
    }

    coordinates[0] = abs(p.vaaSln - p.saa);
    coordinates[1] = p.sza;
    coordinates[2] = p.vzaSln;
    coordinates[3] = p.airPressure;
    coordinates[4] = p.waterVapour;
    coordinates[5] = p.aot;
    coordinates[6] = p.aerosolModel;

    synLutSlnRatm->getVector(&coordinates[0], synRAtmSln, f, w);

    coordinates[0] = p.sza;
    coordinates[1] = p.airPressure;
    coordinates[2] = p.waterVapour;
    coordinates[3] = p.aot;
    coordinates[4] = p.aerosolModel;

    coordinates[0] = p.vzaSln;
    coordinates[1] = p.airPressure;
    coordinates[2] = p.waterVapour;
    coordinates[3] = p.aot;
    coordinates[4] = p.aerosolModel;

    synLutT->getVector(&coordinates[0], synTViewSln, f, w);

    // only last three channels are used, since wavelengths of channels 18, 19, 20 are already present in OLC
    for(size_t i = 21; i < 24; i++) {
        surfReflNadirSyn[i-3] = surfaceReflectance(
                p.ozone,
                p.vzaSln,
                p.sza,
                p.solarIrradiances[i],
                p.radiances[i],
                (*synCo3)[i],  // todo - verify!
                synRhoAtm[i],
                synRAtmSln[i],
                synTSun[i],
                synTViewSln[i]);
    }
}

double Vbm::surfaceReflectance(double ozone, double vza, double sza, double solarIrradiance, double radiance,
        double co3, double rhoAtm, double rAtm, double tSun, double tView) {
    if(radiance == Constants::FILL_VALUE_DOUBLE) {
        return Constants::FILL_VALUE_DOUBLE;
    }

    double rToa = M_PI * radiance / (solarIrradiance * std::cos(sza));
    double M = 0.5 * (1/std::cos(sza) + 1/std::cos(vza));
    double t_O3 = std::exp(-M * ozone * co3);

    double f = (rToa - t_O3 * rAtm) / (t_O3 * tSun * tView);
    return f / (1 + rhoAtm * f);
}

double Vbm::hyperspectralUpscale(double ozone, double M, double hyperSpectralReflectance, double co3, double rhoAtm, double rAtm, double tSun, double tView) {
    if(hyperSpectralReflectance == Constants::FILL_VALUE_DOUBLE) {
        return Constants::FILL_VALUE_DOUBLE;
    }
    const double tO3 = std::exp(-M * ozone * co3);
    const double g = tSun * tView;
    return tO3 * (rAtm + (g * hyperSpectralReflectance) / ((1 - rhoAtm) * hyperSpectralReflectance));
}


void Vbm::performHyperspectralInterpolation(const valarray<double>& channelWavelengths, const valarray<double>& surfaceReflectances, valarray<double>& hyperSpectralReflectances) {
#pragma omp parallel for
    for(size_t i = 0; i < hyperWavelengths.size(); i++) {
        hyperSpectralReflectances[i] = linearInterpolation(channelWavelengths, surfaceReflectances, i);
    }
}

void Vbm::performHyperspectralUpscaling(const valarray<double>& hyperSpectralReflectances, const Pixel& p, valarray<double>& toaReflectances) {
    coordinates[0] = p.airPressure;
    coordinates[1] = p.waterVapour;
    coordinates[2] = p.aot;
    coordinates[3] = p.aerosolModel;
    vgtLutRhoAtm->getVector(&coordinates[0], vgtRhoAtm, f, w);

    coordinates[0] = abs(p.vaaOlc - p.saa);
    coordinates[1] = p.sza;
    coordinates[2] = p.vzaOlc;
    coordinates[3] = p.airPressure;
    coordinates[4] = p.waterVapour;
    coordinates[5] = p.aot;
    coordinates[6] = p.aerosolModel;

    vgtLutRAtm->getVector(&coordinates[0], vgtRAtm, f, w);

    coordinates[0] = p.sza;
    coordinates[1] = p.airPressure;
    coordinates[2] = p.waterVapour;
    coordinates[3] = p.aot;
    coordinates[4] = p.aerosolModel;

    vgtLutT->getVector(&coordinates[0], vgtTSun, f, w);

    coordinates[0] = p.vzaOlc;
    vgtLutT->getVector(&coordinates[0], vgtTView, f, w);

    const double M = 0.5 * (1 / std::cos(p.sza) + 1 / (std::cos(p.vzaOlc)));
#pragma omp parallel for
    for(size_t h = 0; h < hyperSpectralReflectances.size(); h++) {
        toaReflectances[h] = hyperspectralUpscale(p.ozone, M, hyperSpectralReflectances[h], (*vgtCo3)[h], vgtRhoAtm[h], vgtRAtm[h], vgtTSun[h], vgtTView[h]);
    }
}

void Vbm::performHyperspectralFiltering(const valarray<double>& toaReflectances, valarray<double>& filteredRToa) const {
    for(size_t b = 0; b < 4; b++) {
        double numerator = 0.0;
        double denominator = 0.0;
        for(size_t h = 0; h < 914; h++) {
            double solarIrr = vgtSolarIrradiances[h];
            double bSurf = (vgtBSurfaceReflectanceWeights[b])[h];
            if(toaReflectances[h] == Constants::FILL_VALUE_DOUBLE) {
                continue;
            }
            numerator += solarIrr * bSurf * toaReflectances[h];
            denominator += solarIrr * bSurf;
        }
        if(denominator != 0.0) {
            filteredRToa[b] = numerator / denominator;
        } else {
            filteredRToa[b] = Constants::FILL_VALUE_DOUBLE;
        }
    }
}

uint8_t Vbm::getFlagsAndFill(Pixel& p, valarray<double>& vgtToaReflectances) const {
    uint8_t flags = 0;
    if (p.radiances[1] != Constants::FILL_VALUE_DOUBLE && p.radiances[2] != Constants::FILL_VALUE_DOUBLE) {
        flags |= Constants::VGT_B0_GOOD;
    } else {
        vgtToaReflectances[0] = Constants::FILL_VALUE_DOUBLE;
    }

    bool vgtB2Good = true;
    for (size_t i = 5; i < 10; i++) {
        if (p.radiances[i] == Constants::FILL_VALUE_DOUBLE) {
            vgtB2Good = false;
            vgtToaReflectances[1] = Constants::FILL_VALUE_DOUBLE;
            break;
        }
    }
    flags |= vgtB2Good ? Constants::VGT_B2_GOOD : 0;

    bool vgtB3Good = true;
    for (size_t i = 13; i < 18; i++) {
        if (p.radiances[i] == Constants::FILL_VALUE_DOUBLE) {
            vgtB3Good = false;
            vgtToaReflectances[2] = Constants::FILL_VALUE_DOUBLE;
            break;
        }
    }
    flags |= vgtB3Good ? Constants::VGT_B3_GOOD : 0;

    if (p.radiances[22] != Constants::FILL_VALUE_DOUBLE && p.radiances[23] != Constants::FILL_VALUE_DOUBLE) {
        flags |= Constants::VGT_MIR_GOOD;
    } else {
        vgtToaReflectances[3] = Constants::FILL_VALUE_DOUBLE;
    }
    return flags;
}

