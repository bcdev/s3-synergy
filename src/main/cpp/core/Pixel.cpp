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

#include "Pixel.h"

Pixel::Pixel() : radiances(30), solarIrradiances(30), nus(2), omegas(6) {
}

Pixel::~Pixel() {
}

void Pixel::assign(const Pixel& q) {
	using std::copy;

	copy(&q.radiances[0], &q.radiances[q.radiances.size()], &radiances[0]);
	copy(&q.solarIrradiances[0], &q.solarIrradiances[q.solarIrradiances.size()], &solarIrradiances[0]);

	lon = q.lon;
	lat = q.lat;

	sza = q.sza;
	saa = q.saa;
	vzaOlc = q.vzaOlc;
	vzaSln = q.vzaSln;
	vzaSlo = q.vzaSlo;
	vaaOlc = q.vaaOlc;
	vaaSln = q.vaaSln;
	vaaSlo = q.vaaSlo;

	ozone = q.ozone;
	airPressure = q.airPressure;
	waterVapour = q.waterVapour;

	angstromExponent = q.angstromExponent;
	aot = q.aot;
	aotError = q.aotError;
	flags = q.flags;
	aerosolModel = q.aerosolModel;

	c1 = q.c1;
	c2 = q.c2;
	copy(&q.nus[0], &q.nus[q.nus.size()], &nus[0]);
	copy(&q.omegas[0], &q.omegas[q.omegas.size()], &omegas[0]);

	errorMetric = q.errorMetric;
}

std::ostream& operator<<(std::ostream& s, const Pixel& p) {
	for (size_t i = 0; i < p.radiances.size(); i++) {
		s << p.radiances[i] << " ";
	}
	for (size_t i = 0; i < p.solarIrradiances.size(); i++) {
		s << p.solarIrradiances[i] << " ";
	}
	s << p.lat << " ";
	s << p.lon << " ";

	s << p.sza << " ";
	s << p.saa << " ";
	s << p.vzaOlc << " ";
	s << p.vzaSln << " ";
	s << p.vzaSlo << " ";
	s << p.vaaOlc << " ";
	s << p.vaaSln << " ";
	s << p.vaaSlo << " ";

	s << p.ozone << " ";
	s << p.airPressure << " ";
	s << p.waterVapour << " ";

	s << p.angstromExponent << " ";
	s << p.aot << " ";
	s << p.aotError << " ";
	s << p.flags << " ";
	s << (unsigned short) p.aerosolModel << " ";

	s << p.c1 << " ";
	s << p.c2 << " ";

	for (size_t i = 0; i < p.nus.size(); i++) {
		s << p.nus[i] << " ";
	}
	for (size_t i = 0; i < p.omegas.size(); i++) {
		s << p.omegas[i] << " ";
	}

	s << p.errorMetric << std::endl;

	return s;
}

std::istream& operator>>(std::istream& s, Pixel& p) {
	for (size_t i = 0; i < p.radiances.size(); i++) {
		s >> p.radiances[i];
	}
	for (size_t i = 0; i < p.solarIrradiances.size(); i++) {
		s >> p.solarIrradiances[i];
	}
	s >> p.lat;
	s >> p.lon;

	s >> p.sza;
	s >> p.saa;
	s >> p.vzaOlc;
	s >> p.vzaSln;
	s >> p.vzaSlo;
	s >> p.vaaOlc;
	s >> p.vaaSln;
	s >> p.vaaSlo;

	s >> p.ozone;
	s >> p.airPressure;
	s >> p.waterVapour;

	s >> p.angstromExponent;
	s >> p.aot;
	s >> p.aotError;
	s >> p.flags;
	unsigned short aerosolModel;
	s >> aerosolModel;
	p.aerosolModel = (unsigned char) aerosolModel;

	s >> p.c1;
	s >> p.c2;

	for (size_t i = 0; i < p.nus.size(); i++) {
		s >> p.nus[i];
	}
	for (size_t i = 0; i < p.omegas.size(); i++) {
		s >> p.omegas[i];
	}

	s >> p.errorMetric;

	return s;
}

