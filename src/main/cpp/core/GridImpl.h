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

class GridImpl: public virtual Grid {
public:
	GridImpl(long sizeK, long sizeL, long sizeM, long minL, long maxL);

	GridImpl(const Grid& bounds);

	virtual ~GridImpl();

	long getFirstL() const {
		return firstL;
	}

	long getLastL() const {
		return firstL + sizeL - 1;
	}

	long getSizeK() const {
		return sizeK;
	}

	long getSizeL() const {
		return sizeL;
	}

	long getSizeM() const {
		return sizeM;
	}

	long getSize() const {
		return sizeM * sizeL * sizeK;
	}

	long getStrideK() const {
		return strideK;
	}

	long getStrideL() const {
		return strideL;
	}

	long getStrideM() const {
		return strideM;
	}

	size_t getIndex(long k, long l, long m) const throw (out_of_range);

	long getMinK() const {
		return minK;
	}

	long getMaxK() const {
		return minK + sizeK - 1;
	}

	long getMinL() const {
		return minL;
	}

	long getMaxL() const {
		return maxL;
	}

	long getMinM() const {
		return minM;
	}

	long getMaxM() const {
		return minM + sizeM - 1;
	}

	long getK(size_t index) const {
	    return index / strideK;
	}

	long getL(size_t index) const {
	    return (index % strideK) / strideL;
	}

	long getM(size_t index) const {
	    return index % sizeM;
	}

	void setFirstL(long l);

	bool isValidPosition(long k, long l, long m) const;

private:
	long minK;
	long minM;
	long minL;
	long maxL;

	long sizeK;
	long sizeL;
	long sizeM;

	long strideK;
	long strideL;
	long strideM;

	long firstL;
};

#endif	/* GRIDIMPL_H */
