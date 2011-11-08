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

using std::copy;

class Aer: public BasicModule {
public:
	Aer();
	virtual ~Aer();

	void start(Context& context);
	void process(Context& context);

private:
	friend class AerTest;
	Segment* averagedSegment;
	const Grid* averagedGrid;

	double initialTau550;
	double kappa;
	valarray<int16_t> amins;
	valarray<double> initialNu;
	valarray<double> initialOmega;
	valarray<double> aerosolAngstromExponents;

	void getPixels(Context& context, valarray<Pixel>& pixels) const;
	void putPixels(const valarray<Pixel>& pixels, long firstL, long lastL) const;
	void readAuxdata(Context& context);
	void retrieveAerosolProperties(Pixel& p, ErrorMetric& em);
};

#endif /* AER_H_ */
