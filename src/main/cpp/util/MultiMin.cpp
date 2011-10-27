/*
 * MultiMin.cpp
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#include <algorithm>
#include <stdexcept>
#include <string>

#include "../core/Boost.h"
#include "MultiMin.h"

using std::copy;
using std::invalid_argument;
using std::min;
using std::sqrt;
using std::string;

double LineMinimizer::findMinimum(valarray<double>& x, size_t k) {
	this->k = k;

	Min::brack(*this, 0.0, 1.0, bracket);
	Min::brent(*this, bracket);

	const valarray<double>& v = u[k];
	for (size_t i = 0; i < p.size(); ++i) {
		x[i] = p[i] + v[i] * bracket.minimumX;
	}

	return bracket.minimumF;
}

double LineMinimizer::getValue(double x) {
	const valarray<double>& v = u[k];
	for (size_t i = 0; i < p.size(); i++) {
		point[i] = p[i] + x * v[i];
	}

	return f.getValue(point);
}

MultiMin::MultiMin() {

}

MultiMin::~MultiMin() {
}

bool MultiMin::powell(MultivariateFunction& f, LineMinimizer& lineMinimizer, valarray<double>& pn, valarray<double>& p0, valarray<double>& pe, valarray<valarray<double> >& u, const double accuracyGoal, const size_t maxIter) {
	const size_t n = pn.size();
	double zn = f.getValue(pn);

	for (size_t i = 0; i < maxIter; ++i) {
		// 1. Initialize
		copy(&pn[0], &pn[n], &p0[0]);
		double z0 = zn;
		// 2. Successively minimize along all directions
		// 3. Remember magnitude and direction of maximum decrease
		double maxDecrease = 0.0;
		size_t maxIndex = 0;
		for (size_t k = 0; k < n; ++k) {
			const double z = lineMinimizer.findMinimum(pn, k);
			const double d = std::abs(z - zn);
			if (d > maxDecrease) {
				maxIndex = k;
				maxDecrease = d;
			}
			zn = z;
		}
		// 4. Stop if ||pn - p0|| < accuracyGoal
		double sum = 0.0;
		for (size_t k = 0; k < n; ++k) {
			sum += square(pn[k] - p0[k]);
		}
		if (sum < square(accuracyGoal)) {
			return true;
		}
		// 5. Extrapolate
		for (size_t k = 0; k < n; ++k) {
			pe[k] = 2.0 * pn[k] - p0[k];
		}
		const double ze = f.getValue(pe);
		// 6. When necessary, discard the direction of maximum decrease
		if (ze < z0) {
			if (2.0 * (z0 - 2.0 * zn + ze) * square(z0 - zn - maxDecrease) < maxDecrease * square(z0 - ze)) {
				for (size_t k = 0; k < n; ++k) {
					valarray<double>& innerU = u[maxIndex];
					innerU[k] = pn[k] - p0[k];
				}
				zn = lineMinimizer.findMinimum(pn, maxIndex);
			}
		}
	}

	return false;
}

bool MultiMin::powell(MultivariateFunction& f, valarray<double>& pn, valarray<valarray<double> >& u, const double accuracyGoal, const size_t maxIter) {
	const size_t n = pn.size();

	LineMinimizer lineMinimizer(f, pn, u);
	valarray<double> p0(n);
	valarray<double> pe(n);

	return powell(f, lineMinimizer, pn, p0, pe, u, accuracyGoal, maxIter);
}
