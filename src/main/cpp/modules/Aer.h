/*
 * Aer.h
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
 */

#ifndef AER_H_
#define AER_H_

#include "../core/Pixel.h"

#include "../modules/BasicModule.h"

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

	static double aerosolOpticalThickness(double lat, double lon);

	Segment* averagedSegment;
	const Grid* averagedGrid;

	double kappa;
	valarray<int16_t> amins;
	valarray<double> aerosolAngstromExponents;

	static const double D2R = 3.14159265358979323846 / 180.0;
};

#endif /* AER_H_ */
