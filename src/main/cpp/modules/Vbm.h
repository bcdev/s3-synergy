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
#include "../core/TiePointInterpolator.h"

class Vbm: public BasicModule {
public:
	Vbm();
	virtual ~Vbm();

	void start(Context& context);
	void process(Context& context);

private:
	friend class VbmTest;

	uint16_t amin;
	Segment* collocatedSegment;
	Segment* olciInfoSegment;

	LookupTable<double>* synLutRhoAtm;
	LookupTable<double>* synLutOlcRatm;
	LookupTable<double>* synLutSlnRatm;
	LookupTable<double>* synLutT;
	const valarray<double>* synCo3;

	LookupTable<double>* vgtLutRhoAtm;
	LookupTable<double>* vgtLutRAtm;
	LookupTable<double>* vgtLutT;
	valarray<valarray<double> > vgtBSurfaceReflectanceWeights;
	valarray<double> vgtSolarIrradiances;
	const valarray<double>* vgtCo3;
	valarray<double> wavelengths;

    Accessor* synOzoneAccessor;
    Accessor* synLatitudeAccessor;
    Accessor* synLongitudeAccessor;

    valarray<Accessor*> synRadianceAccessors;
    valarray<Accessor*> synSolarIrradianceAccessors;

    Accessor* vgtFlagsAccessor;
    Accessor* vgtB0Accessor;
    Accessor* vgtB2Accessor;
    Accessor* vgtB3Accessor;
    Accessor* vgtMirAccessor;

    shared_ptr<TiePointInterpolator<double> > tiePointInterpolatorOlc;
    shared_ptr<TiePointInterpolator<double> > tiePointInterpolatorSln;

    valarray<double> szaOlcTiePoints;
    valarray<double> saaOlcTiePoints;
    valarray<double> vzaOlcTiePoints;
    valarray<double> vzaSlnTiePoints;

    valarray<double> waterVapourTiePoints;
    valarray<double> airPressureTiePoints;
    valarray<double> ozoneTiePoints;

    valarray<double> coordinates;

    valarray<double> vgtRhoAtm;
    valarray<double> vgtRAtm;
    valarray<double> vgtTSun;
    valarray<double> vgtTView;

    valarray<double> synRhoAtm;
    valarray<double> synRAtmOlc;
    valarray<double> synTSun;
    valarray<double> synTViewOlc;

    valarray<double> synRAtmSln;
    valarray<double> synTViewSln;

    map<size_t, size_t> wavelengthIndices_0;
    map<size_t, size_t> wavelengthIndices_1;

    template<class T>
    static T cube(T x) {
        return x * x * x;
    }

    template<class T>
    static void copy(const std::valarray<T>& s, std::valarray<T>& t) {
        t.resize(s.size());
        std::copy(&s[0], &s[s.size()], &t[0]);
    }

    static double computeT550(double lat) {
        return 0.2 * (std::cos(lat) - 0.25) * cube(std::sin(lat + M_PI_2)) + 0.05;
    }

	static double surfaceReflectance(double ozone, double vza, double sza, double solarIrradiance, double radiance,
	        double co3, double rhoAtm, double rAtm, double tSun, double tView);
	static double hyperspectralUpscale(double ozone, double vza, double sza, double hyperSpectralReflectance,
	        double co3, double rhoAtm, double rAtm, double tSun, double tView);
	static double getSlnWavelength(size_t channel);

	void prepareAccessors();
	void prepareAuxdata(Context& context);
	void prepareTiePointData(Context& context);
    void addVariables(Context& context);

	void computeChannelWavelengths(long k, long m, valarray<double>& channelWavelengths) const;
    void computeInterpolationIndices(const valarray<double>& channelWavelengths, const valarray<double>& surfaceReflectances);

	void setupPixel(Pixel& p, size_t index);
	void performDownscaling(const Pixel& p, valarray<double>& surfReflNadirSyn);
	void performHyperspectralInterpolation(const valarray<double>& channelWavelengths, const valarray<double>& surfaceReflectances, valarray<double>& hyperSpectralReflectances);
	double linearInterpolation(const valarray<double>& x, const valarray<double>& y, const size_t index);
	void performHyperspectralUpscaling(const valarray<double>& hyperSpectralReflectances, const Pixel& p, valarray<double>& toaReflectances);
	void performHyperspectralFiltering(const valarray<double>& toaReflectances, valarray<double>& filteredRToa) const;

	uint8_t getFlagsAndFill(Pixel& p, valarray<double>& vgtToaReflectances) const;
	void setValues(const size_t index, const uint8_t flags, const valarray<double>& vgtToaReflectances);
};

#endif /* VBM_H_ */
