/*
 * PixelFinder.h
 *
 *  Created on: Jan 18, 2012
 *      Author: ralf
 */

#ifndef PIXELFINDER_H_
#define PIXELFINDER_H_

#include "../core/Grid.h"
#include "../core/TiePointInterpolator.h"

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
	void updateNearestPixel(double targetLat, double targetLon, long k, long l, long m, long& sourceK, long& sourceL, long& sourceM, double& minDelta) const;

	long getK(size_t index) const {
		return index / geoLocation.getGrid().getStrideK();
	}
	long getL(size_t index) const {
		return (index - getK(index) * geoLocation.getGrid().getStrideK()) / geoLocation.getGrid().getStrideL();
	}
	long getM(size_t index) const {
		return index % geoLocation.getGrid().getSizeM();
	}
	long getN(long k, long m) const {
		return m + k * geoLocation.getGrid().getSizeM();
	}
	long getK(long n) const {
		return n / geoLocation.getGrid().getSizeM();
	}
	long getM(long n) const {
		return n % geoLocation.getGrid().getSizeM();
	}

	const GeoLocation& geoLocation;
	const double pixelSize;
	const TiePointInterpolator<double>* tpi;

	valarray<double> tpIndices;

	static const double DEG = 180.0 / 3.14159265358979323846;
};

#endif /* PIXELFINDER_H_ */
