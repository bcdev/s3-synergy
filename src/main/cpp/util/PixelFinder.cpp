/*
 * PixelFinder.cpp
 *
 *  Created on: Jan 18, 2012
 *      Author: ralf
 */

#include <algorithm>
#include <cmath>
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
	const Grid& grid = geoLocation.getGrid();

	valarray<double> w(1);
	valarray<size_t> i(1);
	tpi->prepare(targetLon, targetLat, w, i);

	const long strideK = grid.getStrideK();
	const long strideL = grid.getStrideL();

	const size_t index = tpi->interpolate(tpIndices, w, i);
	sourceK = index / strideK;
	sourceL = (index - sourceK * strideK) / strideL;
	sourceM = index % grid.getSizeM();

	return findSourcePixel(targetLat, targetLon, index, sourceK, sourceL, sourceM);
}

bool PixelFinder::findSourcePixel(double targetLat, double targetLon, size_t index, long& sourceK, long& sourceL, long& sourceM) const {
	using std::acos;
	using std::max;
	using std::min;

	const Grid& grid = geoLocation.getGrid();
	const long sizeK = grid.getSizeK();
	const long sizeM = grid.getSizeM();

	double delta = -1.0;

	isNearestPixel(targetLat, targetLon, sourceK, sourceL, sourceM, sourceK, sourceL, sourceM, delta);

	for (long b = 32; b > 0; b >>= 1) {
		const long sourceN = sourceM + sourceK * sizeM;

		const long minL = max(0L, sourceL - b);
		const long maxL = min(grid.getMaxL(), sourceL + b);
		const long minN = max(0L, sourceN - b);
		const long maxN = min(sizeK * sizeM - 1, sourceN + b);
		const long minK = minN / sizeM;
		const long maxK = maxN / sizeM;
		const long minM = minN % sizeM;
		const long maxM = maxN % sizeM;

		if (true) {
			const long centerK = sourceK;
			const long centerL = sourceL;
			const long centerM = sourceM;

			isNearestPixel(targetLat, targetLon, minK, centerL, minM, sourceK, sourceL, sourceM, delta);
			isNearestPixel(targetLat, targetLon, maxK, centerL, maxM, sourceK, sourceL, sourceM, delta);
			isNearestPixel(targetLat, targetLon, centerK, maxL, centerM, sourceK, sourceL, sourceM, delta);
			isNearestPixel(targetLat, targetLon, centerK, minL, centerM, sourceK, sourceL, sourceM, delta);
		}

		isNearestPixel(targetLat, targetLon, minK, minL, minM, sourceK, sourceL, sourceM, delta);
		isNearestPixel(targetLat, targetLon, minK, maxL, minM, sourceK, sourceL, sourceM, delta);
		isNearestPixel(targetLat, targetLon, maxK, maxL, maxM, sourceK, sourceL, sourceM, delta);
		isNearestPixel(targetLat, targetLon, maxK, minL, maxM, sourceK, sourceL, sourceM, delta);
	}

    return acos(delta) * DEG < 1.41 * pixelSize;
}

bool PixelFinder::isNearestPixel(double targetLat, double targetLon, long k, long l, long m, long& resultK, long& resultL, long& resultM, double& maxDelta) const {
	using std::abs;

	const size_t index = geoLocation.getGrid().getIndex(k, l, m);
	const double sourceLat = geoLocation.getLat(index);
	const double sourceLon = geoLocation.getLon(index);

	const double delta = TiePointInterpolator<double>::cosineDistance(targetLon, targetLat, sourceLon, sourceLat);
	if (delta > maxDelta) {
		resultK = k;
		resultL = l;
		resultM = m;
		maxDelta = delta;
		return true;
	}
	return false;
}
