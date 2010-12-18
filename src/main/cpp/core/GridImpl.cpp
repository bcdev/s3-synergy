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
    this->k = 0;
    this->l = 0;
    this->m = 0;
    this->sizeK = sizeK;
    this->sizeL = sizeL;
    this->sizeM = sizeM;
    this->strideK = sizeM * sizeL;
    this->strideL = sizeM;
    this->strideM = 1;
}

GridImpl::GridImpl(const Grid& b) : Grid() {
    this->k = b.getStartK();
    this->l = b.getStartL();
    this->m = b.getStartM();
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
    if (k < this->k || k + 1 > this->k + sizeK) {
        throw out_of_range("index k is out of range.");
    }
    if (l < this->l || l + 1 > this->l + sizeL) {
        throw out_of_range("index l is out of range.");
    }
    if (m < this->m || m + 1 > this->m + sizeM) {
        throw out_of_range("index m is out of range.");
    }
    return (k - this->k) * strideK + (l - this->l) * strideL + (m - this->m) * strideM;
}
