/*
 * Aei.h
 *
 *  Created on: Oct 24, 2011
 *      Author: thomasstorm
 */

#ifndef AEI_H_
#define AEI_H_

#include "../core/Segment.h"

#include "BasicModule.h"

class Aei: public BasicModule {
public:
	Aei();
	virtual ~Aei();

	void start(Context& context);
	void process(Context& context);

private:
	friend class AeiTest;

	double interpolation(const Accessor& accessor, long k, long l0, long l1, long m0, long m1, double wl, double wm) const;

	static long minMax(long x, long min, long max) {
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

	int16_t averagingFactor;
};

#endif /* AEI_H_ */
