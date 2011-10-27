/*
 * ErrorMetric.cpp
 *
 *  Created on: 13.10.2011
 *      Author: thomasstorm
 */

#include <limits>

#include "../core/LookupTable.h"

#include "AuxdataProvider.h"
#include "ErrorMetric.h"

using std::numeric_limits;

ErrorMetric::ErrorMetric(const Pixel& p, int16_t amin, double tau550, Context& context) :
       lutOlcRatm((MatrixLookupTable<double>&) context.getObject("OLC_R_atm")), lutSlnRatm((MatrixLookupTable<double>&) context.getObject("SLN_R_atm")), lutSloRatm(
                (MatrixLookupTable<double>&) context.getObject("SLO_R_atm")), lutT((MatrixLookupTable<double>&) context.getObject("t")), lutRhoAtm(
                (MatrixLookupTable<double>&) context.getObject("rho_atm")), lutTotalAngularWeights((ScalarLookupTable<double>&) context.getObject("weight_ang_tot")),
                D(6), sdrs(30), rSpec(30), rAng(12) {
    const VectorLookupTable<double>& lutD = (VectorLookupTable<double>&) context.getObject("D");
    valarray<double> coordinates(lutD.getDimensionCount());
    assert(coordinates.size() == 4);
    coordinates[0] = p.sza;
    coordinates[1] = p.airPressure;
    coordinates[2] = p.tau550;
    coordinates[3] = amin;

    lutD.getValues(&coordinates[0], D);

    AuxdataProvider& configurationAuxdata = (AuxdataProvider&) context.getObject(Constants::AUXDATA_CONFIGURATION_ID);
    spectralWeights = configurationAuxdata.getDoubleArray("weight_spec");
    vegetationSpectrum = configurationAuxdata.getDoubleArray("R_veg");
    soilReflectance = configurationAuxdata.getDoubleArray("R_soil");
    valarray<int16_t> ndviIndices = configurationAuxdata.getShortArray("NDV_channel");
    angularWeights = configurationAuxdata.getDoubleMatrix("weight_ang");
    gamma = configurationAuxdata.getDouble("gamma");

    const double Constants::FILL_VALUE_DOUBLE = -numeric_limits<double_t>::max();

    for (size_t i = 1; i <= 30; i++) {
        if (p.radiances[i - 1] == Constants::FILL_VALUE_DOUBLE) {
            spectralWeights[i - 1] = 0;
            if (i >= 19 && i <= 24) {
                angularWeights.insert_element(0, i - 19, 0.0);
            } else if (i >= 25 && i <= 30) {
                angularWeights.insert_element(1, i - 25, 0.0);
            }
        }
    }
    applyAtmosphericCorrection(p, amin);

    sum2 = 0.0;
    sum4 = 0.0;
    for (size_t i = 0; i < 30; i++) {
        sum2 += spectralWeights[i];
    }
    for (size_t i = 0; i < 12; i++) {
        size_t xIndex = i < 6 ? 0 : 1;
        size_t yIndex = i % 6;
        sum4 += angularWeights.at_element(xIndex, yIndex);
    }
    const double ndvi = ndv(p, ndviIndices);
    totalAngularWeight = lutTotalAngularWeights.getValue(&ndvi);
}

double ErrorMetric::value(valarray<double>& x) {
    for (size_t i = 0; i < 30; i++) {
        rSpec[i] = specModelSurf(x[0], x[1], i);
    }
    for (size_t i = 0; i < 12; i++) {
        rAng[i] = angModelSurf(i, x);
    }
    return errorMetric();
}

static double ozoneTransmission(double cO3, double sza, double vza, double nO3) {
    static const double D2R = 3.14159265358979323846 / 180.0;
    // Eq. 2-2
    const double m = 0.5 * (1.0 / std::cos(sza * D2R) + 1.0 / std::cos(vza * D2R));
    const double tO3 = std::exp(-m * nO3 * cO3);

    return tO3;
}

static double toaReflectance(double ltoa, double f0, double sza) {
    static const double D2R = 3.14159265358979323846 / 180.0;
    static const double PI = 3.14159265358979323846;
    return (PI * ltoa) / (f0 * std::cos(sza * D2R));
}

static double surfaceReflectance(double rtoa, double ratm, double ts, double tv, double rho, double tO3) {
    // Eq. 2-3
    const double f = (rtoa - tO3 * ratm) / (tO3 * ts * tv);
    const double rboa = f / (1.0 + rho * f);

    return rboa;
}

