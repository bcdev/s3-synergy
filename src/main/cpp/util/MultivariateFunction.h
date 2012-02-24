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

#ifndef MULTIVARIATEFUNCTION_H_
#define MULTIVARIATEFUNCTION_H_

#include <valarray>

using std::valarray;

/**
 * Interface for real-valued functions of several variables.
 */
class MultivariateFunction {
public:

	/**
	 * Returns the value of the multivariate function at a given argument.
	 *
	 * @param x The argument.
	 *
	 * @return The value of the multivariate function at a given argument.
	 */
	virtual double getValue(valarray<double>& x) = 0;

	virtual ~MultivariateFunction() {
	}
};

#endif /* MULTIVARIATEFUNCTION_H_ */
