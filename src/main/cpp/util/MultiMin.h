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

#ifndef MULTIMIN_H_
#define MULTIMIN_H_

#include <algorithm>
#include <cmath>

#include "Min.h"
#include "MultivariateFunction.h"
#include "UnivariateFunction.h"

/**
 * A utility class for minimizing an univariate function along a straight line.
 */
template<class T>
class LineMinimizer: private UnivariateFunction {
public:
	LineMinimizer(T* obj, double(T::*f)(valarray<double>&), const valarray<double>& p, valarray<valarray<double> >& u) :
			obj(obj), f(f), p(p), u(u), point(p.size()) {
	}

	~LineMinimizer() {
	}

	double findMinimum(valarray<double>& x, size_t k, size_t begin, size_t end, double accuracyGoal);
    
private:
	double getValue(double x);

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

/**
 * A utility class for minimizing multivariate functions.
 */
class MultiMin {
public:
	virtual ~MultiMin();

	static bool chol2D(valarray<double>& c, valarray<double>& b, valarray<valarray<double> >& a,
			const valarray<double>& data, size_t begin, size_t end, const valarray<bool>& validMask,
			const valarray<double>& w,
			const valarray<double>& model1,
			const valarray<double>& model2);

	/**
	 * Finds the minimum of a multivariate function using Powell's method.
	 * <p/>
	 * Based on online material provided by John H. Mathews (Department of Mathematics,
	 * California State Univ. Fullerton).
	 *
	 * @param f            the multivariate function.
	 * @param pn           the initial guess of the minimum. On return contains the
	 *                     minimum found. The length of this array must be equal to
	 *                     the number of variables in f.
	 * @param u            the initial direction set. On return contains the actual
	 *                     direction set. The number of directions must be equal to
	 *                     the number of variables in f.
	 * @param accuracyGoal the prescribed accuracy goal.
	 * @param maxIter      the maximum number of iterations.
	 *
	 * @return true on success.
	 */
	static bool powell(MultivariateFunction& f, valarray<double>& pn, valarray<valarray<double> >& u, const double accuracyGoal, const size_t maxIter);

	/**
	 * Finds the minimum of an object's multivariate member function using Powell's method.
	 * <p/>
	 * Based on online material provided by John H. Mathews (Department of Mathematics,
	 * California State Univ. Fullerton).
	 * 
     * @param obj           the object.
	 * @param f             the multivariate member function.
	 * @param lineMinimizer the line minimizer.
     * @param begin         the index of the first variable.
     * @param end           the index of the last variable, exclusive.
	 * @param pn            the initial guess of the minimum. On return contains the
	 *                      minimum found. The length of this array must be equal to
	 *                      the number of variables in f.
	 * @param u             the initial direction set. On return contains the actual
	 *                      direction set. The number of directions must be equal to
	 *                      the number of variables in f.
	 * @param accuracyGoal  the prescribed accuracy goal.
	 * @param maxIter       the maximum number of iterations.
	 *
	 * @return true on success.
	 */
	template<class T>
	static bool powell(T* obj, double(T::*f)(valarray<double>&), LineMinimizer<T>& lineMinimizer, size_t begin, size_t end, valarray<double>& pn, valarray<double>& p0, valarray<double>& pe,
                       valarray<valarray<double> >& u, const double accuracyGoal = DEFAULT_ACCURACY_GOAL, const size_t maxIter = 100);

	static double square(double x) {
		return x * x;
	}

	static const double DEFAULT_ACCURACY_GOAL = 1.0E-4;

private:
	MultiMin();
};


template<class T>
double LineMinimizer<T>::findMinimum(valarray<double>& x, size_t k, size_t begin, size_t end, double accuracyGoal) {
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

template<class T>
double LineMinimizer<T>::getValue(double x) {
    const valarray<double>& v = u[k];
    for (size_t i = begin; i < end; i++) {
        point[i] = p[i] + x * v[i];
    }
    
    return (obj->*f)(point);
}

template<class T>
bool MultiMin::powell(T* obj, double(T::*f)(valarray<double>&), LineMinimizer<T>& lineMinimizer, size_t begin, size_t end, valarray<double>& pn, valarray<double>& p0, valarray<double>& pe,
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
            const double z = lineMinimizer.findMinimum(pn, k, begin, end, accuracyGoal);
            const double d = abs(z - zn);
            if (d > maxDecrease) {
                maxIndex = k;
                maxDecrease = d;
            }
            zn = z;
        }
        // 4. Stop if accuracy goal is reached
        if (2.0 * (z0 - zn) <= accuracyGoal * (abs(z0) + abs(zn)) + 1.0e-20) {
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
                zn = lineMinimizer.findMinimum(pn, maxIndex, begin, end, accuracyGoal);
            }
        }
    }
    
    return false;
}

#endif /* MULTIMIN_H_ */
