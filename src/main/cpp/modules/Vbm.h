/*
 * Vbm.h
 *
 *  Created on: Nov 17, 2011
 *      Author: thomasstorm
 */

#ifndef VBM_H_
#define VBM_H_

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

	void prepareAccessors(Context& context);
	void prepareAuxdata(Context& context);
	void prepareTiePointData(Context& context);
    void addVariables(Context& context);

	void setPixel(Pixel& p, size_t index);
	void performDownscaling(const Pixel& p, valarray<double>& synSurfaceReflectances, valarray<double>& coordinates, valarray<double>& f, valarray<double>& w);
	void performHyperspectralInterpolation(const valarray<double>& synWavelengths, const valarray<double>& synSurfaceReflectances, valarray<double>& hypSurfaceReflectances);
	void performHyperspectralUpscaling(const valarray<double>& hypSurfaceReflectances, const Pixel& p, valarray<double>& hypToaReflectances, valarray<double>& coordinates, valarray<double>& f, valarray<double>& w);
	void performHyperspectralFiltering(const valarray<double>& hypToaReflectances, valarray<double>& vgtToaReflectances) const;

	uint8_t performQualityFlagging(Pixel& p, valarray<double>& vgtToaReflectances) const;
	void setValues(const size_t index, const uint8_t flags, const valarray<double>& vgtToaReflectances);

    template<class T>
    static void copy(const std::valarray<T>& s, std::valarray<T>& t) {
        t.resize(s.size());
        std::copy(&s[0], &s[s.size()], &t[0]);
    }

    static double aerosolOpticalThickness(double lat);

	static double surfaceReflectance(double nO3, double vza, double sza, double f0, double ltoa,
	        double cO3, double rho, double ratm, double ts, double tv);

	static double toaReflectance(double nO3, double airMass, double surfaceReflectance,
	        double cO3, double rho, double ratm, double ts, double tv);

	uint16_t amin;

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
    Accessor* synLatitudeAccessor;
    Accessor* synLongitudeAccessor;

    valarray<Accessor*> synRadianceAccessors;
    valarray<Accessor*> synSolarIrradianceAccessors;

    Accessor* vgtB0Accessor;
    Accessor* vgtB2Accessor;
    Accessor* vgtB3Accessor;
    Accessor* vgtMirAccessor;
    Accessor* vgtFlagsAccessor;
    valarray<Accessor*> vgtReflectanceAccessors;

    shared_ptr<TiePointInterpolator<double> > tiePointInterpolatorOlc;
    shared_ptr<TiePointInterpolator<double> > tiePointInterpolatorSln;

    valarray<double> szaOlcTiePoints;
    valarray<double> saaOlcTiePoints;
    valarray<double> vzaOlcTiePoints;
    valarray<double> vzaSlnTiePoints;

    valarray<double> waterVapourTiePoints;
    valarray<double> airPressureTiePoints;
    valarray<double> ozoneTiePoints;

    valarray<double> rho;
    valarray<double> ratm;
    valarray<double> ts;
    valarray<double> tv;

	static const double D2R = 3.14159265358979323846 / 180.0;
};

inline void Vbm::setValues(const size_t index, const uint8_t flags, const valarray<double>& vgtToaReflectances) {
    vgtFlagsAccessor->setUShort(index, flags);
    for (size_t i = 0; i < vgtReflectanceAccessors.size(); i++) {
        if (vgtToaReflectances[i] != Constants::FILL_VALUE_DOUBLE) {
            vgtReflectanceAccessors[i]->setDouble(index, vgtToaReflectances[i]);
        } else {
            vgtReflectanceAccessors[i]->setFillValue(index);
        }
    }
}

#endif /* VBM_H_ */
