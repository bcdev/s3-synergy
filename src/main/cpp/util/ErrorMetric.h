/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
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

class ErrorMetric : private UnivariateFunction {

public:
    ErrorMetric(const Context& context);

    ~ErrorMetric();

    double computeErrorSurfaceCurvature(const Pixel& p);
    bool findMinimum(Pixel& p);

private:
    void setAerosolOpticalThickness(double aot);
    double computeNdvi(const Pixel& q) const;
    double computeRss2(valarray<double>& x);
    double computeRss8(valarray<double>& x);
    double computeRss10(valarray<double>& x);
    double getValue(double x);
    void setPixel(const Pixel& p);

    static double square(double x) {
    	return x * x;
    }

	const LookupTable<double>& lutOlcRatm;
    const LookupTable<double>& lutSlnRatm;
    const LookupTable<double>& lutSloRatm;
    const LookupTable<double>& lutT;
    const LookupTable<double>& lutRhoAtm;
    const LookupTable<double>& lutTotalAngularWeights;
    const LookupTable<double>& lutD;

    valarray<double> cO3;
	double initialAot;
	valarray<double> initialNus;
	valarray<double> initialOmegas;
    double gamma;
    valarray<int16_t> ndviIndices;
    valarray<double> vegetationModel;
    valarray<double> soilModel;
    valarray<double> spectralWeights;
    matrix<double> angularWeights;

	const Pixel* pixel;

    bool doOLC;
    bool doSLN;
    bool doSLS;
	double sum2;
    double sum8;
    double totalAngularWeight;    
    
    valarray<bool> validMask;
    valarray<double> sdrs;
    
    // for atmospheric correction
	valarray<double> coordinates;
	valarray<double> matRatmOlc;
	valarray<double> matRatmSln;
	valarray<double> matRatmSlo;
	valarray<double> matTv;
	valarray<double> matRho;
    valarray<double> diffuseFractions;
	valarray<double> lutWeights;
	valarray<double> lutWorkspace;

	// for minimization
	valarray<double> pn;
	valarray<double> p0;
	valarray<double> pe;
	valarray<valarray<double> > u;
	LineMinimizer<ErrorMetric> lineMinimizer;

	static const double ACCURACY_GOAL = 5.0e-4;
};

inline double ErrorMetric::computeRss10(valarray<double>& x) {
    return (1.0 - totalAngularWeight) * computeRss2(x) / sum2 + totalAngularWeight * computeRss8(x) / sum8;    
}


#endif /* ERRORMETRIC_H_ */
