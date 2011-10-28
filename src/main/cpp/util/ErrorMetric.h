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
    double computeNdvi(const Pixel& q) const;
    void setPixel(const Pixel& p);


private:
    double computeRss2(valarray<double>& x);
    double computeRss8(valarray<double>& x);
    double computeRss10(valarray<double>& x);
    void applyAtmosphericCorrection(double tau550);

	const Context& context;

	const MatrixLookupTable<double>& lutOlcRatm;
    const MatrixLookupTable<double>& lutSlnRatm;
    const MatrixLookupTable<double>& lutSloRatm;
    const MatrixLookupTable<double>& lutT;
    const MatrixLookupTable<double>& lutRhoAtm;
    const ScalarLookupTable<double>& lutTotalAngularWeights;
    const VectorLookupTable<double>& lutD;
    const AuxdataProvider& configurationAuxdata;
    const double gamma;
    const valarray<int16_t>& ndviIndices;
    const valarray<double>& spectralWeights;
    const valarray<double>& vegetationSpectrum;
    const valarray<double>& soilReflectance;
    const matrix<double>& angularWeights;

	const Pixel* pixel;

    bool doOLC;
    bool doSLS;
	double sum2;
    double sum8;
    double totalAngularWeight;    
    
    valarray<double> sdrs;
    
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
	LineMinimizer<ErrorMetric> lineMinimizer2;
	LineMinimizer<ErrorMetric> lineMinimizer8;
};

inline double ErrorMetric::computeRss10(valarray<double>& x) {
    return (1.0 - totalAngularWeight) * computeRss2(x) / sum2 + totalAngularWeight * computeRss8(x) / sum8;    
}


#endif /* ERRORMETRIC_H_ */
