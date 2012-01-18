/*
 * PixelFinder.h
 *
 *  Created on: Jan 18, 2012
 *      Author: ralf
 */

#ifndef PIXELFINDER_H_
#define PIXELFINDER_H_

#include "../core/Grid.h"

class GeoLocation {
public:
	virtual ~GeoLocation() {
	}

	virtual double getLat(size_t index) const = 0;
	virtual double getLon(size_t index) const = 0;
	virtual const Grid& getGrid() const = 0;
};

class PixelFinder {
public:
	PixelFinder(GeoLocation& geoLocation, double pixelSize);
	~PixelFinder();

	bool findSourcePixel(double targetLat, double targetLon, long& sourceK, long& sourceL, long& sourceM) const;

private:
	bool isNearestPixel(double targetLat, double targetLon, long k, long l, long m, long& sourceK, long& sourceL, long& sourceM, double& minDelta) const;

	const GeoLocation& geoLocation;
	const double pixelSize;
};

#endif /* PIXELFINDER_H_ */
