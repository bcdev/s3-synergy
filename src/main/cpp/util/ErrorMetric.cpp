/*
 * ErrorMetric.cpp
 *
 *  Created on: 13.10.2011
 *      Author: thomasstorm
 */

#include "ErrorMetric.h"

ErrorMetric::ErrorMetric(AerPixel& p, float gamma, int16_t amin, valarray<float> totalAngularWeights, valarray<float> vegetationSpectrum,
        valarray<float> soilReflectance, valarray<int16_t> ndviIndices, matrix<float> angularWeights) :
        p(p), gamma(gamma), amin(amin), totalAngularWeights(totalAngularWeights), vegetationSpectrum(vegetationSpectrum),
        soilReflectance(soilReflectance), ndviIndices(ndviIndices), angularWeights(angularWeights) {
}

double ErrorMetric::value(valarray<double>& x) {
    // todo - ts 12Oct2011 - clarify what is meant by index NDV(p) (see DPM)
    for (size_t i = 1; i <= 30; i++) {
        if (p.isFillValue("L_" + lexical_cast<string>(i))) {
            spectralWeights[i - 1] = 0;
            if (i >= 19 && i <= 24) {
                angularWeights.insert_element(i - 19, 0, 0.0);
            } else if (i >= 25 && i <= 30) {
                angularWeights.insert_element(i - 25, 1, 0.0);
            }
        }
    }
    applyAtmosphericCorrection(p, amin);

    valarray<float> rSpec(30);
    valarray<float> rAng(12);
    for (size_t i = 0; i < 30; i++) {
        rSpec[i] = specModelSurf(p.c_1, p.c_2, i);
    }
    for (size_t i = 0; i < 12; i++) {
        rAng[i] = angModelSurf(i, p);
    }
    return errorMetric(rSpec, rAng);
}

void ErrorMetric::applyAtmosphericCorrection(AerPixel& p, int16_t amin) {

}

float ErrorMetric::angModelSurf(size_t index, AerPixel& p) {
    // todo - ts - 13Oct2011 - clarify how to get D
    float D = 0.0;
    size_t j = index % 6;
    size_t o = index < 6 ? 0 : 1;
    float g = (1 - gamma) * p.omega[j];
    return (1 - D) * p.nu[o] * p.omega[j] + (gamma * p.omega[j]) / (1 - g) * (D + g * (1 - D));
}

float ErrorMetric::specModelSurf(double c_1, double c_2, size_t index) {
    return c_1 * vegetationSpectrum[index] + c_2 * soilReflectance[index];
}

float ErrorMetric::errorMetric(valarray<float> rSpec, valarray<float> rAng) {
    // todo - ts - 13Oct2011 - clarify if ndvi is correctly used as array index here
    double ndvi = ndv(p, ndviIndices);
    double sum1 = 0.0;
    double sum2 = 0.0;
    double sum3 = 0.0;
    double sum4 = 0.0;
    for (size_t i = 0; i < 30; i++) {
        sum1 += spectralWeights[i] * (p.getSDR(i) - rSpec[i]) * (p.getSDR(i) - rSpec[i]);
    }
    for (size_t i = 0; i < 30; i++) {
        sum2 += spectralWeights[i];
    }
    for (size_t i = 0; i < 12; i++) {
        size_t xIndex = i < 6 ? 0 : 1;
        size_t yIndex = i % 6;
        sum3 += angularWeights.at_element(xIndex, yIndex) * (p.getSDR(i) - rAng[i]) * (p.getSDR(i) - rAng[i]);
    }
    for (size_t i = 0; i < 12; i++) {
        size_t xIndex = i < 6 ? 0 : 1;
        size_t yIndex = i % 6;
        sum4 += angularWeights.at_element(xIndex, yIndex);
    }
    return (1 - totalAngularWeights[ndvi]) * sum1 / sum2 + totalAngularWeights[ndvi] * sum3 / sum4;
}

double ErrorMetric::ndv(AerPixel& q, valarray<int16_t> ndviIndices) {
    double L1 = q.getRadiance(ndviIndices[0]);
    double L2 = q.getRadiance(ndviIndices[1]);
    double F1 = q.solarIrradiances[ndviIndices[0]];
    double F2 = q.solarIrradiances[ndviIndices[1]];
    if (q.isFillValue("L_" + lexical_cast<string>(ndviIndices[0])) ||
            q.isFillValue("L_" + lexical_cast<string>(ndviIndices[1])) ||
            isSolarIrradianceFillValue(F1, q.solarIrradianceFillValues, ndviIndices[0]) ||
            isSolarIrradianceFillValue(F2, q.solarIrradianceFillValues, ndviIndices[1])) {
        return 0.5;
    }
    return ((L2 / F2) - (L1 / F1)) / ((L2 / F2) + (L1 / F1));
}

bool ErrorMetric::isSolarIrradianceFillValue(double f, const valarray<double> fillValues, int16_t index) {
    if (index >= 18) {
        return false;
    }
    return f == fillValues[index];
}
