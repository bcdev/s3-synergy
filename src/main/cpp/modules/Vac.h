/*
 * Vac.h
 *
 *  Created on: Jan 09, 2012
 *      Author: thomasstorm
 */

#ifndef VAC_H_
#define VAC_H_

#include "../core/Pixel.h"
#include "../modules/BasicModule.h"

class Vac : public BasicModule {
public:
	Vac();
	virtual ~Vac();

	void start(Context& context);
	void process(Context& context);

private:
	friend class VacTest;

	Segment* collocatedSegment;
	valarray<Accessor*> vgtReflectanceAccessors;
	valarray<double> cO3;
	LookupTable<double>* lutRhoAtm;
	LookupTable<double>* lutRatm;
	LookupTable<double>* lutT;

    valarray<double> coordinates;
    valarray<double> f;

	void setupAccessors();
	void prepareAuxdata(Context& context);
	void setupPixel(Pixel& pixel, long index);
	void computeSDR(Pixel& pixel, valarray<double>& w);
};

#endif /* VAC_H_ */
