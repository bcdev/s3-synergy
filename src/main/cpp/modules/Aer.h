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

	shared_ptr<ErrorMetric> em;

	static bool isSolarIrradianceFillValue(double f, const valarray<double> fillValues, int16_t index);
	vector<shared_ptr<Pixel> > getPixels(Context& context, long firstL, long lastL) const;
	void putPixels(vector<shared_ptr<Pixel> > pixels) const;
	const vector<long> createIndices(long base, long bound) const;
	void readAuxdata(Context& context);
	void aer_s(shared_ptr<Pixel> p);
	void applyMedianFiltering(vector<shared_ptr<Pixel> >& pixels, long firstL, long lastL);
};

#endif /* AER_H_ */
