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

#ifndef AEI_H_
#define AEI_H_

#include "../core/Segment.h"

#include "BasicModule.h"

/**
 * The SYN L2 aerosol interpolation module (AEI).
 */
class Aei: public BasicModule {
public:
	Aei();
	virtual ~Aei();

	void start(Context& context);
	void process(Context& context);

private:
	friend class AeiTest;

	double interpolation(const Accessor& accessor, long k, long l0, long l1, long m0, long m1, double wl, double wm) const;

	template<class T>
	static long minMax(T x, T min, T max) {
		if (x < min) {
			return min;
		}
		if (x > max) {
			return max;
		}
		return x;
	}

	const Grid* targetGrid;
	const Grid* sourceGrid;

	const Segment* sourceSegment;
	Segment* targetSegment;

	uint8_t averagingFactor;
};

#endif /* AEI_H_ */
