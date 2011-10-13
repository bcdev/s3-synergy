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
#include "../util/Min.h"
#include "../util/MultiMin.h"
#include "../util/UnivariateFunction.h"
#include "../util/ErrorMetric.h"

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
    bool e2(AerPixel& q, size_t amin);
    double aotStandardError(float tau550);
};

class E2 : public UnivariateFunction {

public:
    E2(AerPixel& p, float gamma, int16_t amin, valarray<float> totalAngularWeights, valarray<float> vegetationSpectrum,
            valarray<float> soilReflectance, valarray<int16_t> ndviIndices, matrix<float> angularWeights) :
                p(p), gamma(gamma), amin(amin), totalAngularWeights(totalAngularWeights), vegetationSpectrum(vegetationSpectrum),
                soilReflectance(soilReflectance), ndviIndices(ndviIndices), angularWeights(angularWeights) {
    }

    double value(double x) {
        ErrorMetric em(p, gamma, amin, totalAngularWeights, vegetationSpectrum, soilReflectance, ndviIndices, angularWeights);
        valarray<double> pn(10);
        pn[0] = p.c_1;
        pn[1] = p.c_2;
        pn[2] = p.nu[0];
        pn[3] = p.nu[1];
        for(size_t i = 0; i < 6; i++) {
            pn[i + 4] = p.omega[i];
        }
        valarray<valarray<double> > u(valarray<double>(10), 10);
        for(size_t i = 0; i < 10; i++) {
            valarray<double> init(10);
            for(size_t j = 0; j < 10; j++) {
                init[j] = j / 10.0;
            }
            u[i] = init;
        }

        const bool success = MultiMin::powell(em, pn, u, MultiMin::ACCURACY_GOAL, 200);
        p.c_1 = pn[0];
        p.c_2 = pn[1];
        p.nu[0] = pn[2];
        p.nu[1] = pn[3];
        for(size_t i = 0; i < 6; i++) {
            p.omega[i] = pn[i + 4];
        }
        return success;
    }

private:
    AerPixel& p;
    float gamma;
    int16_t amin;
    valarray<float> spectralWeights;
    valarray<float> totalAngularWeights;
    valarray<float> vegetationSpectrum;
    valarray<float> soilReflectance;
    valarray<int16_t> ndviIndices;
    matrix<float> angularWeights;

};

#endif /* AER_H_ */
