/*
 * Min.h
 *
 *  Created on: 28.09.2011
 *      Author: s3-syn
 */

#ifndef MIN_H_
#define MIN_H_

#include "../core/Boost.h"
#include "UnivariateFunction.h"

class Bracket;

class Min {
public:

	virtual ~Min();

	/**
	 * Brackets a minimum of an univariate function given two initial abscissa
	 * values.
	 *
	 * @param f       the univariate function.
	 * @param a       the lower initial abscissa value.
	 * @param b       the upper initial abscissa value.
	 * @param bracket the bracket found.
	 *
	 * @return the bracket found.
	 */
	static Bracket& brack(UnivariateFunction& f, double a, double b, Bracket& bracket);

	/**
	 * Finds the minimum of an univariate function using Brent's algorithm.
	 * <p/>
	 * Based on code provided by the GNU Scientific Library (GSL).
	 *
	 * @param f                    the univariate function.
	 * @param bracket              the bracket for the minimum being searched.
	 * @param relativeAccuracyGoal the relative accuracy goal for the minimum being searched.
	 *
	 * @return {@code true} on success.
	 *
	 * @throws invalid_argument if the {@code bracket} is invalid.
	 */
	static bool brent(UnivariateFunction& f, Bracket& bracket, double relativeAccuracyGoal);

	/**
	 * Finds the minimum of an univariate function using Brent's algorithm.
	 * <p/>
	 * Based on code provided by the GNU Scientific Library (GSL).
	 *
	 * @param f                    the univariate function.
	 * @param bracket              the bracket for the minimum being searched.
	 * @param relativeAccuracyGoal the relative accuracy goal for the minimum being searched.
	 * @param absoluteAccuracyGoal the absolute accuracy goal for the minimum being searched.
	 *
	 * @return {@code true} on success.
	 *
	 * @throws invalid_argument if the {@code bracket} is invalid.
	 */
	static bool brent(UnivariateFunction& f, Bracket& bracket, double relativeAccuracyGoal, double absoluteAccuracyGoal);

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
	static bool brent(UnivariateFunction& f, Bracket& bracket, double relativeAccuracyGoal, double absoluteAccuracyGoal, int maxIter);

	static const double GOLDEN;
	static const double ACCURACY_GOAL;

private:

	Min();

	static bool testInterval(double lowerX, double upperX, double absoluteAccuracyGoal, double relativeAccuracyGoal);
	static double computeEpsilonSqareRoot();
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
