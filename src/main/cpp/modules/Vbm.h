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
	Segment* collocatedSegment;
	Context* context;
	LookupTable<double>* synLutRhoAtm;
	LookupTable<double>* synLutOlcRatm;
	LookupTable<double>* synLutSlnRatm;
	LookupTable<double>* synLutT;
	const valarray<double>* synCo3;

	LookupTable<double>* vgtLutRhoAtm;
	LookupTable<double>* vgtLutRAtm;
	LookupTable<double>* vgtLutT;
	LookupTable<double>* vgtLutSolarIrradiance;
	valarray<LookupTable<double>*> vgtBSrfLuts;
	const valarray<double>* vgtCo3;

	void downscale(const Pixel& p, valarray<double>& surfReflNadirSyn);
	static double surfaceReflectance(double ozone, double vza, double sza, double solarIrradiance, double radiance,
	        double co3, double rhoAtm, double rAtm, double tSun, double tView);
	void performHyperspectralInterpolation(const valarray<double>& surfaceReflectances, valarray<double>& hyperSpectralReflectances);
	double linearInterpolation(const valarray<double>& surfaceReflectances, double wavelength);
	void performHyperspectralUpscaling(const valarray<double>& hyperSpectralReflectances, const Pixel& p, valarray<double>& toaReflectances);
	double hyperspectralUpscale(double sza, double vzaOlc, double ozone, double hyperSpectralReflectance, double co3, double rhoAtm, double rAtm, double tSun, double tView);
	void performHyperspectralFiltering(valarray<double>& toaReflectances, valarray<double>& filteredRToa);
	uint16_t getFlagsAndFills(Pixel & p, valarray<double> & vgtToaReflectances);
	void cleanup(valarray<double>& surfaceReflectances, valarray<double>& hyperSpectralReflectances, valarray<double>& toaReflectances, valarray<double>& vgtToaReflectances);
	void setupPixel(Pixel& p, size_t index);
	void setValues(uint16_t flags, valarray<double>& vgtToaReflectances);
};

#endif /* VBM_H_ */
