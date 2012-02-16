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

#ifndef PIXEL_H_
#define PIXEL_H_

#include <iostream>
#include <valarray>

struct Pixel {
	Pixel();
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
    unsigned short flags;
    unsigned char aerosolModel;

    double c1;
    double c2;
    std::valarray<double> nus;
    std::valarray<double> omegas;

    double errorMetric;
};

std::ostream& operator<<(std::ostream& os, const Pixel& p);

std::istream& operator>>(std::istream& is, Pixel& p);

#endif /* PIXEL_H_ */
