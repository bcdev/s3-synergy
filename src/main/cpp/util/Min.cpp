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

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>

#include "Min.h"

using std::abs;
using std::invalid_argument;
using std::min;
using std::numeric_limits;
using std::sqrt;
using std::string;

static const double EPS = sqrt(numeric_limits<double>::epsilon());
static const double GOLDEN = 0.3819660;

Min::Min() {

}

Min::~Min() {
}

Bracket& Min::brack(UnivariateFunction& function, double a, double b, Bracket& bracket) {
    double lowerX = a;
    double lowerF = function.getValue(a);

    double minimumX = b;
    double minimumF = function.getValue(b);

    if (minimumF > lowerF) {
        const double lx = lowerX;
        const double lf = lowerF;

        lowerX = minimumX;
        lowerF = minimumF;

        minimumX = lx;
        minimumF = lf;
    }

    double upperX = minimumX + (minimumX - lowerX) * (1.0 / GOLDEN - 1.0);
    double upperF = function.getValue(upperX);

    while (minimumF > upperF) {
        upperX = upperX + (upperX - minimumX) * (1.0 / GOLDEN - 1.0);
        upperF = function.getValue(upperX);
    }

    bracket.minimumX = minimumX;
    bracket.minimumF = minimumF;

    if (lowerX > upperX) {
        bracket.lowerX = upperX;
        bracket.lowerF = upperF;
        bracket.upperX = lowerX;
        bracket.upperF = lowerF;
    } else {
        bracket.lowerX = lowerX;
        bracket.lowerF = lowerF;
        bracket.upperX = upperX;
        bracket.upperF = upperF;
    }
    return bracket;
}

bool Min::brent(UnivariateFunction& f, Bracket& bracket, double relativeAccuracyGoal, double absoluteAccuracyGoal, int maxIter) {
    double u;
    double v = bracket.lowerX + GOLDEN * (bracket.upperX - bracket.lowerX);
    double w = v;

    double d = 0.0;
    double e = 0.0;

    double fu;
    double fv = f.getValue(v);
    double fw = fv;

    for (int i = 0; i < maxIter; ++i) {
        const double a = bracket.lowerX;
        const double b = bracket.upperX;
        const double z = bracket.minimumX;

        const double fz = bracket.minimumF;

        const double lowerW = (z - a);
        const double upperW = (b - z);
        const double tolerance = EPS * abs(z);

        const double midpoint = 0.5 * (a + b);

        double p = 0.0;
        double q = 0.0;
        double r = 0.0;

        if (abs(e) > tolerance) {
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
        if (abs(p) < abs(0.5 * q * r) && p < q * lowerW && p < q * upperW) {
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
        if (abs(d) >= tolerance) {
            u = z + d;
        } else {
            u = z + ((d > 0.0) ? tolerance : -tolerance);
        }

        fu = f.getValue(u);

        if (fu <= fz) {
            if (u < z) {
                bracket.upperX = z;
                bracket.upperF = fz;
            } else {
                bracket.lowerX = z;
                bracket.lowerF = fz;
            }

            v = w;
            w = z;
            fv = fw;
            fw = fz;

            bracket.minimumX = u;
            bracket.minimumF = fu;
        } else {
            if (u < z) {
                bracket.lowerX = u;
                bracket.lowerF = fu;
            } else {
                bracket.upperX = u;
                bracket.upperF = fu;
            }
        }
        if (testInterval(bracket.lowerX, bracket.upperX, absoluteAccuracyGoal, relativeAccuracyGoal)) {
            return true;
        }
    }

    return false;
}

bool Min::testInterval(double lowerX, double upperX, double absoluteAccuracyGoal, double relativeAccuracyGoal) {
    double minAbs;
    if ((lowerX > 0.0 && upperX > 0.0) || (lowerX < 0.0 && upperX < 0.0)) {
        minAbs = min(abs(lowerX), abs(upperX));
    } else {
        minAbs = 0.0;
    }

    return abs(upperX - lowerX) < absoluteAccuracyGoal + relativeAccuracyGoal * minAbs;
}

Bracket::Bracket(double lowerX, double upperX, UnivariateFunction& function) {
    if (lowerX > upperX) {
        this->lowerX = upperX;
        this->upperX = lowerX;
    } else {
        this->lowerX = lowerX;
        this->upperX = upperX;
    }

    lowerF = function.getValue(this->lowerX);
    upperF = function.getValue(this->upperX);

    minimumX = this->lowerX + GOLDEN * (this->upperX - this->lowerX);
    minimumF = function.getValue(this->minimumX);
}
