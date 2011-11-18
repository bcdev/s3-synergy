/*
 * Vbm.h
 *
 *  Created on: Nov 17, 2011
 *      Author: thomasstorm
 */

#ifndef VBM_H_
#define VBM_H_

#include <math.h>

#include "../modules/BasicModule.h"
#include "../core/Pixel.h"

class Vbm: public BasicModule {
public:
	Vbm();
	virtual ~Vbm();

	void start(Context& context);
	void stop(Context& context);
	void process(Context& context);

	static double computeT550(double lat) {
	    return 0.2 * (std::cos(lat) - 0.25) * cube(std::sin(lat + M_PI_2)) + 0.05;
	}

private:
	friend class VbmTest;
	static double cube(double x) {
	    return x * x * x;
	}

	uint16_t amin;
	valarray<double> cO3;
	Segment* collocatedSegment;
	Context* context;

	void downscale(const Pixel& p, uint16_t aerosolModelIndex, double t550, valarray<double>& surfReflNadirSyn);
	double surfaceReflectance(size_t i, uint16_t aerosolModelIndex, double t550, double waterVapour, double airPressure, double vzaOlc,
	        double sza, double vaaOlc, double saa, double solarIrradiance, double radiance, double rhoAtm, double rAtm, double tSun, double tView);
};

#endif /* VBM_H_ */
