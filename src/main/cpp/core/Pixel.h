/*
 * Pixel.h
 *
 *  Created on: 20.10.2011
 *      Author: Ralf Quast, Thomas Storm
 */

#ifndef PIXEL_H_
#define PIXEL_H_

#include <iostream>
#include <valarray>

struct Pixel {
	Pixel();
    Pixel(const Pixel& q);
    ~Pixel();

    void assign(const Pixel& q);

    std::valarray<double> radiances;
    std::valarray<double> solarIrradiances;

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

    double angstromExponent;
    double aot;
    double aotError;
    double aotFiltered;
    double aotErrorFiltered;
    unsigned short flags;
    unsigned char aerosolModel;

    double c1;
    double c2;
    std::valarray<double> nus;
    std::valarray<double> omegas;

    double errorMetric;
    double a;
};

std::ostream& operator<<(std::ostream& os, Pixel& p);

std::istream& operator>>(std::istream& is, Pixel& p);

#endif /* PIXEL_H_ */
