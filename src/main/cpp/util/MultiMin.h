/*
 * MultiMin.h
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#ifndef MULTIMIN_H_
#define MULTIMIN_H_

#include "../core/Boost.h"
#include "Min.h"
#include "MultivariateFunction.h"
#include "UnivariateFunction.h"

class LineMinimizer: private UnivariateFunction {
public:
	LineMinimizer(MultivariateFunction& f, const valarray<double>& p, valarray<valarray<double> >& u) :
			f(f), p(p), u(u), point(p.size()) {
	}

	~LineMinimizer() {
	}

	double findMinimum(valarray<double>& x, size_t k);

private:
	double getValue(double x);

	MultivariateFunction& f;
	const valarray<double>& p;
	const valarray<valarray<double> >& u;

	valarray<double> point;
	Bracket bracket;
	size_t k;
};

class MultiMin {
public:
	virtual ~MultiMin();

	/**
	 * Finds the minimum of a multivariate function using Powell's method.
	 * <p/>
	 * Based on online material provided by John H. Mathews (Department of Mathematics,
	 * California State Univ. Fullerton).
	 *
	 * @param f            the multivariate function.
	 * @param pn           the initial guess of the minimum. On return contains the
	 *                     minimum found. The length of this array must be equal to
	 *                     the number of variables in {@code f}.
	 * @param u            the initial direction set. On return contains the actual
	 *                     direction set. The number of directions must be equal to
	 *                     the number of variables in {@code f}.
	 * @param accuracyGoal the prescribed accuracy goal.
	 * @param maxIter      the maximum number of iterations.
	 *
	 * @return {@code true} on success.
	 *
	 * @throws illegal_argument if the dimensions of {@code pn} and {@code u}
	 *         are not consistent.
	 */
	static bool powell(MultivariateFunction& f, valarray<double>& pn, valarray<valarray<double> >& u, const double accuracyGoal, const size_t maxIter);

	static bool powell(MultivariateFunction& f, LineMinimizer& lineMinimizer, valarray<double>& pn, valarray<double>& p0, valarray<double>& pe, valarray<valarray<double> >& u,
			const double accuracyGoal, const size_t maxIter);

	static double square(double x) {
		return x * x;
	}

private:
	MultiMin();
};

#endif /* MULTIMIN_H_ */
