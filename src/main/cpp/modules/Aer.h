/*
 * Aer.h
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
 */

#ifndef AER_H_
#define AER_H_

#include "../modules/BasicModule.h"
#include "../core/Pixel.h"
#include "../util/Min.h"
#include "../util/MultiMin.h"
#include "../util/UnivariateFunction.h"
#include "../util/ErrorMetric.h"

class Aer: public BasicModule {
public:
	Aer();
	virtual ~Aer();

	void start(Context& context);
	void process(Context& context);

private:
	friend class AerTest;

	void getPixels(Context& context, valarray<Pixel>& pixels) const;
	void putPixels(const valarray<Pixel>& pixels, long firstL, long lastL) const;
	void readAuxiliaryData(Context& context);
	void retrieveAerosolProperties(Pixel& p, Pixel& q, ErrorMetric& em);

	Segment* averagedSegment;
	const Grid* averagedGrid;

	double kappa;
	const valarray<int16_t>* amins;
	const valarray<double>* aerosolAngstromExponents;
};

#endif /* AER_H_ */
