/*
 * MultiMin.cpp
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#include "MultiMin.h"

MultiMin::MultiMin() {

}

MultiMin::~MultiMin() {
}

bool MultiMin::powell(MultivariateFunction& f, valarray<double>& pn, valarray<valarray<double> >& u, const double accuracyGoal, const size_t maxIter) {
	const size_t n = pn.size();
    
	LineMinimizer<MultivariateFunction> lineMinimizer(&f, &MultivariateFunction::getValue, pn, u);
	valarray<double> p0(n);
	valarray<double> pe(n);
    
	return powell(&f, &MultivariateFunction::getValue, lineMinimizer, 0, n, pn, p0, pe, u, accuracyGoal, maxIter);
}
