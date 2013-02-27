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

#ifndef VBM_H_
#define VBM_H_

#include "../core/TiePointInterpolator.h"

#include "BasicModule.h"

/**
 * The VGT spectral band-mapping module (VBM).
 */
class Vbm: public BasicModule {
public:
	Vbm();
	virtual ~Vbm();

	void start(Context& context);
	void stop(Context& context);
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
	void performDownscaling(const Pixel& p, valarray<double>& synSurfaceReflectances, valarray<double>& coordinates, valarray<double>& rho, valarray<double>& ratm, valarray<double>& tv, valarray<double>& f, valarray<double>& w);
	void performHyperspectralInterpolation(const valarray<double>& synWavelengths, const valarray<double>& synSurfaceReflectances, valarray<double>& hypSurfaceReflectances);
	void performHyperspectralUpscaling(const valarray<double>& hypSurfaceReflectances, const Pixel& p, valarray<double>& hypToaReflectances, valarray<double>& coordinates, valarray<double>& rho, valarray<double>& ratm, valarray<double>& tv, valarray<double>& f, valarray<double>& w);
	void performHyperspectralFiltering(const valarray<double>& hypToaReflectances, valarray<double>& vgtToaReflectances) const;

	uint8_t performQualityFlagging(Pixel& p, const valarray<double>& vgtToaReflectances) const;
	void setValues(size_t index, Pixel& p, uint8_t flags, const valarray<double>& vgtToaReflectances, const valarray<double>& synSurfaceReflectances);

    template<class T>
    static void copy(const valarray<T>& s, valarray<T>& t);

    static double aerosolOpticalThickness(double lat);

	static double surfaceReflectance(double nO3, double vza, double sza, double f0, double ltoa,
	        double cO3, double rho, double ratm, double tv);

	static double toaReflectance(double nO3, double airMass, double surfaceReflectance,
	        double cO3, double rho, double ratm, double tv);

	static double duToAtmCm(double ozone) {
		return ozone * 0.001;
	}

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
    valarray<Accessor*> synSurfaceReflectanceAccessors;
    valarray<Accessor*> synSolarIrradianceAccessors;
    Accessor* synFlagsAccessor;
    valarray<Accessor*> vgtReflectanceAccessors;
    Accessor* vgtFlagsAccessor;
    Accessor* vgtAgAccessor;
    Accessor* vgtPgAccessor;
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

	static const double D2R = 3.14159265358979323846 / 180.0;

	static const size_t HYP_CHANNEL_COUNT = 833;
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
