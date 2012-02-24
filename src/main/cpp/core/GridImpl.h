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

#ifndef GRIDIMPL_H
#define	GRIDIMPL_H

#include "Grid.h"

/**
 * A concrete implementation of the Grid interface.
 */
class GridImpl: public virtual Grid {
public:
	/**
	 * Constructor.
	 * @param sizeK The size of the camera dimension.
	 * @param sizeL The number of rows in memory.
	 * @param sizeM The size of the column dimension.
	 * @param minL The minimum row index.
	 * @param maxL The maximum row index.
	 */
	GridImpl(long sizeK, long sizeL, long sizeM, long minL, long maxL);

	/**
	 * Destructor.
	 */
	virtual ~GridImpl();

	long getMinInMemoryL() const {
		return minInMemoryL;
	}

	long getMaxInMemoryL() const {
		return minInMemoryL + sizeL - 1;
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

	void moveForward(long l);

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

	long minInMemoryL;
};

#endif	/* GRIDIMPL_H */
