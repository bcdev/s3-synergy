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

    ErrorMetric(Pixel& p,int16_t amin, Context& context);

    double value(valarray<double>& x);

    static double ndv(Pixel& q, const valarray<int16_t>& ndviIndices);

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

    Pixel& p;

    double gamma;
    valarray<double> D;
    valarray<double> spectralWeights;
    valarray<double> vegetationSpectrum;
    valarray<double> soilReflectance;
    matrix<double> angularWeights;

    void applyAtmosphericCorrection(Pixel& p, int16_t amin);

    double angModelSurf(size_t index, valarray<double>& x);

    double specModelSurf(double c_1, double c_2, size_t index);

    double errorMetric(valarray<double> rSpec, valarray<double> rAng);

};

#endif /* ERRORMETRIC_H_ */
