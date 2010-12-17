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
    GridImpl(uint8_t sizeK, uint16_t sizeL, uint16_t sizeM);
    GridImpl(const Grid& bounds);
    virtual ~GridImpl();

    uint8_t getK() const {
        return k;
    }

    uint16_t getL() const {
        return l;
    }

    void setL(uint16_t l) {
        this->l = l;
    }

    uint16_t getM() const {
        return m;
    }

    uint8_t getSizeK() const {
        return sizeK;
    }

    uint16_t getSizeL() const {
        return sizeL;
    }

    uint16_t getSizeM() const {
        return sizeM;
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

private:

    uint8_t k;
    uint16_t l;
    uint16_t m;

    uint8_t sizeK;
    uint16_t sizeL;
    uint16_t sizeM;

    size_t strideK;
    size_t strideL;
    size_t strideM;
};

#endif	/* GRIDIMPL_H */

