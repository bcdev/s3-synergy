/*
 * AerPixel.h
 *
 *  Created on: 13.10.2011
 *      Author: thomasstorm
 */

#ifndef AERPIXEL_H_
#define AERPIXEL_H_

#include <algorithm>
#include <string>
#include <valarray>

#include "Pixel.h"

using std::copy;
using std::string;

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

#endif /* AERPIXEL_H_ */
