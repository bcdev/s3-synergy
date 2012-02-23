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

#include <stdexcept>

#include "Boost.h"
#include "GridImpl.h"
#include "Constants.h"

GridImpl::GridImpl(long sizeK, long sizeL, long sizeM, long minL, long maxL) :
		Grid() {
	using std::invalid_argument;

	if (minL > maxL) {
		BOOST_THROW_EXCEPTION(std::invalid_argument("minL > maxL"));
	}
	if (sizeL - 1 > maxL - minL) {
		BOOST_THROW_EXCEPTION(std::invalid_argument("sizeL - 1 > maxL - minL"));
	}
	this->minK = 0;
	this->minL = minL;
	this->maxL = maxL;
	this->minM = 0;
	this->sizeK = sizeK;
	this->sizeL = sizeL;
	this->sizeM = sizeM;
	this->strideK = sizeM * sizeL;
	this->strideL = sizeM;
	this->strideM = 1;
	this->minInMemoryL = minL;
}

GridImpl::~GridImpl() {
}

void GridImpl::moveForward(long l) {
	if (l < minL) {
		BOOST_THROW_EXCEPTION(std::out_of_range("l < minL"));
	}
	if (l + sizeL - 1 > maxL) {
		BOOST_THROW_EXCEPTION(std::out_of_range("l + sizeL - 1 > maxL"));
	}
	this->minInMemoryL = l;
}

size_t GridImpl::getIndex(long k, long l, long m) const throw (out_of_range) {
	using boost::lexical_cast;

	if (k < minK || k > minK + sizeK - 1) {
		BOOST_THROW_EXCEPTION(out_of_range("index k is out of range: k=" + lexical_cast<string>(k) + ", firstK=" + lexical_cast<string>(minK) + ", sizeK=" + lexical_cast<string>(sizeK)));
	}
	if (l < minInMemoryL || l > minInMemoryL + sizeL - 1) {
		BOOST_THROW_EXCEPTION(out_of_range("index l is out of range: l=" + lexical_cast<string>(l) + ", firstL=" + lexical_cast<string>(minInMemoryL) + ", sizeL=" + lexical_cast<string>(sizeL)));
	}
	if (m < minM || m > minM + sizeM - 1) {
		BOOST_THROW_EXCEPTION(out_of_range("index m is out of range: m=" + lexical_cast<string>(m) + ", firstM=" + lexical_cast<string>(minM) + ", sizeM=" + lexical_cast<string>(sizeM)));
	}
	return (k - minK) * strideK + (l - minInMemoryL) * strideL + (m - minM) * strideM;
}

bool GridImpl::isValidPosition(long k, long l, long m) const {
	return (getMinK() <= k && getMaxK() >= k && getMinInMemoryL() <= l && getMaxInMemoryL() >= l && getMinM() <= m && getMaxM() >= m);
}
