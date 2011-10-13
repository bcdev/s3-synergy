/*
 * Aer.h
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
 */

#ifndef AER_H_
#define AER_H_

#include "../core/BasicModule.h"
#include "../util/Pixel.h"
#include "../util/MultivariateFunction.h"

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
    void setPixelsToSegment(map<size_t, shared_ptr<AerPixel> >& pixels);
    void initializeP(AerPixel& p);
    bool e1(AerPixel& p, int16_t amin);
    bool e2(AerPixel& q, size_t amin);
    double aotStandardError(float tau550);
};

class AerPixel : public Pixel {
public:

    AerPixel(Segment& segment, long k, long l, long m) : Pixel(segment, k, l, m), solarIrradiances(30), solarIrradianceFillValues(18) {

    }

    AerPixel(const AerPixel& p) : Pixel(p) {
        setSynFlags(p.getSynFlags());
        setTau550(p.getTau550());
        setDeltaTau550(p.getDeltaTau550());
        setAlpha550(p.getAlpha550());
        setAMIN(p.getAMIN());
        K = p.K;
        E_2 = p.E_2;
        c_1 = p.c_1;
        c_2 = p.c_2;
        copy(&p.nu[0], &p.nu[p.nu.size()-1], &nu[0]);
        copy(&p.omega[0], &p.omega[p.omega.size()-1], &omega[0]);
        copy(&p.solarIrradiances[0], &p.solarIrradiances[p.solarIrradiances.size()-1], &solarIrradiances[0]);
        copy(&p.solarIrradianceFillValues[0], &p.solarIrradianceFillValues[p.solarIrradianceFillValues.size()-1], &solarIrradianceFillValues[0]);
    }

    uint16_t getSynFlags() const {
        return getUShort("SYN_flags");
    }

    void setSynFlags(uint16_t value) {
        setUShort("SYN_flags", value);
    }

    float getAlpha550() const {
        return getFloat("A550");
    }

    void setAlpha550(float value) {
        setFloat("A550", value);
    }

    float getTau550() const {
        return getFloat("T550");
    }

    void setTau550(float value) {
        setFloat("T550", value);
    }

    float getDeltaTau550() const {
        return getFloat("T550_er");
    }

    void setDeltaTau550(float value) {
        setFloat("T550_er", value);
    }

    uint8_t getAMIN() const {
        return getUByte("AMIN");
    }

    void setAMIN(uint8_t value) {
        setUByte("AMIN", value);
    }

    virtual double getRadiance(int16_t index) {
        return getDouble("L_" + lexical_cast<string>(index));
    }

    virtual double getSDR(int16_t index) {
        return getDouble("SDR_" + lexical_cast<string>(index));
    }

    uint32_t K;
    double E_2;
    double c_1;
    double c_2;
    valarray<float> nu;
    valarray<float> omega;
    valarray<double> solarIrradiances;
    valarray<double> solarIrradianceFillValues;

};

class ErrorMetric : public MultivariateFunction {

public:

    ErrorMetric(AerPixel& p, float gamma, int16_t amin, valarray<float> totalAngularWeights, valarray<float> vegetationSpectrum,
            valarray<float> soilReflectance, valarray<int16_t> ndviIndices, matrix<float> angularWeights) :
        p(p), gamma(gamma), amin(amin), totalAngularWeights(totalAngularWeights), vegetationSpectrum(vegetationSpectrum),
        soilReflectance(soilReflectance), ndviIndices(ndviIndices), angularWeights(angularWeights) {

    }

    double value(valarray<double>& x) {
        // todo - ts 12Oct2011 - clarify what is meant by index NDV(p) (see DPM)
        for(size_t i = 1; i <= 30; i++) {
            if(p.isFillValue("L_" + lexical_cast<string>(i))) {
                spectralWeights[i-1] = 0;
                if(i >= 19 && i <= 24) {
                    angularWeights.insert_element(i - 19, 0, 0.0);
                } else if(i >= 25 && i <= 30) {
                    angularWeights.insert_element(i - 25, 1, 0.0);
                }
            }
        }
        applyAtmosphericCorrection(p, amin);

        valarray<float> rSpec(30);
        valarray<float> rAng(12);
        for(size_t i = 0; i < 30; i++) {
            rSpec[i] = specModelSurf(p.c_1, p.c_2, i);
        }
        for(size_t i = 0; i < 12; i++) {
            rAng[i] = angModelSurf(i, p);
        }
        return errorMetric(rSpec, rAng);
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

    void applyAtmosphericCorrection(AerPixel& p, int16_t amin) {

    }

    float angModelSurf(size_t index, AerPixel& p) {
        // todo - ts - 13Oct2011 - clarify how to get D
        float D = 0.0;
        size_t j = index % 6;
        size_t o = index < 6 ? 0 : 1;
        float g = (1 - gamma) * p.omega[j];
        return (1 - D) * p.nu[o] * p.omega[j] + (gamma * p.omega[j]) / (1 - g) * (D + g * (1 - D));
    }

    float errorMetric(valarray<float> rSpec, valarray<float> rAng) {
        // todo - ts - 13Oct2011 - clarify if ndvi is correctly used as array index here
        double ndvi = Aer::ndv(p, ndviIndices);
        double sum1 = 0.0;
        double sum2 = 0.0;
        double sum3 = 0.0;
        double sum4 = 0.0;
        for(size_t i = 0; i < 30; i++) {
            sum1 += spectralWeights[i] * (p.getSDR(i) - rSpec[i]) * (p.getSDR(i) - rSpec[i]);
        }
        for(size_t i = 0; i < 30; i++) {
            sum2 += spectralWeights[i];
        }
        for(size_t i = 0; i < 12; i++) {
            size_t xIndex = i < 6 ? 0 : 1;
            size_t yIndex = i % 6;
            sum3 += angularWeights.at_element(xIndex, yIndex) * (p.getSDR(i) - rAng[i]) * (p.getSDR(i) - rAng[i]);
        }
        for(size_t i = 0; i < 12; i++) {
            size_t xIndex = i < 6 ? 0 : 1;
            size_t yIndex = i % 6;
            sum4 += angularWeights.at_element(xIndex, yIndex);
        }
        return (1 - totalAngularWeights[ndvi]) * sum1 / sum2 + totalAngularWeights[ndvi] * sum3 / sum4;
    }

    float specModelSurf(double c_1, double c_2, size_t index) {
        return c_1 * vegetationSpectrum[index] + c_2 * soilReflectance[index];
    }

};

#endif /* AER_H_ */
