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

#ifndef MIN_H_
#define MIN_H_

#include "UnivariateFunction.h"

class Bracket;


/**
 * A utility class for minimizing univariate functions.
 */
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
	 * @return true on success.
	 *
	 * @throws invalid_argument if the bracket is invalid.
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
