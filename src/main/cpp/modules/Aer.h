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

class Aer : public BasicModule {
public:
	Aer();
	virtual ~Aer();

    void start(Context& context);
    void stop(Context& context);
    void process(Context& context);

    static double ndv(Pixel& q, const valarray<int16_t>& ndviIndices);

private:
    friend class AerTest;
    Segment* averagedSegment;
    const Grid* averagedGrid;

    double initialTau550;
    double kappa;
    valarray<int16_t> amins;
    valarray<int16_t> ndviIndices;
    valarray<double> initialNu;
    valarray<double> initialOmega;
    valarray<double> aerosolAngstromExponents;

    static bool isSolarIrradianceFillValue(double f, const valarray<double> fillValues, int16_t index);
    vector<shared_ptr<Pixel> > getPixels(Context& context, long firstL, long lastL) const;
    shared_ptr<Pixel> getPixel(Context& context, long k, long l, long m) const;
    void putPixels(vector<shared_ptr<Pixel> > pixels) const;
    const vector<long> createIndices(long base, long bound) const;
    void readAuxdata(Context& context);
    void aer_s(shared_ptr<Pixel> p, Context& context);
    void applyMedianFiltering(vector<shared_ptr<Pixel> >& pixels, long firstL, long lastL);
    bool e2(Pixel& q, size_t amin, Context& context);
    double errorCurvature(shared_ptr<Pixel> p, Context& context);
};

class E1 : public UnivariateFunction {

public:
    E1(Pixel& p, int16_t amin, Context& context) : p(p), amin(amin), context(context) {
    }

    double value(double tau550) {
        p.tau550 = tau550;
        ErrorMetric em(p, amin, context);
        valarray<double> pn(10);
        pn[0] = p.c1;
        pn[1] = p.c2;
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

        MultiMin::powell(em, pn, u, MultiMin::ACCURACY_GOAL, 200);
        p.c1 = pn[0];
        p.c2 = pn[1];
        p.nu[0] = pn[2];
        p.nu[1] = pn[3];
        for(size_t i = 0; i < 6; i++) {
            p.omega[i] = pn[i + 4];
        }
        return em.value(pn);
    }

private:
    Pixel& p;
    int16_t amin;
    Context& context;

};

#endif /* AER_H_ */