void ErrorMetric::applyAtmosphericCorrection(const Pixel& p, int16_t amin) {
    using std::abs;

    valarray<double> coordinates(10);

    coordinates[0] = abs(p.saa - p.vaaOlc); // ADA
    coordinates[1] = p.sza; // SZA
    coordinates[2] = p.vzaOlc; // VZA
    coordinates[3] = p.airPressure; // air pressure
    coordinates[4] = p.waterVapour; // water vapour
    coordinates[5] = p.tau550; // aerosol

    coordinates[6] = coordinates[1]; // SZA
    coordinates[7] = coordinates[3]; // air pressure
    coordinates[8] = coordinates[4]; // water vapour
    coordinates[9] = coordinates[5]; // aerosol

    matrix<double> matRatmOlc(40, 18);
    matrix<double> matRatmSln(40, 6);
    matrix<double> matRatmSlo(40, 6);
    matrix<double> matTs(40, 30);
    matrix<double> matTv(40, 30);
    matrix<double> matRho(40, 30);

    valarray<double> f(lutOlcRatm.getDimensionCount());
    valarray<double> w(lutOlcRatm.getWorkspaceSize());

    lutOlcRatm.getValues(&coordinates[0], matRatmOlc, f, w);
    lutT.getValues(&coordinates[6], matTs, f, w);
    lutT.getValues(&coordinates[2], matTv, f, w);
    lutRhoAtm.getValues(&coordinates[3], matRho, f, w);

    for (size_t b = 0; b < 18; b++) {
        if (spectralWeights[b] > 0.0) {
            // Eq. 2-1
            const double rtoa = toaReflectance(p.radiances[b], p.solarIrradiances[b], p.sza);

            // Eq. 2-2
            const double tO3 = ozoneTransmission(p.cO3[b], p.sza, p.vzaOlc, p.ozone);

            // Eq. 2-3
            const double ratm = matRatmOlc(amin - 1, b);
            const double ts = matTs(amin - 1, b);
            const double tv = matTv(amin - 1, b);
            const double rho = matRho(amin - 1, b);
            const double sdr = surfaceReflectance(rtoa, ratm, ts, tv, rho, tO3);

            sdrs[b] = sdr;
        }
    }

    coordinates[0] = abs(p.saa - p.vaaSln); // ADA
    coordinates[1] = p.sza; // SZA
    coordinates[2] = p.vzaSln; // VZA
    coordinates[3] = p.airPressure; // air pressure
    coordinates[4] = p.waterVapour; // water vapour
    coordinates[5] = p.tau550; // aerosol

    lutSlnRatm.getValues(&coordinates[0], matRatmSln, f, w);
    lutT.getValues(&coordinates[2], matTv, f, w);

    for (size_t b = 18; b < 24; b++) {
        if (spectralWeights[b] > 0.0) {
            // Eq. 2-1
            const double rtoa = toaReflectance(p.radiances[b], p.solarIrradiances[b], p.sza);

            // Eq. 2-2
            const double tO3 = ozoneTransmission(p.cO3[b], p.sza, p.vzaOlc, p.ozone);

            // Eq. 2-3
            const double ratm = matRatmSln(amin - 1, b - 18);
            const double ts = matTs(amin - 1, b);
            const double tv = matTv(amin - 1, b);
            const double rho = matRho(amin - 1, b);
            const double sdr = surfaceReflectance(rtoa, ratm, ts, tv, rho, tO3);

            sdrs[b] = sdr;
        }
    }

    coordinates[0] = abs(p.saa - p.vaaSlo); // ADA
    coordinates[1] = p.sza; // SZA
    coordinates[2] = p.vzaSlo; // VZA
    coordinates[3] = p.airPressure; // air pressure
    coordinates[4] = p.waterVapour; // water vapour
    coordinates[5] = p.tau550; // aerosol

    lutSloRatm.getValues(&coordinates[0], matRatmSlo, f, w);
    lutT.getValues(&coordinates[2], matTv, f, w);

    for (size_t b = 24; b < 30; b++) {
        if (spectralWeights[b] > 0.0) {
            // Eq. 2-1
            const double rtoa = toaReflectance(p.radiances[b], p.solarIrradiances[b], p.sza);

            // Eq. 2-2
            const double tO3 = ozoneTransmission(p.cO3[b], p.sza, p.vzaOlc, p.ozone);

            // Eq. 2-3
            const double ratm = matRatmSlo(amin - 1, b - 24);
            const double ts = matTs(amin - 1, b);
            const double tv = matTv(amin - 1, b);
            const double rho = matRho(amin - 1, b);
            const double sdr = surfaceReflectance(rtoa, ratm, ts, tv, rho, tO3);

            sdrs[b] = sdr;
        }
    }
}

double ErrorMetric::angModelSurf(size_t index, valarray<double>& x) {
    const size_t j = index % 6;
    const size_t o = index / 6;
    const double d = D[j];
    const double omega = x[j + 4];
    const double nu = x[o + 2];
    const double g = (1 - gamma) * omega;
    return (1 - d) * nu * omega + (gamma * omega) / (1 - g) * (d + g * (1 - d));
}

double ErrorMetric::specModelSurf(double c_1, double c_2, size_t index) {
    return c_1 * vegetationSpectrum[index] + c_2 * soilReflectance[index];
}

double ErrorMetric::errorMetric() {
    double sum1 = 0.0;
    double sum3 = 0.0;
    for (size_t i = 0; i < 30; i++) {
        const double w = spectralWeights[i];
        if (w > 0.0) {
            sum1 += w * (sdrs[i] - rSpec[i]) * (sdrs[i] - rSpec[i]);
        }
    }
    for (size_t i = 0; i < 12; i++) {
        size_t xIndex = i < 6 ? 0 : 1;
        size_t yIndex = i % 6;
        const double w = angularWeights.at_element(xIndex, yIndex);
        if (w > 0.0) {
            sum3 += w * (sdrs[i] - rAng[i]) * (sdrs[i] - rAng[i]);
        }
    }
    return (1 - totalAngularWeight) * sum1 / sum2 + totalAngularWeight * sum3 / sum4;
}

double ErrorMetric::ndv(const Pixel& q, const valarray<int16_t>& ndviIndices) {
    double L1 = q.radiances[ndviIndices[0] - 1];
    double L2 = q.radiances[ndviIndices[1] - 1];
    double F1 = q.solarIrradiances[ndviIndices[0] - 1];
    double F2 = q.solarIrradiances[ndviIndices[1] - 1];

    if (L1 == Constants::FILL_VALUE_DOUBLE || L2 == Constants::FILL_VALUE_DOUBLE || F1 == Constants::FILL_VALUE_DOUBLE || F2 == Constants::FILL_VALUE_DOUBLE) {
        return 0.5;
    }

    return ((L2 / F2) - (L1 / F1)) / ((L2 / F2) + (L1 / F1));
}
