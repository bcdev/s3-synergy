/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

#include <algorithm>
#include <cmath>
#include <iostream>

#include "PixelFinder.h"

const double PixelFinder::DEG = 180.0 / 3.14159265358979323846;
const double PixelFinder::RAD = 3.14159265358979323846 / 180.0;

PixelFinder::PixelFinder(GeoLocation& geoLocation, double tolerance) :
		geoLocation(geoLocation), tolerance(tolerance) {
	const Grid& grid = geoLocation.getGrid();

	const long sizeK = grid.getSizeK();
	const long sizeL = grid.getSizeL();
	const long sizeM = grid.getSizeM();

	size_t tpCount = computeTiePointCount(1, sizeL, sizeM);

	for (long k = 0; k < sizeK; k++) {
		valarray<double> tpLats(tpCount);
		valarray<double> tpLons(tpCount);
		valarray<double> tpIdxs(tpCount);

		size_t i = 0;
		for (long l = 0; l < sizeL; l += 64) {
			for (long m = 0; m < sizeM; m += 64) {
				const size_t index = grid.getIndex(k, l, m);
				tpLats[i] = geoLocation.getLat(index);
				tpLons[i] = geoLocation.getLon(index);
				tpIdxs[i] = index;
				i++;
			}
		}

		tpInterpolators.push_back(TiePointInterpolator<double>(tpLons, tpLats));
		tpIndexes.push_back(tpIdxs);
	}
}

PixelFinder::~PixelFinder() {
}

bool PixelFinder::findSourcePixel(double targetLat, double targetLon, long& k, long& l, long& m) const {
	using std::acos;
	using std::max;
	using std::min;

	const Grid& grid = geoLocation.getGrid();

	valarray<double> w(1);
	valarray<size_t> i(1);

	double delta1 = -1.0;
	bool found = false;

	for (long k2 = 0; k2 < grid.getSizeK(); k2++) {
		tpInterpolators[k2].prepare(targetLon, targetLat, w, i);

		const size_t index = tpInterpolators[k2].interpolate(tpIndexes[k2], w, i);
		long l2 = getL(index);
		long m2 = getM(index);

		double delta2 = -1.0;
		updateNearestPixel(targetLat, targetLon, k2, l2, m2, k2, l2, m2, delta2, found);

		for (long b = 64; b > 0; b >>= 1) {
			const long midL = l2;
			const long midM = m2;

			const long outerMinL = max(grid.getMinL(), midL - b);
			const long outerMaxL = min(grid.getMaxL(), midL + b);
			const long outerMinM = max(grid.getMinM(), midM - b);
			const long outerMaxM = min(grid.getMaxM(), midM + b);

			if (true) { // consider outer points in the N, S, E, and W
				updateNearestPixel(targetLat, targetLon, k2, midL, outerMinM, k2, l2, m2, delta2, found);
				updateNearestPixel(targetLat, targetLon, k2, midL, outerMaxM, k2, l2, m2, delta2, found);
				updateNearestPixel(targetLat, targetLon, k2, outerMaxL, midM, k2, l2, m2, delta2, found);
				updateNearestPixel(targetLat, targetLon, k2, outerMinL, midM, k2, l2, m2, delta2, found);
			}
			if (true) { // consider outer points in the NW, SW, SE, and NE
				updateNearestPixel(targetLat, targetLon, k2, outerMinL, outerMinM, k2, l2, m2, delta2, found);
				updateNearestPixel(targetLat, targetLon, k2, outerMaxL, outerMinM, k2, l2, m2, delta2, found);
				updateNearestPixel(targetLat, targetLon, k2, outerMaxL, outerMaxM, k2, l2, m2, delta2, found);
				updateNearestPixel(targetLat, targetLon, k2, outerMinL, outerMaxM, k2, l2, m2, delta2, found);
			}
			if (true) { // consider inner points in the NW, SW, SE, and NE
				const long innerMinL = max(outerMinL, midL - b / 2);
				const long innerMaxL = min(outerMaxL, midL + b / 2);
				const long innerMinM = max(outerMinM, midM - b / 2);
				const long innerMaxM = min(outerMaxM, midM + b / 2);

				updateNearestPixel(targetLat, targetLon, k2, innerMinL, innerMinM, k2, l2, m2, delta2, found);
				updateNearestPixel(targetLat, targetLon, k2, innerMaxL, innerMinM, k2, l2, m2, delta2, found);
				updateNearestPixel(targetLat, targetLon, k2, innerMaxL, innerMaxM, k2, l2, m2, delta2, found);
				updateNearestPixel(targetLat, targetLon, k2, innerMinL, innerMaxM, k2, l2, m2, delta2, found);
			}
			updateNearestPixel(targetLat, targetLon, k2, l2, m2, k, l, m, delta1, found);
		}
		if (found) {
			break;
		}
	}

	return found;
}

void PixelFinder::updateNearestPixel(double targetLat, double targetLon, long k, long l, long m, long& resultK, long& resultL, long& resultM, double& maxDelta, bool& found) const {
	const size_t sourceIndex = geoLocation.getGrid().getIndex(k, l, m);
	const double sourceLat = geoLocation.getLat(sourceIndex);
	const double sourceLon = geoLocation.getLon(sourceIndex);
	const double delta = TiePointInterpolator<double>::cosineDistance(targetLon, targetLat, sourceLon, sourceLat);

	if (delta > maxDelta) {
		resultK = k;
		resultL = l;
		resultM = m;
		maxDelta = delta;
		found = found || acos(delta) * DEG < tolerance;
	}
}

size_t PixelFinder::computeTiePointCount(long sizeK, long sizeL, long sizeM) {
	size_t tpCount = 0;
	for (long k = 0; k < sizeK; k++) {
		for (long l = 0; l < sizeL; l += 64) {
			for (long m = 0; m < sizeM; m += 64) {
				tpCount++;
			}
		}
	}
	return tpCount;
}
