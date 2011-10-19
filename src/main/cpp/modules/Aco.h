/*
 * Aco.h
 *
 *  Created on: Aug 24, 2011
 *      Author: ralf
 */

#ifndef ACO_H_
#define ACO_H_

#include "../modules/BasicModule.h"
#include "../core/LookupTable.h"

class Aco: public BasicModule {
public:
	Aco();
	virtual ~Aco();

	void start(Context& context);
	void stop(Context& context);
	void process(Context& context);
private:
	static double ozoneTransmission(const ScalarLookupTable<double>& lut, double sza, double vza, double nO3, double channel);
	static double surfaceReflectance(double rtoa, double ratm, double ts, double tv, double rho, double tO3);
	static double toaReflectance(double ltoa, double f0, double sza);

	static const double PI = 3.14159265358979323846;
	static const double D2R = 3.14159265358979323846 / 180.0;
};

#endif /* ACO_H_ */
