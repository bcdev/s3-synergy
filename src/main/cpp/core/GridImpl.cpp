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

#include <stdexcept>

#include "Boost.h"
#include "GridImpl.h"
#include "Constants.h"

using std::invalid_argument;

GridImpl::GridImpl(long sizeK, long sizeL, long sizeM, long minL, long maxL) :
		Grid() {
	if (minL > maxL) {
		BOOST_THROW_EXCEPTION(invalid_argument("minL > maxL"));
	}
	if (sizeL - 1 > maxL - minL) {
		BOOST_THROW_EXCEPTION(invalid_argument("sizeL - 1 > maxL - minL"));
	}
	this->firstK = 0;
	this->firstL = minL;
	this->firstM = 0;
	this->sizeK = sizeK;
	this->sizeL = sizeL;
	this->sizeM = sizeM;
	this->strideK = sizeM * sizeL;
	this->strideL = sizeM;
	this->strideM = 1;
	this->minL = minL;
	this->maxL = maxL;
}

GridImpl::GridImpl(const Grid& b) :
		Grid() {
	this->firstK = b.getFirstK();
	this->firstL = b.getFirstL();
	this->firstM = b.getFirstM();
	this->sizeK = b.getSizeK();
	this->sizeL = b.getSizeL();
	this->sizeM = b.getSizeM();
	this->strideK = b.getStrideK();
	this->strideL = b.getStrideL();
	this->strideM = b.getStrideM();
}

GridImpl::~GridImpl() {
}

void GridImpl::setFirstL(long l) {
	if (l < minL) {
		BOOST_THROW_EXCEPTION(std::out_of_range("l < minL"));
	}
	if (l + sizeL - 1 > maxL) {
		BOOST_THROW_EXCEPTION(std::out_of_range("l + sizeL - 1 > maxL"));
	}
	this->firstL = l;
}

size_t GridImpl::getIndex(long k, long l, long m) const throw (out_of_range) {
	if (k < firstK || k > firstK + sizeK - 1) {
		BOOST_THROW_EXCEPTION(out_of_range("index k is out of range: k=" + lexical_cast<string>(k) + ", firstK=" + lexical_cast<string>(firstK) + ", sizeK=" + lexical_cast<string>(sizeK)));
	}
	if (l < firstL || l > firstL + sizeL - 1) {
		BOOST_THROW_EXCEPTION(out_of_range("index l is out of range: l=" + lexical_cast<string>(l) + ", firstL=" + lexical_cast<string>(firstL) + ", sizeL=" + lexical_cast<string>(sizeL)));
	}
	if (m < firstM || m > firstM + sizeM - 1) {
		BOOST_THROW_EXCEPTION(out_of_range("index m is out of range: m=" + lexical_cast<string>(m) + ", firstM=" + lexical_cast<string>(firstM) + ", sizeM=" + lexical_cast<string>(sizeM)));
	}
	return (k - firstK) * strideK + (l - firstL) * strideL + (m - firstM) * strideM;
}

bool GridImpl::isValidPosition(long k, long l, long m) const {
    return (getFirstK() <= k &&
            getMaxK() >= k &&
            getFirstL() <= l &&
            getMaxL() >= l &&
            getFirstM() <= m &&
            getMaxM() >= m);
}
