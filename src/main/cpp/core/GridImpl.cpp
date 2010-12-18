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
 * File:   GridImpl.cpp
 * Author: ralf
 * 
 * Created on December 16, 2010, 3:47 PM
 */

#include "GridImpl.h"
#include "Constants.h"

GridImpl::GridImpl(size_t sizeK, size_t sizeL, size_t sizeM) : Grid() {
    this->startK = 0;
    this->startL = 0;
    this->startM = 0;
    this->sizeK = sizeK;
    this->sizeL = sizeL;
    this->sizeM = sizeM;
    this->strideK = sizeM * sizeL;
    this->strideL = sizeM;
    this->strideM = 1;
}

GridImpl::GridImpl(const Grid& b) : Grid() {
    this->startK = b.getStartK();
    this->startL = b.getStartL();
    this->startM = b.getStartM();
    this->sizeK = b.getSizeK();
    this->sizeL = b.getSizeL();
    this->sizeM = b.getSizeM();
    this->strideK = b.getStrideK();
    this->strideL = b.getStrideL();
    this->strideM = b.getStrideM();
}

GridImpl::~GridImpl() {
}

size_t GridImpl::getIndex(size_t k, size_t l, size_t m) const throw (out_of_range) {
    if (k < startK || k > startK + sizeK - 1) {
        throw out_of_range("index k is out of range.");
    }
    if (l < startL || l > startL + sizeL - 1) {
        throw out_of_range("index l is out of range.");
    }
    if (m < startM || m  > startM + sizeM - 1) {
        throw out_of_range("index m is out of range.");
    }
    return (k - startK) * strideK + (l - startL) * strideL + (m - startM) * strideM;
}
