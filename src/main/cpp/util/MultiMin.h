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
    MultiMin();
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
    bool powell(shared_ptr<MultivariateFunction> f, valarray<double> pn, valarray<valarray<double> > u, const double accuracyGoal, const size_t maxIter) const;

    static double square(double x) {
        return x * x;
    }
};


/**
 * Builds an univariate function from the values of a multivariate
 * function along a straight line.
 */
class F1 : public UnivariateFunction {
public:
    F1(shared_ptr<MultivariateFunction> f, valarray<double> p, valarray<double> u) {
        this->f = f;
        this->p = p;
        this->u = u;
        point = valarray<double>(p.size());
    }

    double value(double x) {
        for(size_t i = 0; i < p.size(); i++) {
            point[i] = p[i] + x * u[i];
        }
        return f->value(point);
    }

private:
    shared_ptr<MultivariateFunction> f;
    valarray<double> p;
    valarray<double> u;
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
    LineMinimizer(shared_ptr<MultivariateFunction> f, valarray<double> p, valarray<double> u) {
        this->f = shared_ptr<F1>(new F1(f, p, u));
        this->p = p;
        this->u = u;

        this->bracket = shared_ptr<Bracket>(new Bracket());
    }

    /**
     * Finds a minimum of {@code f} along the line defined by {@code p}
     * and {@code u}.
     *
     * @param x the minimum found.
     *
     * @return the value of {@code f} at the minimum found.
     */
    double findMinimum(valarray<double> x) {
        shared_ptr<Min> min = shared_ptr<Min>(new Min());
        bracket = min->brack(f, 0.0, 1.0, bracket);
        min->brent(f, bracket, 1.0E-6);

        for (size_t i = 0; i < p.size(); ++i) {
            x[i] = p[i] + u[i] * bracket->minimumX;
        }

        return bracket->minimumF;
    }

private:
    shared_ptr<F1> f;
    valarray<double> p;
    valarray<double> u;
    shared_ptr<Bracket> bracket;
};

#endif /* MULTIMIN_H_ */
