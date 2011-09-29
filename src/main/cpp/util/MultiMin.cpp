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

using std::invalid_argument;
using std::min;
using std::sqrt;
using std::string;

MultiMin::MultiMin() {

}

MultiMin::~MultiMin() {
}

bool MultiMin::powell(shared_ptr<MultivariateFunction> f, shared_ptr<valarray<double> > pn, valarray<shared_ptr<valarray<double> > > u, const double accuracyGoal, const size_t maxIter) const {
    const size_t n = pn->size();

    valarray<shared_ptr<LineMinimizer> > minimizers = valarray<shared_ptr<LineMinimizer> >(n);
    for (size_t k = 0; k < n; ++k) {
        minimizers[k] = shared_ptr<LineMinimizer>(new LineMinimizer(f, pn, u[k]));
    }

    valarray<double> p0 = valarray<double>(n);
    shared_ptr<valarray<double> > pe = shared_ptr<valarray<double> >(new valarray<double>(n));

    double zn = f->value(pn);

    for (size_t i = 0; i < maxIter; ++i) {
        // 1. Initialize
        for(size_t i = 0; i < n; i++) {
            p0[i] = (*pn)[i];
        }
        double z0 = zn;
        // 2. Successively minimize along all directions
        // 3. Remember magnitude and direction of maximum decrease
        double maxDecrease = 0.0;
        size_t maxIndex = 0;
        for (size_t k = 0; k < n; ++k) {
            const double z = minimizers[k]->findMinimum(pn);
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
            sum += square((*pn)[k] - p0[k]);
        }
        if (sum < square(accuracyGoal)) {
            return true;
        }
        // 5. Extrapolate
        for (size_t k = 0; k < n; ++k) {
            (*pe)[k] = 2.0 * (*pn)[k] - p0[k];
        }
        const double ze = f->value(pe);
        // 6. When necessary, discard the direction of maximum decrease
        if (ze < z0) {
            if (2.0 * (z0 - 2.0 * zn + ze) * square(z0 - zn - maxDecrease) < maxDecrease * square(z0 - ze)) {
                for (size_t k = 0; k < n; ++k) {
                    shared_ptr<valarray<double> > uPtr = u[maxIndex];
                    (*uPtr)[k] = (*pn)[k] - p0[k];
                }
                zn = minimizers[maxIndex]->findMinimum(pn);
            }
        }
    }

    return false;
}
