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

#ifndef AER_H_
#define AER_H_

#include "../core/Pixel.h"

#include "../modules/BasicModule.h"

#include "../util/Min.h"
#include "../util/MultiMin.h"
#include "../util/UnivariateFunction.h"
#include "../util/ErrorMetric.h"

/**
 * The SYN L2 aerosol retrieval module (AER).
 */
class Aer: public BasicModule {
public:
	Aer();
	virtual ~Aer();

	void start(Context& context);
	void process(Context& context);

private:
	friend class AerTest;

	void getPixels(Context& context, valarray<Pixel>& pixels) const;
	void putPixels(const valarray<Pixel>& pixels, long firstL, long lastL) const;
	void readAuxiliaryData(Context& context);
	void retrieveAerosolProperties(Pixel& p, Pixel& q, ErrorMetric& em);

	static double aerosolOpticalThickness(double lat);

	Segment* averagedSegment;
	const Grid* averagedGrid;

	double kappa;
	valarray<int16_t> amins;
	valarray<double> aerosolAngstromExponents;

	static const double D2R = 3.14159265358979323846 / 180.0;
};

#endif /* AER_H_ */
