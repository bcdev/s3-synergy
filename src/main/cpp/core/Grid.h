/* 
 * Copyright (C) 2010 by Brockmann Consult (info@brockmann-consult.de)
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
 *
 * File:   Grid.h
 * Author: ralf
 *
 * Created on December 16, 2010, 3:17 PM
 */

#ifndef GRID_H
#define	GRID_H

#include <cstddef>
#include <stdexcept>
#include <valarray>

using std::out_of_range;

/**
 * Interface, which represents (a slice of) the OLCI L1C grid. The OLCI L1C
 * grid dimensions are the camera index {@code k}, the row index {@code l},
 * and the column index {@code c}.
 */
class Grid {
public:

	/**
	 * Destructor.
	 */
	virtual ~Grid() {
	}

	/**
	 * Returns the first value of the camera index dimension.
	 * @return the first value of the camera index dimension.
	 */
	virtual long getFirstK() const = 0;

	/**
	 * Returns the first value of the row index dimension.
	 * @return the first value of the row index dimension.
	 */
	virtual long getFirstL() const = 0;

	/**
	 * Returns the last value of the row index dimension.
	 * @return the last value of the row index dimension.
	 */
	virtual long getLastL() const = 0;

	/**
	 * Returns the first value of the column index dimension.
	 * @return the first value of the column index dimension.
	 */
	virtual long getFirstM() const = 0;

	/**
	 * Returns the size of the camera index dimension.
	 * @return the size of the camera index dimension.
	 */
	virtual long getSizeK() const = 0;

	/**
	 * Returns the size of the row index dimension.
	 * @return the size of the row index dimension.
	 */
	virtual long getSizeL() const = 0;

	/**
	 * Returns the size of the column index dimension.
	 * @return the size of the column index dimension.
	 */
	virtual long getSizeM() const = 0;

	/**
	 * Returns the plain size of the grid in terms of the the total number of
	 * grid points.
	 * @return the plain size of the grid.
	 */
	virtual long getSize() const = 0;

	/**
	 * Returns the stride for the camera index dimension.
	 * @return the stride for the camera index dimension.
	 */
	virtual long getStrideK() const = 0;

	/**
	 * Returns the stride for the row index dimension.
	 * @return the stride for the row index dimension.
	 */
	virtual long getStrideL() const = 0;

	/**
	 * Returns the stride for the column index dimension.
	 * @return the stride for the column index dimension.
	 */
	virtual long getStrideM() const = 0;

	/**
	 * Returns the plain index for given coordinates ({@code k}, {@code l},
	 * {@code m}).
	 * @param k The camera index.
	 * @param l The row index.
	 * @param m The column index.
	 * @return the plain index.
	 */
	virtual size_t getIndex(long k, long l, long m) const throw (out_of_range) = 0;

	/**
	 * Returns the minimum valid value of the camera index dimension.
	 * @return the minimum valid value of the camera index dimension.
	 */
	virtual long getMinK() const = 0;

	/**
	 * Returns the maximum valid value of the camera index dimension.
	 * @return the maximum valid value of the camera index dimension.
	 */
	virtual long getMaxK() const = 0;

	/**
	 * Returns the minimum valid value of the row index dimension.
	 * @return the minimum valid value of the row index dimension.
	 */
	virtual long getMinL() const = 0;

	/**
	 * Returns the maximum valid value of the row index dimension.
	 * @return the maximum valid value of the row index dimension.
	 */
	virtual long getMaxL() const = 0;

	/**
	 * Returns the minimum valid value of the column index dimension.
	 * @return the minimum valid value of the column index dimension.
	 */
	virtual long getMinM() const = 0;

	/**
	 * Returns the maximum valid value of the column index dimension.
	 * @return the maximum valid value of the column index dimension.
	 */
	virtual long getMaxM() const = 0;

    virtual long getK(size_t index) const = 0;

    virtual long getL(size_t index) const = 0;

    virtual long getM(size_t index) const = 0;

	/**
	 * Returns if the position indicated by the given indices is valid for this grid.
	 * @return true, if the position is valid, false otherwise.
	 */
	virtual bool isValidPosition(long k, long l, long m) const = 0;
};

#endif	/* GRID_H */

