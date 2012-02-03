/*
 * Vac.h
 *
 *  Created on: Jan 09, 2012
 *      Author: thomasstorm
 */

#ifndef VAC_H_
#define VAC_H_

#include <cmath>

#include "../modules/BasicModule.h"

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
