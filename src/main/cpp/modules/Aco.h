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

#ifndef ACO_H_
#define ACO_H_

#include "../modules/BasicModule.h"

class Aco: public BasicModule {
public:
	Aco();
	virtual ~Aco();

	void start(Context& context);
	void process(Context& context);

	static double ozoneTransmission(double cO3, double sza, double vza, double nO3);
	static double surfaceReflectance(double rtoa, double ratm, double ts, double tv, double rho, double tO3);
	static double toaReflectance(double ltoa, double f0, double sza);

private:
	static const double PI = 3.14159265358979323846;
	static const double D2R = 3.14159265358979323846 / 180.0;
};

#endif /* ACO_H_ */
