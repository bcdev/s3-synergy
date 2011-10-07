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

using std::copy;

class AerPixel;

class Aer : public BasicModule {
public:
	Aer();
	virtual ~Aer();

    void start(Context& context);
    void stop(Context& context);
    void process(Context& context);

private:
    friend class AerTest;
    Segment* averagedSegment;
    const Grid* averagedGrid;
    uint8_t aminFillValue;
    int16_t tau550FillValue;
    int16_t deltaTau550FillValue;
    uint8_t alpha550FillValue;

    shared_ptr<AerPixel> initPixel(long k, long l, long m) const;
    const vector<long> createIndices(long base, long bound) const;
    void aer_s(shared_ptr<AerPixel> p);
    bool isMinimal(long a, long b) const;
    void applyMedianFiltering(map<size_t, shared_ptr<AerPixel> >& pixels);
    void setPixelsToSegment(map<size_t, shared_ptr<AerPixel> >& pixels);
    vector<size_t> getListOfAerosolModelIndexNumbers();
    void initializeP(AerPixel& p);
};

class AerPixel : public Pixel {
public:

    AerPixel(Segment& segment, long k, long l, long m) : Pixel(segment, k, l, m) {

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
    }

    uint16_t getSynFlags() const {
        return getUShort("SYN_flags");
    }

    void setSynFlags(uint16_t value) {
        setUShort("SYN_flags", value);
    }

    double getAlpha550() const {
        return getDouble("A550");
    }

    void setAlpha550(double value) {
        setDouble("A550", value);
    }

    double getTau550() const {
        return getDouble("T550");
    }

    void setTau550(double value) {
        setDouble("T550", value);
    }

    double getDeltaTau550() const {
        return getDouble("T550_er");
    }

    void setDeltaTau550(double value) {
        setDouble("T550_er", value);
    }

    uint8_t getAMIN() const {
        return getUByte("AMIN");
    }

    void setAMIN(uint8_t value) {
        setDouble("AMIN", value);
    }

    uint32_t K;
    double E_2;
    double c_1;
    double c_2;
    valarray<double> nu;
    valarray<double> omega;

};

#endif /* AER_H_ */
