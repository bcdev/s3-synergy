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
	Pixel() : radiances(30), solarIrradiances(30), nu(2), omega(6) {

	}

    Pixel(long k, long l, long m, size_t index) :
            radiances(30), solarIrradiances(30), nu(2), omega(6) {
    }

    Pixel(const Pixel& q) : radiances(q.radiances), solarIrradiances(q.solarIrradiances), nu(q.nu), omega(q.omega) {
        alpha550 = q.alpha550;
        tau550 = q.tau550;
        tau550Error = q.tau550Error;
        tau550Filtered = q.tau550Filtered;
        tau550ErrorFiltered = q.tau550ErrorFiltered;
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

        minErrorMetric = q.minErrorMetric;
    }

    ~Pixel() {
    }

    void assign(const Pixel& q) {
        copy(&q.radiances[0], &q.radiances[q.radiances.size()], &radiances[0]);
        copy(&q.solarIrradiances[0], &q.solarIrradiances[q.solarIrradiances.size()], &solarIrradiances[0]);

        alpha550 = q.alpha550;
        tau550 = q.tau550;
        tau550Error = q.tau550Error;
        tau550Filtered = q.tau550Filtered;
        tau550ErrorFiltered = q.tau550ErrorFiltered;
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
        copy(&q.nu[0], &q.nu[q.nu.size()], &nu[0]);
        copy(&q.omega[0], &q.omega[q.omega.size()], &omega[0]);

        minErrorMetric = q.minErrorMetric;
    }

    valarray<double> radiances;
    valarray<double> solarIrradiances;

    double alpha550;
    double tau550;
    double tau550Error;
    double tau550Filtered;
    double tau550ErrorFiltered;
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

    double minErrorMetric;
};

#endif /* IPIXEL_H_ */
