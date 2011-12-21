/*
 * DirectLocator.cpp
 *
 *  Created on: Dec 20, 2011
 *      Author: ralf
 */

#include <cmath>

#include "DirectLocator.h"

using std::asin;
using std::atan2;
using std::cos;
using std::sin;

Rotation::Rotation(double lat, double lon) {
	const double u = toRadians(lon);
	const double v = toRadians(lat);
	const double w = 0.0;

	const double cu = cos(u);
	const double cv = cos(v);
	const double cw = 1.0;
	const double su = sin(u);
	const double sv = sin(v);
	const double sw = 0.0;

	a11 = cu * cv;
	a12 = su * cv;
	a13 = sv;

	a21 = sw * (cu * sv) - su * cw;
	a22 = cw * cu + sw * (su * sv);
	a23 = -sw * cv;

	a31 = sw * -su - cw * (cu * sv);
	a32 = sw * cu - cw * (su * sv);
	a33 = cw * cv;
}

Rotation::~Rotation() {
}

void Rotation::transform(double& lat, double& lon) const {
	const double u = toRadians(lon);
	const double v = toRadians(lat);

	const double w = cos(v);
	const double x = cos(u) * w;
	const double y = sin(u) * w;
	const double z = sin(v);

	const double x2 = a11 * x + a12 * y + a13 * z;
	const double y2 = a21 * x + a22 * y + a23 * z;
	const double z2 = a31 * x + a32 * y + a33 * z;

	lat = toDegrees(asin(z2));
	lon = toDegrees(atan2(y2, x2));
}

void Rotation::inverseTransform(double& lat, double& lon) const {
    const double u = toRadians(lon);
    const double v = toRadians(lat);

    const double w = cos(v);
    const double x = cos(u) * w;
    const double y = sin(u) * w;
    const double z = sin(v);

    const double x2 = a11 * x + a21 * y + a31 * z;
    const double y2 = a12 * x + a22 * y + a32 * z;
    const double z2 = a13 * x + a23 * y + a33 * z;

    lat = toDegrees(asin(z2));
    lon = toDegrees(atan2(y2, x2));
}

DirectLocator::DirectLocator(const Accessor& latAccessor, const Accessor& lonAccessor, const Grid& grid) :
		latAccessor(latAccessor), lonAccessor(lonAccessor), grid(grid) {
}

DirectLocator::~DirectLocator() {
}

bool DirectLocator::getLocation(long k, double x, double y, double& lat, double& lon) const {
	const long w = grid.getSizeM();
	const long h = grid.getSizeL();

	long l = (long) std::floor(y);
	long m = (long) std::floor(x);

	if (m >= 0 && m < w && l >= 0 && l < h) {
		if ((m > 0 && x - m < 0.5) || m == w - 1) {
			m -= 1;
		}
		if ((l > 0 && y - l < 0.5) || l == h - 1) {
			l -= 1;
		}
		const long x1 = m + 1;
		const long y1 = l + 1;
		if (x1 < w && y1 < h) {
			const double wx = x - (m + 0.5);
			const double wy = y - (l + 0.5);
			interpolate(k, l, m, wx, wy, lat, lon);
		} else {
			const long index = grid.getIndex(k, l, m);
			lat = latAccessor.getDouble(index);
			lon = lonAccessor.getDouble(index);
		}
		return true;
	}

	return false;
}

void DirectLocator::interpolate(long k, long y0, long x0, double wx, double wy, double& lat, double& lon) const {
	const long x1 = x0 + 1;
	const long y1 = y0 + 1;

	const long i00 = grid.getIndex(k, y0, x0);
	const long i01 = grid.getIndex(k, y0, x1);
	const long i10 = grid.getIndex(k, y1, x0);
	const long i11 = grid.getIndex(k, y1, x1);

	if (lonAccessor.isFillValue(i00) || lonAccessor.isFillValue(i01) || lonAccessor.isFillValue(i10) || lonAccessor.isFillValue(i11) || latAccessor.isFillValue(i00) || latAccessor.isFillValue(i01)
			|| latAccessor.isFillValue(i10) || latAccessor.isFillValue(i11)) {
		if (wy < 0.5) {
			if (wx < 0.5) {
				lat = latAccessor.getDouble(i00);
				lon = lonAccessor.getDouble(i00);
			} else {
				lat = latAccessor.getDouble(i01);
				lon = lonAccessor.getDouble(i01);
			}
		} else {
			if (wx < 0.5) {
				lat = latAccessor.getDouble(i10);
				lon = lonAccessor.getDouble(i10);
			} else {
				lat = latAccessor.getDouble(i11);
				lon = lonAccessor.getDouble(i11);
			}
		}
	} else {
		double lats[4];
		double lons[4];

		lats[0] = latAccessor.getDouble(i00);
		lats[1] = latAccessor.getDouble(i01);
		lats[2] = latAccessor.getDouble(i10);
		lats[3] = latAccessor.getDouble(i11);

		lons[0] = lonAccessor.getDouble(i00);
		lons[1] = lonAccessor.getDouble(i01);
		lons[2] = lonAccessor.getDouble(i10);
		lons[3] = lonAccessor.getDouble(i11);

		const Rotation r(lats[0], lons[0]);
		for (size_t i = 0; i < 4; i++) {
			r.transform(lats[i], lons[i]);
		}
		lat = interpolate(wx, wy, lats[0], lats[1], lats[2], lats[3]);
		lon = interpolate(wx, wy, lons[0], lons[1], lons[2], lons[3]);
		for (size_t i = 0; i < 4; i++) {
			r.inverseTransform(lats[i], lons[i]);
		}
	}
}

