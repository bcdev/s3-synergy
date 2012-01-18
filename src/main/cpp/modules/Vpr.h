/*
 * Vpr.h
 *
 *  Created on: Dec 01, 2011
 *      Author: thomasstorm
 */

#ifndef VPR_H_
#define VPR_H_

#include "../core/Pixel.h"
#include "../modules/BasicModule.h"

class Vpr : public BasicModule {
public:
	Vpr();
	virtual ~Vpr();

	void start(Context& context);
	void process(Context& context);
	void process2(Context& context);

private:
	friend class VprTest;

	Segment* vgpSegment;
	const Segment* synSegment;
	const Segment* geoSegment;
	// TODO - do this somewhere else
	const Segment* olcSegment;
	const Segment* slnSegment;
	const Segment* sloSegment;

	const Accessor* latAccessor;
	const Accessor* lonAccessor;

    valarray<Accessor*> synReflectanceAccessors;
    valarray<Accessor*> vgtReflectanceAccessors;
    Accessor* vgtFlagsAccessor;
    Accessor* synFlagsAccessor;

    static const size_t PIXEL_SEARCH_RADIUS = 10;

	static const uint16_t PIXELS_PER_DEGREE = 112;
	static const uint16_t LAT_CELL_COUNT = 131;
	static const uint16_t LON_CELL_COUNT = 360;

	static const long LINE_COUNT = LAT_CELL_COUNT * PIXELS_PER_DEGREE;
	static const long COL_COUNT = LON_CELL_COUNT * PIXELS_PER_DEGREE;

	static const double TARGET_PIXEL_SIZE = 1.0 / PIXELS_PER_DEGREE;

	static double getTargetLat(long l);
	static double getTargetLon(long l);

	void setupAccessors();
	void getMinMaxSourceLat(double& minLat, double& maxLat) const;
	void getMinMaxTargetLat(double& minLat, double& maxLat, long firstL, long lastL) const;
	void getMinMaxSourceLon(double& minLon, double& maxLon) const;
	bool findSourcePixel(double targetLat, double targetLon, long& sourceK, long& sourceL, long& sourceM) const;
	bool findSourcePixel(double targetLat, double targetLon, long firstK, long lastK, long firstL, long lastL, long firstM, long lastM, long& sourceK, long& sourceL, long& sourceM) const;
	long findLineOfSynSegmentNearestTo(double vgtMaxLat) const;
	void setValues(long synK, long synL, long synM, long l, long m);
};

#endif /* VPR_H_ */
