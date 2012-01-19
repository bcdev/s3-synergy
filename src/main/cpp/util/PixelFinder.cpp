/*
 * PixelFinder.cpp
 *
 *  Created on: Jan 18, 2012
 *      Author: ralf
 */

#include <algorithm>
#include <cmath>
#include <limits>

#include "PixelFinder.h"

PixelFinder::PixelFinder(GeoLocation& geoLocation, double pixelSize) : geoLocation(geoLocation), pixelSize(pixelSize) {
}

PixelFinder::~PixelFinder() {
}

bool PixelFinder::findSourcePixel(double targetLat, double targetLon, long& sourceK, long& sourceL, long& sourceM) const {
	using std::max;
	using std::min;
	using std::numeric_limits;

	const Grid& geoGrid = geoLocation.getGrid();
	const long sizeK = geoGrid.getSizeK();
	const long sizeM = geoGrid.getSizeM();

	const long minK = geoGrid.getMinK();
	const long maxK = geoGrid.getMaxK();
	const long minL = geoGrid.getMinL();
	const long maxL = geoGrid.getMaxL();
	const long minM = geoGrid.getMinM();
	const long maxM = geoGrid.getMaxM();
	const long minN = minM;
	const long maxN = sizeK * sizeM - 1;

	const long centerK = sourceK;
	const long centerL = sourceL;
	const long centerM = sourceM;
	const long centerN = sourceM + sourceK * sizeM;

	double minDelta = numeric_limits<double>::max();
	bool found = false;

	for (long b = 0; b <= max(maxL, maxN); b++) {
		const long minBoundaryL = max(centerL - b, minL);
		const long maxBoundaryL = min(centerL + b, maxL);
		const long minBoundaryN = max(centerN - b, minN);
		const long maxBoundaryN = min(centerN + b, maxN);
		const long minBoundaryM = minBoundaryN % sizeM;
		const long maxBoundaryM = maxBoundaryN % sizeM;
		const long minBoundaryK = minBoundaryN / sizeM;
		const long maxBoundaryK = maxBoundaryN / sizeM;

		bool boundaryFound = false;

		for (long l = minBoundaryL; l <= maxBoundaryL; l++) {
			boundaryFound |= isNearestPixel(targetLat, targetLon, minBoundaryK, l, minBoundaryM, sourceK, sourceL, sourceM, minDelta);
			boundaryFound |= isNearestPixel(targetLat, targetLon, maxBoundaryK, l, maxBoundaryM, sourceK, sourceL, sourceM, minDelta);
		}
		for (long n = minBoundaryN; n <= maxBoundaryN; n++) {
			boundaryFound |= isNearestPixel(targetLat, targetLon, n / sizeM, minBoundaryL, n % sizeM, sourceK, sourceL, sourceM, minDelta);
			boundaryFound |= isNearestPixel(targetLat, targetLon, n / sizeM, maxBoundaryL, n % sizeM, sourceK, sourceL, sourceM, minDelta);
		}
		if (boundaryFound) {
			;
		}
		found |= boundaryFound;
		if (found && !boundaryFound) {
			break;
		}
	}

    return found;
}

bool PixelFinder::isNearestPixel(double targetLat, double targetLon, long k, long l, long m, long& resultK, long& resultL, long& resultM, double& minDelta) const {
	using std::abs;

	const Grid& geoGrid = geoLocation.getGrid();

	const size_t index = geoGrid.getIndex(k, l, m);
	const double sourceLat = geoLocation.getLat(index);
	const double sourceLon = geoLocation.getLon(index);
	const double latDelta = abs(targetLat - sourceLat);
	if (latDelta > 0.5 * pixelSize) {
		return false;
	}
	// TODO - anti-meridian
	const double lonDelta = abs(targetLon - sourceLon);
	if (lonDelta > 0.5 * pixelSize) {
		return false;
	}
	const double delta = latDelta + lonDelta;
	if (delta < minDelta) {
		resultK = k;
		resultL = l;
		resultM = m;
		minDelta = delta;
		return true;
	}
	return false;
}
