/*
 * Vac.h
 *
 *  Created on: Jan 09, 2012
 *      Author: thomasstorm
 */

#ifndef VAC_H_
#define VAC_H_

#include "../core/TiePointInterpolator.h"
#include "../modules/BasicModule.h"

class Vac : public BasicModule {
public:
	Vac();
	virtual ~Vac();

	void start(Context& context);
	void process(Context& context);


private:
	friend class VacTest;

	struct Pixel {
		Pixel() : reflectances(4) {
		}

		~Pixel() {
		}

	    valarray<double> reflectances;

	    double lon;
	    double lat;
	    double sza;
	    double saa;
	    double vza;
	    double vaa;

	    double ozone;
	    double airPressure;
	    double waterVapour;

	    double aot;
	    uint8_t aerosolModel;
	    uint8_t vgtFlags;
	};

	Segment* collocatedSegment;
	Segment* geoSegment;

	valarray<Accessor*> vgtReflectanceAccessors;
    Accessor* latAccessor;
    Accessor* lonAccessor;
    Accessor* aotAccessor;

    TiePointInterpolator<double>* tiePointInterpolatorOlc;

    valarray<double> szaOlcTiePoints;
    valarray<double> vzaOlcTiePoints;
    valarray<double> airPressureTiePoints;
    valarray<double> waterVapourTiePoints;

	valarray<double> cO3;
	uint16_t aerosolModel;

	LookupTable<double>* lutRhoAtm;
	LookupTable<double>* lutRatm;
	LookupTable<double>* lutT;

    valarray<double> coordinates;
    valarray<double> tpiWeights;
    valarray<size_t> tpiIndices;
    valarray<double> f;

	void setupAccessors(Context& context);
	void prepareAuxdata(Context& context);
	void prepareTiePointData(Context& context);
	void setupPixel(Pixel& pixel, size_t index, size_t geoIndex);
	void computeSDR(Pixel& pixel, valarray<double>& w);
    template<class T>
    static void copy(const valarray<T>& s, valarray<T>& t);
};

template<class T>
void Vac::copy(const std::valarray<T>& s, std::valarray<T>& t) {
    using std::copy;

    if (t.size() != s.size()) {
        t.resize(s.size());
    }
    copy(&s[0], &s[s.size()], &t[0]);
}

#endif /* VAC_H_ */
