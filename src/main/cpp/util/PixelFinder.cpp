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

PixelFinder::PixelFinder(GeoLocation& geoLocation, double pixelSize) : geoLocation(geoLocation), pixelSize(pixelSize), tpIndices() {
	const Grid& grid = geoLocation.getGrid();

	const long sizeK = grid.getSizeK();
	const long sizeL = grid.getSizeL();
	const long sizeM = grid.getSizeM();

	size_t tpCount = computeTiePointCount(sizeK, sizeL, sizeM);

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
		const long midK = k;
		const long midL = l;
		const long midM = m;
		const long midN = getN(k, m);

		const long outerMinL = max(grid.getMinL(), midL - b);
		const long outerMaxL = min(grid.getMaxL(), midL + b);
		const long outerMinN = max(getN(grid.getMinK(), grid.getMinM()), midN - b);
		const long outerMaxN = min(getN(grid.getMaxK(), grid.getMaxM()), midN + b);

		const long outerMinK = getK(outerMinN);
		const long outerMaxK = getK(outerMaxN);
		const long outerMinM = getM(outerMinN);
		const long outerMaxM = getM(outerMaxN);

		if (true) { // consider outer points in the N, S, E, and W
			updateNearestPixel(targetLat, targetLon, outerMinK, midL, outerMinM, k, l, m, delta);
			updateNearestPixel(targetLat, targetLon, outerMaxK, midL, outerMaxM, k, l, m, delta);
			updateNearestPixel(targetLat, targetLon, midK, outerMaxL, midM, k, l, m, delta);
			updateNearestPixel(targetLat, targetLon, midK, outerMinL, midM, k, l, m, delta);
		}
		if (true) { // consider outer points in the NW, SW, SE, and NE
			updateNearestPixel(targetLat, targetLon, outerMinK, outerMinL, outerMinM, k, l, m, delta);
			updateNearestPixel(targetLat, targetLon, outerMinK, outerMaxL, outerMinM, k, l, m, delta);
			updateNearestPixel(targetLat, targetLon, outerMaxK, outerMaxL, outerMaxM, k, l, m, delta);
			updateNearestPixel(targetLat, targetLon, outerMaxK, outerMinL, outerMaxM, k, l, m, delta);
		}
		if (true) { // consider inner points in the NW, SW, SE, and NE
			const long innerMinL = max(outerMinL, midL - b / 2);
			const long innerMaxL = min(outerMaxL, midL + b / 2);
			const long innerMinN = max(outerMinN, midN - b / 2);
			const long innerMaxN = min(outerMaxN, midN + b / 2);

			const long innerMinK = getK(innerMinN);
			const long innerMaxK = getK(innerMaxN);
			const long innerMinM = getM(innerMinN);
			const long innerMaxM = getM(innerMaxN);

			updateNearestPixel(targetLat, targetLon, innerMinK, innerMinL, innerMinM, k, l, m, delta);
			updateNearestPixel(targetLat, targetLon, innerMinK, innerMaxL, innerMinM, k, l, m, delta);
			updateNearestPixel(targetLat, targetLon, innerMaxK, innerMaxL, innerMaxM, k, l, m, delta);
			updateNearestPixel(targetLat, targetLon, innerMaxK, innerMinL, innerMaxM, k, l, m, delta);
		}
	}

    return true; // acos(delta) * DEG < pixelSize;
}

void PixelFinder::updateNearestPixel(double targetLat, double targetLon, long k, long l, long m, long& resultK, long& resultL, long& resultM, double& maxDelta) const {
	const size_t sourceIndex = geoLocation.getGrid().getIndex(k, l, m);
	const double sourceLat = geoLocation.getLat(sourceIndex);
	const double sourceLon = geoLocation.getLon(sourceIndex);
	const double delta = TiePointInterpolator<double>::cosineDistance(targetLon, targetLat, sourceLon, sourceLat);

	if (delta > maxDelta) {
		resultK = k;
		resultL = l;
		resultM = m;
		maxDelta = delta;
	}
}

size_t PixelFinder::computeTiePointCount(long sizeK, long sizeL, long sizeM) const {
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
