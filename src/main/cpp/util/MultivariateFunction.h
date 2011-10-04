/*
 * MultivariateFunction.h
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#ifndef MULTIVARIATEFUNCTION_H_
#define MULTIVARIATEFUNCTION_H_

#include <valarray>

using std::valarray;

/**
 * Interface for real-valued functions of more than a single variable.
 */
class MultivariateFunction {
public:

    /**
     * Returns the value of the function at {@code x}.
     *
     * @param x The variables.
     *
     * @return The value of the function at {@code x}.
     */
    virtual double value(valarray<double>& x) = 0;

};

#endif /* MULTIVARIATEFUNCTION_H_ */
