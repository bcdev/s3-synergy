/*
 * Aer.h
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
 */

#ifndef AER_H_
#define AER_H_

#include "../core/BasicModule.h"

class Pixel;

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

    shared_ptr<Pixel> initPixel(long k, long l, long m) const;
    const vector<long> createIndices(long base, size_t bound) const;
    void aer_s(shared_ptr<Pixel> p);
    bool isMinimal(long a, long b) const;
    void applyMedianFiltering(map<size_t, shared_ptr<Pixel> >& pixels);
    void setPixelsToSegment(map<size_t, shared_ptr<Pixel> >& pixels);
    vector<size_t> getListOfAerosolModelIndexNumbers();
    void initializeP(Pixel& p);
};

class Pixel {

public:
    Pixel() {
    }

    Pixel(const Pixel& p) {
        synFlags = p.synFlags;
        tau_550 = p.tau_550;
        deltaTau_550 = p.deltaTau_550;
        alpha_550 = p.alpha_550;
        M_a = p.M_a;
        K = p.K;
        E_2 = p.E_2;
        c_1 = p.c_1;
        c_2 = p.c_2;
        nu_1 = p.nu_1;
        nu_2 = p.nu_2;
        // todo - turn into array
        omega_1 = p.omega_1;
        omega_2 = p.omega_2;
        omega_3 = p.omega_3;
        omega_4 = p.omega_4;
        omega_5 = p.omega_5;
        omega_6 = p.omega_6;

        k = p.k;
        l = p.l;
        m = p.m;
    }

    bool operator<(const Pixel& other) const {
        // todo - use grid
        return l < other.l;
    }

    uint16_t synFlags;
    double tau_550;
    double deltaTau_550;
    double alpha_550;
    uint8_t M_a;
    uint32_t K;
    double E_2;
    double c_1;
    double c_2;
    double nu_1;
    double nu_2;
    double omega_1;
    double omega_2;
    double omega_3;
    double omega_4;
    double omega_5;
    double omega_6;

    long k;
    long l;
    long m;
};


#endif /* AER_H_ */
