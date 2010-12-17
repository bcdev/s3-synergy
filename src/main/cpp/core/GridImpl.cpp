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

GridImpl::GridImpl(uint8_t sizeK, uint16_t sizeL, uint16_t sizeM) : Grid() {
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
    this->k = b.getK();
    this->l = b.getL();
    this->m = b.getM();
    this->sizeK = b.getSizeK();
    this->sizeL = b.getSizeL();
    this->sizeM = b.getSizeM();
    this->strideK = b.getStrideK();
    this->strideL = b.getStrideL();
    this->strideM = b.getStrideM();
}

GridImpl::~GridImpl() {
}

