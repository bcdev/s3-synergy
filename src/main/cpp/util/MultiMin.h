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
     *                                   are not consistent.
     */
    static bool powell(MultivariateFunction& f, valarray<double>& pn, valarray<valarray<double> >& u, const double accuracyGoal, const size_t maxIter);

    static double square(double x) {
        return x * x;
    }

private:
    MultiMin();
};


/**
 * Builds an univariate function from the values of a multivariate
 * function along a straight line.
 */
class F1 : public UnivariateFunction {
public:
    F1(MultivariateFunction& f, const valarray<double>& p, const valarray<double>& u) : f(f), p(p), u(u), point(p.size()) {
    }

    double value(double x) {
        for(size_t i = 0; i < p.size(); i++) {
            point[i] = p[i] + x * u[i];
        }
        return f.value(point);
    }

private:
    MultivariateFunction& f;
    const valarray<double>& p;
    const valarray<double>& u;
    valarray<double> point;
};

/**
 * Finds the minimum of a multivariate function along a straight
 * line.
 */
class LineMinimizer {
public:
    /**
     * Constructs a new instance of this class.
     *
     * @param f the multivariate function.
     * @param p the starting point of the line.
     * @param u the direction of the line.
     */
    LineMinimizer(MultivariateFunction& g, const valarray<double>& p, const valarray<double>& u) : f(F1(g, p, u)), p(p), u(u) {
    }

    /**
     * Finds a minimum of {@code f} along the line defined by {@code p}
     * and {@code u}.
     *
     * @param x the minimum found.
     *
     * @return the value of {@code f} at the minimum found.
     */
    double findMinimum(valarray<double>& x) {
        Bracket bracket;
        Min::brack(f, 0.0, 1.0, bracket);
        Min::brent(f, bracket, 1.0E-6);

        for (size_t i = 0; i < p.size(); ++i) {
            x[i] = p[i] + u[i] * bracket.minimumX;
        }

        return bracket.minimumF;
    }

private:
    F1 f;
    const valarray<double>& p;
    const valarray<double>& u;
};

#endif /* MULTIMIN_H_ */
