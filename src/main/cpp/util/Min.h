/*
 * Min.h
 *
 *  Created on: 28.09.2011
 *      Author: s3-syn
 */

#ifndef MIN_H_
#define MIN_H_

#include "UnivariateFunction.h"

class Bracket;

class Min {
public:

	virtual ~Min();

	static Bracket& brack(UnivariateFunction& function, double a, double b, Bracket& bracket);

	/**
	 * Finds the minimum of an univariate function using Brent's algorithm.
	 * <p/>
	 * Based on code provided by the GNU Scientific Library (GSL).
	 *
	 * @param f                    the univariate function.
	 * @param bracket              the bracket for the minimum being searched.
	 * @param relativeAccuracyGoal the relative accuracy goal for the minimum being searched.
	 * @param absoluteAccuracyGoal the absolute accuracy goal for the minimum being searched.
	 * @param maxIter              the maximum number of iterations being performed.
	 *
	 * @return {@code true} on success.
	 *
	 * @throws invalid_argument if the {@code bracket} is invalid.
	 */
	static bool brent(UnivariateFunction& f, Bracket& bracket, double relativeAccuracyGoal = DEFAULT_RELATIVE_ACCURACY_GOAL, double absoluteAccuracyGoal = DEFAULT_ABSOLUTE_ACCURACY_GOAL, int maxIter = 100);

	static const double DEFAULT_ABSOLUTE_ACCURACY_GOAL = 1.0E-8;
	static const double DEFAULT_RELATIVE_ACCURACY_GOAL = 1.0E-4;

private:
	Min();

	static bool testInterval(double lowerX, double upperX, double absoluteAccuracyGoal, double relativeAccuracyGoal);
};

class Bracket {
public:

	Bracket() {
	}
	;
	Bracket(double lowerX, double upperX, UnivariateFunction& function);

	double lowerX;
	double upperX;
	double minimumX;

	double lowerF;
	double upperF;
	double minimumF;

};

#endif /* MIN_H_ */
