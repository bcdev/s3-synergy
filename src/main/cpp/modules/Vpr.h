/*
 * Vpr.h
 *
 *  Created on: Dec 01, 2011
 *      Author: thomasstorm
 */

#ifndef VPR_H_
#define VPR_H_

#include "../modules/BasicModule.h"

class Vpr : public BasicModule {
public:
	Vpr();
	virtual ~Vpr();

	void start(Context& context);
	void process(Context& context);

private:
	friend class VprTest;

	Segment* collocatedSegment;
	Segment* vgtSegment;

	static const uint16_t PIXELS_PER_DEGREE = 112;
	static const uint16_t LAT_CELL_COUNT = 180;
	static const uint16_t LON_CELL_COUNT = 360;

	static const long LINE_COUNT = LAT_CELL_COUNT * PIXELS_PER_DEGREE;
	static const long COL_COUNT = LON_CELL_COUNT * PIXELS_PER_DEGREE;

	static double getLatitude(long l);
	static double getLongitude(long l);
};

#endif /* VPR_H_ */
