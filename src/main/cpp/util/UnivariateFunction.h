/*
 * UnivariateFunction.h
 *
 *  Created on: 28.09.2011
 *      Author: s3-syn
 */

#ifndef UNIVARIATEFUNCTION_H_
#define UNIVARIATEFUNCTION_H_

/**
 * Interface for real-valued functions of a single variable.
 */
class UnivariateFunction {
public:
    /**
     * Returns the value of the function at {@code x}.
     *
     * @param x The variable.
     *
     * @return The value of the function at {@code x}.
     */
    virtual double value(double x) = 0;

};

#endif /* UNIVARIATEFUNCTION_H_ */
