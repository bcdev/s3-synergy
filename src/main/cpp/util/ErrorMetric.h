/*
 * ErrorMetric.h
 *
 *  Created on: 13.10.2011
 *      Author: thomasstorm
 */

#ifndef ERRORMETRIC_H_
#define ERRORMETRIC_H_

#include "../core/Boost.h"
#include "../core/Context.h"
#include "../core/Pixel.h"
#include "../core/LookupTable.h"

#include "AuxdataProvider.h"
#include "MultivariateFunction.h"
#include "UnivariateFunction.h"
#include "MultiMin.h"

class ErrorMetric : public UnivariateFunction {

public:

    ErrorMetric(const Context& context);

    const valarray<double>& getOptimizedParameters() {
    	return pn;
    }

    double getValue(double x);
    double computeSpectralRss(valarray<double>& x);
    double computeAngularRss(valarray<double>& x);

    void setPixel(const Pixel& p);

    double computeNdvi(const Pixel& q);

private:
	const Context& context;

	const MatrixLookupTable<double>& lutOlcRatm;
    const MatrixLookupTable<double>& lutSlnRatm;
    const MatrixLookupTable<double>& lutSloRatm;
    const MatrixLookupTable<double>& lutT;
    const MatrixLookupTable<double>& lutRhoAtm;
    const ScalarLookupTable<double>& lutTotalAngularWeights;
    const VectorLookupTable<double>& lutD;

    AuxdataProvider& configurationAuxdata;
    const double gamma;
    const valarray<int16_t>& ndviIndices;
    const valarray<double>& spectralWeights;
    const valarray<double>& vegetationSpectrum;
    const valarray<double>& soilReflectance;
    const matrix<double>& angularWeights;

	const Pixel* pixel;

	double sum2;
    double sum4;
    double totalAngularWeight;

    valarray<double> sdrs;
    valarray<double> rSpec;
    valarray<double> rAng;

    // for atmospheric correction
	valarray<double> coordinates;
	matrix<double> matRatmOlc;
	matrix<double> matRatmSln;
	matrix<double> matRatmSlo;
	matrix<double> matTs;
	matrix<double> matTv;
	matrix<double> matRho;
    valarray<double> diffuseFraction;
	valarray<double> f;
	valarray<double> w;

	// for minimization
	valarray<double> pn;
	valarray<double> p0;
	valarray<double> pe;
	valarray<valarray<double> > u;
	LineMinimizer<ErrorMetric> lineMinimizer;

    void computeAtmosphericCorrection(double tau550);

    double angModelSurf(size_t index, valarray<double>& x);

    double computeErrorMetric();
};

#endif /* ERRORMETRIC_H_ */
