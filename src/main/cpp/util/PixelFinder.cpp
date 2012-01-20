/*
 * PixelFinder.cpp
 *
 *  Created on: Jan 18, 2012
 *      Author: ralf
 */

#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>

#include "PixelFinder.h"

PixelFinder::PixelFinder(GeoLocation& geoLocation, double pixelSize) : geoLocation(geoLocation), pixelSize(pixelSize), tpIndices() {
	const Grid& grid = geoLocation.getGrid();

	const long sizeK = grid.getSizeK();
	const long sizeL = grid.getSizeL();
	const long sizeM = grid.getSizeM();

	size_t tpCount = 0;

	for (long k = 0; k < sizeK; k++) {
		for (long l = 0; l < sizeL; l += 64) {
			for (long m = 0; m < sizeM; m += 64) {
				tpCount++;
			}
		}
	}

	tpIndices.resize(tpCount);
	valarray<double> tpLats(tpCount);
	valarray<double> tpLons(tpCount);

	for (long i = 0, k = 0; k < sizeK; k++) {
		for (long l = 0; l < sizeL; l += 64) {
			for (long m = 0; m < sizeM; i++, m += 64) {
				const size_t index = grid.getIndex(k, l, m);
				tpLats[i] = geoLocation.getLat(index);
				tpLons[i] = geoLocation.getLon(index);
				tpIndices[i] = index;
			}
		}
	}

	tpi = new TiePointInterpolator<double>(tpLons, tpLats);
}

PixelFinder::~PixelFinder() {
	delete tpi;
}

bool PixelFinder::findSourcePixel(double targetLat, double targetLon, long& sourceK, long& sourceL, long& sourceM) const {
	using std::max;
	using std::min;
	using std::numeric_limits;

	valarray<double> w(1);
	valarray<size_t> i(1);
	tpi->prepare(targetLon, targetLat, w, i);
	const size_t index = tpi->interpolate(tpIndices, w, i);

	const Grid& grid = geoLocation.getGrid();
	const long sizeK = grid.getSizeK();
	const long sizeM = grid.getSizeM();

	/*
	const long centerK = sourceK;
	const long centerL = sourceL;
	const long centerM = sourceM;
	const long centerN = sourceM + sourceK * sizeM;
	*/

	const long centerK = index / grid.getStrideK();;
	const long centerL = (index - centerK * grid.getStrideK()) / grid.getStrideL();
	const long centerM = index % grid.getSizeM();
	const long centerN = centerM + centerK * sizeM;

	const long minL = max(grid.getMinL(), centerL - 64);
	const long maxL = min(grid.getMaxL(), centerL + 64);
	const long minN = max(0L, centerN - 64);
	const long maxN = min(sizeK * sizeM - 1, centerN + 64);

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
