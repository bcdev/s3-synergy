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

#ifndef GRID_H
#define	GRID_H

#include <cstddef>
#include <stdexcept>
#include <valarray>

using std::out_of_range;

/**
 * Interface, which represents (a slice of) the OLCI L1C grid. The OLCI L1C
 * grid dimensions are the camera index <em>k</em>, the row index <em>l</em>,
 * and the column index <em>m</em>.
 */
class Grid {
public:

	/**
	 * Destructor.
	 */
	virtual ~Grid() {
	}

	/**
	 * Returns the row index of the first row in memory.
	 * @return the row index of the first row in memory.
	 */
	virtual long getMinInMemoryL() const = 0;

	/**
	 * Returns the row index of the last row in memory.
	 * @return the row index of the last row in memory.
	 */
	virtual long getMaxInMemoryL() const = 0;

	/**
	 * Returns the size of the camera index dimension.
	 * @return the size of the camera index dimension.
	 */
	virtual long getSizeK() const = 0;

	/**
	 * Returns the number of rows in memory.
	 * @return the number of rows in memory.
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
	 * Returns the plain index for given indexes (<em>k</em>, <em>l</em>, <em>m</em>).
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

	/**
	 * Returns the camera index for a given plain index.
	 * @param index The plain index.
	 * @return the camera index.
	 */
    virtual long getK(size_t index) const = 0;

	/**
	 * Returns the row index for a given plain index.
	 * @param index The plain index.
	 * @return the row index.
	 */
    virtual long getL(size_t index) const = 0;

	/**
	 * Returns the column index for a given plain index.
	 * @param index The plain index.
	 * @return the column index.
	 */
    virtual long getM(size_t index) const = 0;

	/**
	 * Returns if the position indicated by the given indices is valid for this grid.
	 * @return true, if the position is valid, false otherwise.
	 */
	virtual bool isValidPosition(long k, long l, long m) const = 0;
};

#endif	/* GRID_H */

