/*
 * MultiMin.cpp
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#include <limits>

#include "MultiMin.h"

MultiMin::MultiMin() {

}

MultiMin::~MultiMin() {
}

bool MultiMin::chol2D(valarray<double>& c, valarray<double>& b, valarray<valarray<double> >& a, const valarray<double>& data, size_t begin, size_t end, const valarray<bool>& validMask, const valarray<double>& w, const valarray<double>& model1, const valarray<double>& model2) {
	using std::fill;
	using std::sqrt;

	fill(&b[0], &b[2], 0.0);
	for (size_t i = 0; i < 2; ++i) {
		fill(&a[i][0], &a[i][2], 0.0);
	}

	// establish the normal equations
	for (size_t i = begin; i < end; ++i) {
		if (validMask[i]) {
			c[0] = model1[i];
			c[1] = model2[i];

			for (size_t j = 0; j < 2; ++j) {
				for (size_t k = j; k < 2; ++k) {
					a[j][k] += (c[j] * c[k]) * w[i];
				}
				b[j] += (data[i] * c[j]) * w[i];
			}
		}
	}
	// solve using Cholesky decomposition
	for (size_t i = 0; i < 2; ++i) {
		for (size_t j = i; j < 2; ++j) {
			double s = a[i][j];

			for (size_t k = 0; k < i; ++k) {
				s -= a[i][k] * a[j][k];
			}
			if (i < j) {
				a[j][i] = s / a[i][i];
			} else {
				if (s > 0.0) {
					a[i][i] = sqrt(s);
				} else { // normal equations are effectively singular
					return false;
				}
			}
		}
	}
	for (size_t i = 0; i < 2; ++i) {
		double s = b[i];

		for (size_t k = 0; k < i; ++k) {
			s -= a[i][k] * c[k];
		}
		c[i] = s / a[i][i];
	}
	for (size_t i = 1; i + 1 > 0; --i) {
		double s = c[i];

		for (size_t k = i + 1; k < 2; ++k) {
			s -= a[k][i] * c[k];
		}
		c[i] = s / a[i][i];
	}

	return true;
}

bool MultiMin::powell(MultivariateFunction& f, valarray<double>& pn, valarray<valarray<double> >& u, const double accuracyGoal, const size_t maxIter) {
	const size_t n = pn.size();
    
	LineMinimizer<MultivariateFunction> lineMinimizer(&f, &MultivariateFunction::getValue, pn, u);
	valarray<double> p0(n);
	valarray<double> pe(n);
    
	return powell(&f, &MultivariateFunction::getValue, lineMinimizer, 0, n, pn, p0, pe, u, accuracyGoal, maxIter);
}
