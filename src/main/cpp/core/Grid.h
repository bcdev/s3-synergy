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

class Grid {
public:
    virtual ~Grid() {
    }

    virtual size_t getStartK() const = 0;
    virtual size_t getStartL() const = 0;
    virtual void setStartL(size_t l) = 0;
    virtual size_t getStartM() const = 0;
    virtual size_t getSizeK() const = 0;
    virtual size_t getSizeL() const = 0;
    virtual size_t getSizeM() const = 0;
    virtual size_t getSize() const = 0;
    virtual size_t getStrideK() const = 0;
    virtual size_t getStrideL() const = 0;
    virtual size_t getStrideM() const = 0;
    virtual size_t getIndex(size_t k, size_t l, size_t m) const throw(out_of_range) = 0;
};

#endif	/* GRID_H */

