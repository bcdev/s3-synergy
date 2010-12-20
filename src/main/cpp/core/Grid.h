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
     * Returns the start value of the camera index dimension.
     * @return the start value of the camera index dimension.
     */
    virtual size_t getStartK() const = 0;

    /**
     * Returns the start value of the row index dimension.
     * @return the start value of the row index dimension.
     */
    virtual size_t getStartL() const = 0;

    /**
     * Sets the start value of the row index dimension to a new value.
     * @param l The new start value of the row index dimension.
     */
    virtual void setStartL(size_t l) = 0;

    /**
     * Returns the start value of the column index dimension.
     * @return the start value of the column index dimension.
     */
    virtual size_t getStartM() const = 0;

    /**
     * Returns the size of the camera index dimension.
     * @return the size of the camera index dimension.
     */
    virtual size_t getSizeK() const = 0;

    /**
     * Returns the size of the row index dimension.
     * @return the size of the row index dimension.
     */
    virtual size_t getSizeL() const = 0;

    /**
     * Returns the size of the column index dimension.
     * @return the size of the column index dimension.
     */
    virtual size_t getSizeM() const = 0;

    /**
     * Returns the plain size of the grid in terms of the the total number of
     * grid points.
     * @return the plain size of the grid.
     */
    virtual size_t getSize() const = 0;
    
    /**
     * Returns the stride for the camera index dimension.
     * @return the stride for the camera index dimension.
     */
    virtual size_t getStrideK() const = 0;

    /**
     * Returns the stride for the row index dimension.
     * @return the stride for the row index dimension.
     */
    virtual size_t getStrideL() const = 0;

    /**
     * Returns the stride for the column index dimension.
     * @return the stride for the column index dimension.
     */
    virtual size_t getStrideM() const = 0;

    /**
     * Returns the plain index for given coordinates ({@code k}, {@code l},
     * {@code m}).
     * @param k The camera index.
     * @param l The row index.
     * @param m The column index.
     * @return the plain index.
     */
    virtual size_t getIndex(size_t k, size_t l, size_t m) const throw (out_of_range) = 0;
};

#endif	/* GRID_H */
