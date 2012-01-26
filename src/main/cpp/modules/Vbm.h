/*
 * Vbm.h
 *
 *  Created on: Nov 17, 2011
 *      Author: thomasstorm
 */

#ifndef VBM_H_
#define VBM_H_

#include "../core/TiePointInterpolator.h"

#include "BasicModule.h"

class Vbm: public BasicModule {
public:
	Vbm();
	virtual ~Vbm();

	void start(Context& context);
	void process(Context& context);

private:
	friend class VbmTest;

	struct Pixel {
		Pixel() : radiances(21), solarIrradiances(21) {
		}

		~Pixel() {
		}

	    valarray<double> radiances;
	    valarray<double> solarIrradiances;
	    uint16_t synFlags;

	    double lon;
	    double lat;
	    double sza;
	    double saa;
	    double vzaOlc;
	    double vaaOlc;
	    double vzaSln;
	    double vaaSln;

	    double ozone;
	    double airPressure;
	    double waterVapour;

	    double aot;
	    uint8_t aerosolModel;
	    uint8_t vgtFlags;
	};

	void prepareAccessors(Context& context);
	void prepareAuxdata(Context& context);
	void prepareTiePointData(Context& context);
    void addTargetVariables(Context& context);

	void initPixel(Pixel& p, size_t index, size_t geoIndex, valarray<double>& tpiWeights, valarray<size_t>& tpiIndexes);
	void performDownscaling(const Pixel& p, valarray<double>& synSurfaceReflectances, valarray<double>& coordinates, valarray<double>& f, valarray<double>& w);
	void performHyperspectralInterpolation(const valarray<double>& synWavelengths, const valarray<double>& synSurfaceReflectances, valarray<double>& hypSurfaceReflectances);
	void performHyperspectralUpscaling(const valarray<double>& hypSurfaceReflectances, const Pixel& p, valarray<double>& hypToaReflectances, valarray<double>& coordinates, valarray<double>& f, valarray<double>& w);
	void performHyperspectralFiltering(const valarray<double>& hypToaReflectances, valarray<double>& vgtToaReflectances) const;

	uint8_t performQualityFlagging(Pixel& p, const valarray<double>& vgtToaReflectances) const;
	void setValues(size_t index, Pixel& p, uint8_t flags, const valarray<double>& vgtToaReflectances);

    template<class T>
    static void copy(const valarray<T>& s, valarray<T>& t);

    static double aerosolOpticalThickness(double lat);

	static double surfaceReflectance(double nO3, double vza, double sza, double f0, double ltoa,
	        double cO3, double rho, double ratm, double ts, double tv);

	static double toaReflectance(double nO3, double airMass, double surfaceReflectance,
	        double cO3, double rho, double ratm, double ts, double tv);

	uint16_t aerosolModel;

	LookupTable<double>* synLutRho;
	LookupTable<double>* olcLutRatm;
	LookupTable<double>* slnLutRatm;
	LookupTable<double>* synLutT;
	valarray<double> synCO3;

	LookupTable<double>* hypLutRho;
	LookupTable<double>* hypLutRatm;
	LookupTable<double>* hypLutT;

	valarray<valarray<double> > hypSpectralResponses;
	valarray<double> hypCO3;
	valarray<double> hypSolarIrradiances;
	valarray<double> hypWavelengths;

    Accessor* synOzoneAccessor;
    Accessor* latAccessor;
    Accessor* lonAccessor;

    valarray<Accessor*> synRadianceAccessors;
    valarray<Accessor*> synSolarIrradianceAccessors;
    Accessor* synFlagsAccessor;
    valarray<Accessor*> vgtReflectanceAccessors;
    Accessor* vgtFlagsAccessor;
    Accessor* vgtAgAccessor;
    Accessor* vgtOgAccessor;
    Accessor* vgtWvgAccessor;
    Accessor* vgtSaaAccessor;
    Accessor* vgtSzaAccessor;
    Accessor* vgtVaaAccessor;
    Accessor* vgtVzaAccessor;

    shared_ptr<TiePointInterpolator<double> > tiePointInterpolatorOlc;
    shared_ptr<TiePointInterpolator<double> > tiePointInterpolatorSln;

    valarray<double> szaOlcTiePoints;
    valarray<double> saaOlcTiePoints;
    valarray<double> vzaOlcTiePoints;
    valarray<double> vaaOlcTiePoints;
    valarray<double> vzaSlnTiePoints;
    valarray<double> vaaSlnTiePoints;

    valarray<double> airPressureTiePoints;
    valarray<double> ozoneTiePoints;
    valarray<double> waterVapourTiePoints;

    valarray<double> rho;
    valarray<double> ratm;
    valarray<double> ts;
    valarray<double> tv;

	static const double D2R = 3.14159265358979323846 / 180.0;

	static const size_t HYP_CHANNEL_COUNT = 914;
	static const size_t SYN_CHANNEL_COUNT = 21;
	static const size_t VGT_CHANNEL_COUNT = 4;
};

template<class T>
void Vbm::copy(const std::valarray<T>& s, std::valarray<T>& t) {
	using std::copy;

	if (t.size() != s.size()) {
		t.resize(s.size());
	}
    copy(&s[0], &s[s.size()], &t[0]);
}

#endif /* VBM_H_ */
