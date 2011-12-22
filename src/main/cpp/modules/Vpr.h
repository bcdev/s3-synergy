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

private:
	friend class VprTest;

	Segment* vgtSegment;
	const Segment* collocatedSegment;

	const Grid* geoGrid;
	const Accessor* latAccessor;
	const Accessor* lonAccessor;

	static const uint16_t PIXELS_PER_DEGREE = 112;
	static const uint16_t LAT_CELL_COUNT = 131;
	static const uint16_t LON_CELL_COUNT = 360;

	static const long LINE_COUNT = LAT_CELL_COUNT * PIXELS_PER_DEGREE;
	static const long COL_COUNT = LON_CELL_COUNT * PIXELS_PER_DEGREE;

	static const double PIXEL_SIZE = 1.0 / PIXELS_PER_DEGREE;

	static double getLatitude(long l);
	static double getLongitude(long l);

	void findPixelPos(double lat, double lon, valarray<long>& synIndices) const;
	void findPixelPos(double lat, double lon, long k0, long kMax, long l0, long lMax, long m0, long mMax, valarray<long>& synIndices) const;
	void findPixelPosAroundGivenIndices(double lat, double lon, valarray<long>& synIndices) const;
	void setupPixel(shared_ptr<Pixel> p, long synK, long synL, long synM) const;
	Pixel& findClosestPixel(const valarray<shared_ptr<Pixel> >& pixels, double lat, double lon) const;
	void setValues(const Pixel& p, long l, long m);
};

#endif /* VPR_H_ */
