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
	const long minK = geoGrid.getMinK();
	const long maxK = geoGrid.getMaxK();
	const long minL = geoGrid.getMinL();
	const long maxL = geoGrid.getMaxL();
	const long minM = geoGrid.getMinM();
	const long maxM = geoGrid.getMaxM();

	const long centerK = sourceK;
	const long centerL = sourceL;
	const long centerM = sourceM;

	double minDelta = numeric_limits<double>::max();
	bool found = false;

	for (long b = 0; b <= max(maxL, maxM); b++) {
		const long minBoundaryL = max(centerL - b, minL);
		const long maxBoundaryL = min(centerL + b, maxL);
		const long minBoundaryM = max(centerM - b, minM);
		const long maxBoundaryM = min(centerM + b, maxM);

		bool boundaryFound = false;

		for (long k = minK; k <= maxK; k++) {
			for (long l = minBoundaryL; l <= maxBoundaryL; l++) {
				boundaryFound |= isNearestPixel(targetLat, targetLon, k, l, minBoundaryM, sourceK, sourceL, sourceM, minDelta);
				boundaryFound |= isNearestPixel(targetLat, targetLon, k, l, maxBoundaryM, sourceK, sourceL, sourceM, minDelta);
			}
			for (long m = minBoundaryM; m <= maxBoundaryM; m++) {
				boundaryFound |= isNearestPixel(targetLat, targetLon, k, minBoundaryL, m, sourceK, sourceL, sourceM, minDelta);
				boundaryFound |= isNearestPixel(targetLat, targetLon, k, maxBoundaryL, m, sourceK, sourceL, sourceM, minDelta);
			}
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
