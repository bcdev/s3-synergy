/*
 * ErrorMetric.h
 *
 *  Created on: 13.10.2011
 *      Author: thomasstorm
 */

#ifndef ERRORMETRIC_H_
#define ERRORMETRIC_H_

#include <string>

#include "../core/Boost.h"
#include "../util/MultivariateFunction.h"
#include "AerPixel.h"

using std::string;

class ErrorMetric : public MultivariateFunction {

public:

    ErrorMetric(AerPixel& p, float gamma, int16_t amin, valarray<float> totalAngularWeights, valarray<float> vegetationSpectrum,
            valarray<float> soilReflectance, valarray<int16_t> ndviIndices, matrix<float> angularWeights);

    double value(valarray<double>& x);

private:
    AerPixel& p;
    float gamma;
    int16_t amin;
    valarray<float> spectralWeights;
    valarray<float> totalAngularWeights;
    valarray<float> vegetationSpectrum;
    valarray<float> soilReflectance;
    valarray<int16_t> ndviIndices;
    matrix<float> angularWeights;

    void applyAtmosphericCorrection(AerPixel& p, int16_t amin);

    float angModelSurf(size_t index, AerPixel& p);

    float specModelSurf(double c_1, double c_2, size_t index);

    float errorMetric(valarray<float> rSpec, valarray<float> rAng);

    double ndv(AerPixel& q, valarray<int16_t> ndviIndices);

    bool isSolarIrradianceFillValue(double f, const valarray<double> fillValues, int16_t index);
};

#endif /* ERRORMETRIC_H_ */
