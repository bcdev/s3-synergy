/*
 * IPixel.h
 *
 *  Created on: 20.10.2011
 *      Author: thomasstorm
 */

#ifndef IPIXEL_H_
#define IPIXEL_H_

#include <valarray>

using std::valarray;

class IPixel {

public:

    virtual uint16_t getSynFlags() const = 0;
    virtual double getAlpha550() const = 0;
    virtual float getTau550() const = 0;
    virtual float getDeltaTau550() const = 0;
    virtual uint8_t getAMIN() const = 0;
    virtual double getLongitude() const = 0;
    virtual double getLatitude() const = 0;
    virtual double getRadiance(int16_t index) = 0;
    virtual double getSDR(int16_t index) = 0;
    virtual virtual double getC1() const = 0;
    virtual virtual double getC2() const = 0;
    virtual virtual double getE2() const = 0;
    virtual virtual double getAirPressure() const = 0;
    virtual valarray<double> getNu() const = 0;
    virtual double getOzoneTransmission(size_t index) const = 0;
    virtual valarray<double> getOmega() const = 0;
    virtual double getOzone() const = 0;
    virtual double getSaa() const = 0;
    virtual double getSolarIrradiances(size_t index) const = 0;
    virtual double getSza() const = 0;
    virtual double getVaaOlc() const = 0;
    virtual double getVaaSln() const = 0;
    virtual double getVaaSlo() const = 0;
    virtual double getVzaOlc() const = 0;
    virtual double getVzaSln() const = 0;
    virtual double getVzaSlo() const = 0;
    virtual double getWaterVapour() const = 0;

    virtual void setSynFlags(uint16_t value) = 0;
    virtual void setAlpha550(double value) = 0;
    virtual void setTau550(double value) = 0;
    virtual void setDeltaTau550(double value) = 0;
    virtual void setAMIN(uint8_t value) = 0;
    virtual void setSDR(int16_t index, double sdr) = 0;
    virtual void setC1(double c1) = 0;
    virtual void setC2(double c2) = 0;
    virtual void setE2(double E2) = 0;
    virtual void setAirPressure(double airPressure) = 0;
    virtual void setNu(valarray<double> nu) = 0;
    virtual void setO3(valarray<double> cO3) = 0;
    virtual void setOmega(valarray<double> omega) = 0;
    virtual void setOzone(double ozone) = 0;
    virtual void setSaa(double saa) = 0;
    virtual void setSolarIrradiance(size_t index, double solarIrradiance) = 0;
    virtual void setSza(double sza) = 0;
    virtual void setVaaOlc(double vaaOlc) = 0;
    virtual void setVaaSln(double vaaSln) = 0;
    virtual void setVaaSlo(double vaaSlo) = 0;
    virtual void setVzaOlc(double vzaOlc) = 0;
    virtual void setVzaSln(double vzaSln) = 0;
    virtual void setVzaSlo(double vzaSlo) = 0;
    virtual void setWaterVapour(double waterVapour) = 0;

};

#endif /* IPIXEL_H_ */
