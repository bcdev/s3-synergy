/*
 * Min.cpp
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#include <algorithm>
#include <stdexcept>
#include <string>

#include "../core/Boost.h"
#include "Min.h"

using std::invalid_argument;
using std::min;
using std::sqrt;
using std::string;

const double Min::GOLDEN = 0.3819660;

Min::Min() {
    // TODO Auto-generated constructor stub

}

Min::~Min() {
    // TODO Auto-generated destructor stub
}

shared_ptr<Bracket> Min::brack(const shared_ptr<UnivariateFunction> function, double a, double b, shared_ptr<Bracket> bracket) const {
    double lowerX = a;
    double lowerF = function->value(a);

    double minimumX = b;
    double minimumF = function->value(b);

    if (minimumF > lowerF) {
        const double lx = lowerX;
        const double lf = lowerF;

        lowerX = minimumX;
        lowerF = minimumF;

        minimumX = lx;
        minimumF = lf;
    }

    double upperX = minimumX + (minimumX - lowerX) * (1.0 / GOLDEN - 1.0);
    double upperF = function->value(upperX);

    while (minimumF > upperF) {
        upperX = upperX + (upperX - minimumX) * (1.0 / GOLDEN - 1.0);
        upperF = function->value(upperX);
    }

    bracket->minimumX = minimumX;
    bracket->minimumF = minimumF;

    if (lowerX > upperX) {
        bracket->lowerX = upperX;
        bracket->lowerF = upperF;
        bracket->upperX = lowerX;
        bracket->upperF = lowerF;
    } else {
        bracket->lowerX = lowerX;
        bracket->lowerF = lowerF;
        bracket->upperX = upperX;
        bracket->upperF = upperF;
    }
    return bracket;
}

bool Min::brent(shared_ptr<UnivariateFunction> function, shared_ptr<Bracket> bracket, double relativeAccuracyGoal) const {
    return brent(function, bracket, relativeAccuracyGoal, 1.0E-10);
}

bool Min::brent(shared_ptr<UnivariateFunction> function, shared_ptr<Bracket> bracket, double relativeAccuracyGoal, double absoluteAccuracyGoal) const {
    return brent(function, bracket, relativeAccuracyGoal, absoluteAccuracyGoal, 100);
}

bool Min::brent(shared_ptr<UnivariateFunction> f, shared_ptr<Bracket> bracket, double relativeAccuracyGoal, double absoluteAccuracyGoal, int maxIter) const {
    if (bracket->minimumF >= bracket->lowerF || bracket->minimumF >= bracket->upperF
            || (bracket->minimumX <= bracket->lowerX && bracket->minimumX <= bracket->upperX)
            || (bracket->minimumX >= bracket->lowerX && bracket->minimumX >= bracket->upperX)) {
        BOOST_THROW_EXCEPTION(invalid_argument("The points a = " + lexical_cast<string>(bracket->lowerX) + ", b = " + lexical_cast<string>(bracket->minimumX) + ", c = " + lexical_cast<string>(bracket->upperX) + " do not bracket a minimum."));
    }

    double u;
    double v = bracket->lowerX + GOLDEN * (bracket->upperX - bracket->lowerX);
    double w = v;

    double d = 0.0;
    double e = 0.0;

    double fu;
    double fv = f->value(v);
    double fw = fv;

    for (int i = 0; i < maxIter; ++i) {
        const double a = bracket->lowerX;
        const double b = bracket->upperX;
        const double z = bracket->minimumX;

        const double fz = bracket->minimumF;

        const double lowerW = (z - a);
        const double upperW = (b - z);
        const double tolerance = computeEpsilonSqareRoot() * std::abs(z);

        const double midpoint = 0.5 * (a + b);

        double p = 0.0;
        double q = 0.0;
        double r = 0.0;

        if (std::abs(e) > tolerance) {
            // fit parabola
            r = (z - w) * (fz - fv);
            q = (z - v) * (fz - fw);
            p = (z - v) * q - (z - w) * r;
            q = 2.0 * (q - r);

            if (q > 0.0) {
                p = -p;
            } else {
                q = -q;
            }

            r = e;
            e = d;
        }
        if (std::abs(p) < std::abs(0.5 * q * r) && p < q * lowerW && p < q * upperW) {
            const double t2 = 2.0 * tolerance;

            d = p / q;
            u = z + d;

            if ((u - a) < t2 || (b - u) < t2) {
                d = (z < midpoint) ? tolerance : -tolerance;
            }
        } else {
            e = (z < midpoint) ? b - z : -(z - a);
            d = GOLDEN * e;
        }
        if (std::abs(d) >= tolerance) {
            u = z + d;
        } else {
            u = z + ((d > 0.0) ? tolerance : -tolerance);
        }

        fu = f->value(u);

        if (fu <= fz) {
            if (u < z) {
                bracket->upperX = z;
                bracket->upperF = fz;
            } else {
                bracket->lowerX = z;
                bracket->lowerF = fz;
            }

            v = w;
            w = z;
            fv = fw;
            fw = fz;

            bracket->minimumX = u;
            bracket->minimumF = fu;
        } else {
            if (u < z) {
                bracket->lowerX = u;
                bracket->lowerF = fu;
            } else {
                bracket->upperX = u;
                bracket->upperF = fu;
            }
        }
        if (testInterval(bracket->lowerX, bracket->upperX, absoluteAccuracyGoal, relativeAccuracyGoal)) {
            return true;
        }
    }

    return false;
}

bool Min::testInterval(double lowerX, double upperX, double absoluteAccuracyGoal, double relativeAccuracyGoal) const {
    double minAbs;
    if ((lowerX > 0.0 && upperX > 0.0) || (lowerX < 0.0 && upperX < 0.0)) {
        minAbs = min(std::abs(lowerX), std::abs(upperX));
    } else {
        minAbs = 0.0;
    }

    return std::abs(upperX - lowerX) < absoluteAccuracyGoal + relativeAccuracyGoal * minAbs;
}

double Min::computeEpsilonSqareRoot() const {
    double r = 1.0;
    while (1.0 < (double) (1.0 + r)) {
        r /= 2.0;
    }
    r = 2.0 * r;
    return sqrt(r);
}

Bracket::Bracket(double lowerX, double upperX, shared_ptr<UnivariateFunction> function) {
    if (lowerX > upperX) {
        this->lowerX = upperX;
        this->upperX = lowerX;
    } else {
        this->lowerX = lowerX;
        this->upperX = upperX;
    }

    lowerF = function->value(this->lowerX);
    upperF = function->value(this->upperX);

    minimumX = this->lowerX + Min::GOLDEN * (this->upperX - this->lowerX);
    minimumF = function->value(this->minimumX);
}
;
