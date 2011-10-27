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
#include "../core/Pixel.h"
#include "../core/LookupTable.h"
#include "../util/MultivariateFunction.h"

using std::string;

class ErrorMetric : public MultivariateFunction {

public:

    ErrorMetric(const Pixel& p,int16_t amin, double tau550, Context& context);

    double getValue(valarray<double>& x);

    static double ndv(const Pixel& q, const valarray<int16_t>& ndviIndices);

private:
    const MatrixLookupTable<double>& lutOlcRatm;
    const MatrixLookupTable<double>& lutSlnRatm;
    const MatrixLookupTable<double>& lutSloRatm;
    const MatrixLookupTable<double>& lutT;
    const MatrixLookupTable<double>& lutRhoAtm;
    const ScalarLookupTable<double>& lutTotalAngularWeights;

    double sum2;
    double sum4;
    double totalAngularWeight;

    double gamma;
    valarray<double> D;
    valarray<double> spectralWeights;
    valarray<double> vegetationSpectrum;
    valarray<double> soilReflectance;
    valarray<double> sdrs;
    valarray<double> rSpec;
    valarray<double> rAng;
    matrix<double> angularWeights;

    void applyAtmosphericCorrection(const Pixel& p, int16_t amin);

    double angModelSurf(size_t index, valarray<double>& x);

    double specModelSurf(double c_1, double c_2, size_t index);

    double errorMetric();
};

#endif /* ERRORMETRIC_H_ */
