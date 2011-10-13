/*
 * Aer.h
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
 */

#ifndef AER_H_
#define AER_H_

#include "../core/BasicModule.h"
#include "../util/AerPixel.h"
#include "../util/MultiMin.h"

using std::copy;

class AerPixel;

class Aer : public BasicModule {
public:
	Aer();
	virtual ~Aer();

    void start(Context& context);
    void stop(Context& context);
    void process(Context& context);

    static double ndv(AerPixel& q, valarray<int16_t> ndviIndices);

private:
    friend class AerTest;
    Segment* averagedSegment;
    const Grid* averagedGrid;
    float initialTau550;
    float kappa;
    float gamma;
    valarray<int16_t> amins;
    valarray<int16_t> ndviIndices;
    valarray<float> initialNu;
    valarray<float> initialOmega;
    valarray<float> aerosolAngstromExponents;
    valarray<float> spectralWeights;
    valarray<float> totalAngularWeights;
    matrix<float> angularWeights;
    valarray<float> vegetationSpectrum;
    valarray<float> soilReflectance;

    static bool isSolarIrradianceFillValue(double f, const valarray<double> fillValues, int16_t index);
    shared_ptr<AerPixel> initPixel(Context& context, long k, long l, long m) const;
    const vector<long> createIndices(long base, long bound) const;
    void readAuxdata();
    void aer_s(shared_ptr<AerPixel> p);
    void applyMedianFiltering(map<size_t, shared_ptr<AerPixel> >& pixels);
    void initializeP(AerPixel& p);
    bool e1(AerPixel& p, int16_t amin);
    bool e2(AerPixel& q, size_t amin);
    double aotStandardError(float tau550);
};

#endif /* AER_H_ */
