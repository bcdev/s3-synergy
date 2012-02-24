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

#ifndef UNIVARIATEFUNCTION_H_
#define UNIVARIATEFUNCTION_H_

/**
 * Interface for real-valued functions of a single variable.
 */
class UnivariateFunction {
public:
    /**
     * Returns the value of the univariate function at a given argument.
     *
     * @param x The argument.
     *
     * @return The value of the univariate function at the given argument.
     */
    virtual double getValue(double x) = 0;

    virtual ~UnivariateFunction() {
    }

};

#endif /* UNIVARIATEFUNCTION_H_ */
