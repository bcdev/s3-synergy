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

#ifndef VAC_H_
#define VAC_H_

#include <cmath>

#include "../modules/BasicModule.h"

/**
 * The VGT atmospheric correction module (VAC).
 */
class Vac : public BasicModule {
public:
	Vac();
	virtual ~Vac();

	void start(Context& context);
	void process(Context& context);

private:
	friend class VacTest;

	static double airMass(double sza, double vza) {
		using std::cos;

		return 0.5 * (1.0 / cos(sza * D2R) + 1.0 / cos(vza * D2R));
	}

	static double atmCmToDU(double ozone) {
		return ozone * 1000.0;
	}

	static double ndvi(double b1, double b2) {
		return (b1 - b2) / (b1 + b2);
	}

	static const double D2R = 3.14159265358979323846 / 180.0;
};

#endif /* VAC_H_ */
