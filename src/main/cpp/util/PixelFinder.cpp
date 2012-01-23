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

bool PixelFinder::findSourcePixel(double targetLat, double targetLon, long& k, long& l, long& m) const {
	using std::acos;
	using std::max;
	using std::min;

	const Grid& grid = geoLocation.getGrid();

	valarray<double> w(1);
	valarray<size_t> i(1);
	tpi->prepare(targetLon, targetLat, w, i);

	const size_t index = tpi->interpolate(tpIndices, w, i);
	k = getK(index);
	l = getL(index);
	m = getM(index);

	double delta = -1.0;
	updateNearestPixel(targetLat, targetLon, k, l, m, k, l, m, delta);

	for (long b = 32; b > 0; b >>= 1) {
		const long n = getN(k, m);

		const long minL = max(grid.getMinL(), l - b);
		const long maxL = min(grid.getMaxL(), l + b);
		const long minN = max(getN(grid.getMinK(), grid.getMinM()), n - b);
		const long maxN = min(getN(grid.getMaxK(), grid.getMaxM()), n + b);

		const long minK = getK(minN);
		const long maxK = getK(maxN);
		const long minM = getM(minN);
		const long maxM = getM(maxN);

		if (true) { // consider points in the N, S, E, and W
			const long midK = k;
			const long midL = l;
			const long midM = m;

			updateNearestPixel(targetLat, targetLon, minK, midL, minM, k, l, m, delta);
			updateNearestPixel(targetLat, targetLon, maxK, midL, maxM, k, l, m, delta);
			updateNearestPixel(targetLat, targetLon, midK, maxL, midM, k, l, m, delta);
			updateNearestPixel(targetLat, targetLon, midK, minL, midM, k, l, m, delta);
		}
		if (true) { // consider points in the NW, SW, SE, and NE
			updateNearestPixel(targetLat, targetLon, minK, minL, minM, k, l, m, delta);
			updateNearestPixel(targetLat, targetLon, minK, maxL, minM, k, l, m, delta);
			updateNearestPixel(targetLat, targetLon, maxK, maxL, maxM, k, l, m, delta);
			updateNearestPixel(targetLat, targetLon, maxK, minL, maxM, k, l, m, delta);
		}
	}

    return acos(delta) * DEG < 1.4 * pixelSize;
}

void PixelFinder::updateNearestPixel(double targetLat, double targetLon, long k, long l, long m, long& resultK, long& resultL, long& resultM, double& maxDelta) const {
	const size_t index = geoLocation.getGrid().getIndex(k, l, m);
	const double sourceLat = geoLocation.getLat(index);
	const double sourceLon = geoLocation.getLon(index);
	const double delta = TiePointInterpolator<double>::cosineDistance(targetLon, targetLat, sourceLon, sourceLat);

	if (delta > maxDelta) {
		resultK = k;
		resultL = l;
		resultM = m;
		maxDelta = delta;
	}
}
