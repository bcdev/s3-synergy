/*
 * MultiMin.h
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#ifndef MULTIMIN_H_
#define MULTIMIN_H_

#include <algorithm>

#include "Min.h"
#include "MultivariateFunction.h"
#include "UnivariateFunction.h"

template<class T>
class LineMinimizer: private UnivariateFunction {
public:
	LineMinimizer(T* obj, double(T::*f)(valarray<double>&), const valarray<double>& p, valarray<valarray<double> >& u) :
			obj(obj), f(f), p(p), u(u), point(p.size()) {
	}

	~LineMinimizer() {
	}

	double findMinimum(valarray<double>& x, size_t k, size_t begin, size_t end) {
		this->k = k;
		this->begin = begin;
		this->end = end;

		Min::brack(*this, 0.0, 1.0, bracket);
		Min::brent(*this, bracket);

		const valarray<double>& v = u[k];
		for (size_t i = begin; i < end; ++i) {
			x[i] = p[i] + v[i] * bracket.minimumX;
		}

		return bracket.minimumF;
	}

private:
	double getValue(double x) {
		const valarray<double>& v = u[k];
		for (size_t i = begin; i < end; i++) {
			point[i] = p[i] + x * v[i];
		}

		return (obj->*f)(point);
	}

	T* obj;
	double(T::*f)(valarray<double>&);
	const valarray<double>& p;
	const valarray<valarray<double> >& u;

	valarray<double> point;
	Bracket bracket;
	size_t k;
	size_t begin;
	size_t end;
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

	template<class T>
	static bool powell(T* obj, double(T::*f)(valarray<double>&), LineMinimizer<T>& lineMinimizer, size_t begin, size_t end, valarray<double>& pn, valarray<double>& p0, valarray<double>& pe,
			valarray<valarray<double> >& u, const double accuracyGoal, const size_t maxIter) {
		using std::abs;
		using std::copy;

		double zn = (obj->*f)(pn);

		for (size_t i = 0; i < maxIter; ++i) {
			// 1. Initialize
			copy(&pn[begin], &pn[end], &p0[begin]);
			double z0 = zn;
			// 2. Successively minimize along all directions
			// 3. Remember magnitude and direction of maximum decrease
			double maxDecrease = 0.0;
			size_t maxIndex = 0;
			for (size_t k = begin; k < end; ++k) {
				const double z = lineMinimizer.findMinimum(pn, k, begin, end);
				const double d = abs(z - zn);
				if (d > maxDecrease) {
					maxIndex = k;
					maxDecrease = d;
				}
				zn = z;
			}
			// 4. Stop if ||pn - p0|| < accuracyGoal
			double sum = 0.0;
			for (size_t k = begin; k < end; ++k) {
				sum += square(pn[k] - p0[k]);
			}
			if (sum < square(accuracyGoal)) {
				return true;
			}
			// 5. Extrapolate
			for (size_t k = begin; k < end; ++k) {
				pe[k] = 2.0 * pn[k] - p0[k];
			}
			const double ze = (obj->*f)(pe);
			// 6. When necessary, discard the direction of maximum decrease
			if (ze < z0) {
				if (2.0 * (z0 - 2.0 * zn + ze) * square(z0 - zn - maxDecrease) < maxDecrease * square(z0 - ze)) {
					valarray<double>& innerU = u[maxIndex];
					for (size_t k = begin; k < end; ++k) {
						innerU[k] = pn[k] - p0[k];
					}
					zn = lineMinimizer.findMinimum(pn, maxIndex, begin, end);
				}
			}
		}

		return false;
	}

	static double square(double x) {
		return x * x;
	}

private:
	MultiMin();
};

#endif /* MULTIMIN_H_ */
