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
 * File:   GridImpl.h
 * Author: ralf
 *
 * Created on December 16, 2010, 3:47 PM
 */

#ifndef GRIDIMPL_H
#define	GRIDIMPL_H

#include "Grid.h"

class GridImpl : public virtual Grid {
public:
    GridImpl(size_t sizeK, size_t sizeL, size_t sizeM, size_t minL, size_t maxL);
    GridImpl(const Grid& bounds);
    virtual ~GridImpl();

    size_t getStartK() const {
        return startK;
    }

    size_t getStartL() const {
        return startL;
    }

    void setStartL(size_t l) {
        if (l + sizeL - 1 > maxL) {
            l = maxL - sizeL + 1;
        }
        this->startL = l;
    }

    size_t getStartM() const {
        return startM;
    }

    size_t getSizeK() const {
        return sizeK;
    }

    size_t getSizeL() const {
        return sizeL;
    }

    size_t getSizeM() const {
        return sizeM;
    }

    size_t getSize() const {
        return sizeM * sizeL * sizeK;
    }

    size_t getStrideK() const {
        return strideK;
    }

    size_t getStrideL() const {
        return strideL;
    }

    size_t getStrideM() const {
        return strideM;
    }

    size_t getIndex(size_t k, size_t l, size_t m) const throw (out_of_range);

    size_t getMinL() const {
        return minL;
    }

    size_t getMaxL() const {
        return maxL;
    }

private:
    size_t startK;
    size_t startL;
    size_t startM;

    size_t sizeK;
    size_t sizeL;
    size_t sizeM;

    size_t strideK;
    size_t strideL;
    size_t strideM;

    size_t minL;
    size_t maxL;
};

#endif	/* GRIDIMPL_H */

