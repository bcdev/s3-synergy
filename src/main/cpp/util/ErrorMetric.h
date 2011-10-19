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
#include "../core/Context.h"
#include "../util/MultivariateFunction.h"
#include "AerPixel.h"

using std::string;

class ErrorMetric : public MultivariateFunction {

public:

    ErrorMetric(AerPixel& p,int16_t amin, Context& context);

    double value(valarray<double>& x);

private:
    AerPixel& p;
    int16_t amin;
    Context& context;
    double gamma;
    valarray<double> spectralWeights;
    valarray<double> totalAngularWeights;
    valarray<double> vegetationSpectrum;
    valarray<double> soilReflectance;
    valarray<int16_t> ndviIndices;
    matrix<double> angularWeights;

    void applyAtmosphericCorrection(AerPixel& p, int16_t amin);

    float angModelSurf(size_t index, AerPixel& p);

    float specModelSurf(double c_1, double c_2, size_t index);

    float errorMetric(valarray<double> rSpec, valarray<double> rAng);

    double ndv(AerPixel& q, valarray<int16_t> ndviIndices);

    bool isSolarIrradianceFillValue(double f, const valarray<double> fillValues, int16_t index);
};

#endif /* ERRORMETRIC_H_ */
