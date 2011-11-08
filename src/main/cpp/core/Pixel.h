/*
 * Pixel.h
 *
 *  Created on: 20.10.2011
 *      Author: thomasstorm
 */

#ifndef IPIXEL_H_
#define IPIXEL_H_

#include <algorithm>
#include <valarray>

using std::copy;
using std::valarray;

struct Pixel {
	Pixel() : k(0), l(0), m(0), index(0), radiances(30), sdrs(30), solarIrradiances(30), cO3(30), nu(2), omega(6) {

	}

    Pixel(long k, long l, long m, size_t index) :
            k(k), l(l), m(m), index(index), radiances(30), sdrs(30), solarIrradiances(30), cO3(30), nu(2), omega(6) {
    }

    Pixel(const Pixel& q) : k(q.k), l(q.l), m(q.m), index(q.index), radiances(q.radiances), sdrs(q.sdrs), solarIrradiances(q.solarIrradiances), cO3(q.cO3), nu(q.nu), omega(q.omega) {
        alpha550 = q.alpha550;
        tau550 = q.tau550;
        tau550err = q.tau550err;
        tau550Filtered = q.tau550Filtered;
        tau550errFiltered = q.tau550errFiltered;
        synFlags = q.synFlags;
        amin = q.amin;

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

        c1 = q.c1;
        c2 = q.c2;

        E2 = q.E2;
    }

    ~Pixel() {
    }

    void assign(const Pixel& q) {
    	k = q.k;
    	l = q.l;
    	m = q.m;
    	index = q.index;

        alpha550 = q.alpha550;
        tau550 = q.tau550;
        tau550err = q.tau550err;
        tau550Filtered = q.tau550Filtered;
        tau550errFiltered = q.tau550errFiltered;
        synFlags = q.synFlags;
        amin = q.amin;

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

        c1 = q.c1;
        c2 = q.c2;

        E2 = q.E2;

        copy(&q.nu[0], &q.nu[q.nu.size()], &nu[0]);
        copy(&q.omega[0], &q.omega[q.omega.size()], &omega[0]);
        copy(&q.solarIrradiances[0], &q.solarIrradiances[q.solarIrradiances.size()], &solarIrradiances[0]);
        copy(&q.radiances[0], &q.radiances[q.radiances.size()], &radiances[0]);
        copy(&q.sdrs[0], &q.sdrs[q.sdrs.size()], &sdrs[0]);
        copy(&q.cO3[0], &q.cO3[q.cO3.size()], &cO3[0]);
    }

    long k;
    long l;
    long m;
    size_t index;

    valarray<double> radiances;
    valarray<double> sdrs;
    valarray<double> solarIrradiances;
    valarray<double> cO3;

    double alpha550;
    double tau550;
    double tau550err;
    double tau550Filtered;
    double tau550errFiltered;
    uint16_t synFlags;
    uint8_t amin;

    double lon;
    double lat;
    double sza;
    double saa;
    double vzaOlc;
    double vzaSln;
    double vzaSlo;
    double vaaOlc;
    double vaaSln;
    double vaaSlo;
    double ozone;
    double airPressure;
    double waterVapour;

    double c1;
    double c2;
    valarray<double> nu;
    valarray<double> omega;

    double E2;
};

#endif /* IPIXEL_H_ */
