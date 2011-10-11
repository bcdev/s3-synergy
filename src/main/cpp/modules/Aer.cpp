/*
 * Aer.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
 */

#include <cmath>
#include <limits>
#include <set>

#include "Aer.h"
#include "../util/IOUtils.h"

using std::min;
using std::numeric_limits;
using std::set;

Aer::Aer() : BasicModule("AER") {
    auxdataProvider = shared_ptr<ConfigurationAuxdataProvider>(new ConfigurationAuxdataProvider(getAuxdataPath()));
}

Aer::~Aer() {
}

void Aer::start(Context& context) {
    averagedSegment = &context.getSegment(Constants::SEGMENT_SYN_AVERAGED);
    averagedGrid = &averagedSegment->getGrid();
    const ProductDescriptor& synL2Descriptor = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2);
    const SegmentDescriptor& synCollocatedDescriptor = synL2Descriptor.getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);
    const VariableDescriptor& aminDescriptor = synCollocatedDescriptor.getVariableDescriptor("AMIN");
}

void Aer::stop(Context& context) {
    // todo cleanup
}

void Aer::process(Context& context) {
    set<shared_ptr<AerPixel> > missingPixels;
    map<size_t, shared_ptr<AerPixel> > pixels;

    // todo - ts11Oct2011 - check
    long firstL = context.getFirstComputableL(*averagedSegment, *this);
    long lastL = context.getLastComputableL(*averagedSegment, *this) - 8;

    for(long l_prime = firstL; l_prime <= lastL; l_prime++) {
        for(long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
            for(long m_prime = averagedGrid->getFirstM(); m_prime <= averagedGrid->getMaxM(); m_prime++) {
                shared_ptr<AerPixel> p = initPixel(context, k, l_prime, m_prime);
                aer_s(p);
                if(p->isFillValue("AMIN")) {
                    missingPixels.insert(p);
                }
                pixels[averagedGrid->getIndex(k, l_prime, m_prime)] = p;
            }
        }
    }

    long N_b = 1;
    long I = 0;

    while (!missingPixels.empty()) {
        if (I >= 5 && I <= 12) {
            N_b++;
        }
        vector<shared_ptr<AerPixel> > completedPixels;
        foreach(shared_ptr<AerPixel> p, missingPixels) {
            double tau_550, deltaTau_500, alpha550 = 0;
            uint8_t AMIN = 0;
            uint32_t K = 0;
            const long k = p->k;
            const long l_prime = p->l;
            const long m_prime = p->m;
            const vector<long> iPrimeIndices = createIndices(p->l, N_b);
            const vector<long> jPrimeIndices = createIndices(p->m, N_b);
            long minIDiff = numeric_limits<long>::max();
            long minJDiff = numeric_limits<long>::max();
            foreach(long i_prime, iPrimeIndices) {
                foreach(long j_prime, jPrimeIndices) {
                    if (averagedGrid->isValidPosition(k, i_prime, j_prime)) {
                        if (missingPixels.find(p) == missingPixels.end()) {
                            tau_550 += p->getTau550();
                            deltaTau_500 += p->getDeltaTau550();
                            alpha550 += p->getAlpha550();

                            long iDiff = std::abs(i_prime - l_prime);
                            if(iDiff < minIDiff) {
                                minIDiff = iDiff;
                            }
                            long jDiff = std::abs(j_prime - m_prime);
                            if(jDiff < minIDiff) {
                                minIDiff = iDiff;
                            }

                            if (minIDiff == iDiff && minJDiff == jDiff) {
                                AMIN = p->getAMIN();
                            }
                            K++;
                        }
                    }
                }
            }
            if (K > 1) {
                p->setTau550(tau_550 / K);
                p->setDeltaTau550( deltaTau_500 / K);
                p->setAlpha550(alpha550 / K);
                p->setAMIN(AMIN);
                // todo - ts05Oct2011 - clarify if 'SYN_filled' (as it's called in DPM) is really meaning 'SYN_aerosol_filled'
                p->setSynFlags(p->getSynFlags() | 2048);
                completedPixels.push_back(p);
            }
        }
        foreach(shared_ptr<AerPixel> p, completedPixels) {
            missingPixels.erase(p);
        }

        I++;
    }
    applyMedianFiltering(pixels);
    setPixelsToSegment(pixels);

    context.setLastComputedL(*averagedSegment, *this, lastL);
}

shared_ptr<AerPixel> Aer::initPixel(Context& context, long k, long l, long m) const {
    shared_ptr<AerPixel> p = shared_ptr<AerPixel>(new AerPixel(*averagedSegment, k, l, m));
    const Accessor& synFlags = averagedSegment->getAccessor("SYN_flags");
    const size_t index = averagedGrid->getIndex(k, l, m);
    p->setSynFlags(synFlags.getUShort(index));
    // todo - ts 11Oct2011 - set solar irradiances, and solar irradiance fill values
    Segment& olcInfo = context.getSegment(Constants::SEGMENT_OLC_INFO);
    return p;
}

const vector<long> Aer::createIndices(long base, long bound) const {
    vector<long> result;
    for(long index = base - bound; index <= base + bound; index++) {
        if(index != 0) {
            result.push_back(index);
        }
    }
    return result;
}

void Aer::aer_s(shared_ptr<AerPixel> p) {
    p->setFillValue("T550");
    p->setFillValue("T550_er");
    p->setFillValue("A550");
    p->setFillValue("AMIN");
    // set flags SYN_success, SYN_negative_curvature, SYN_too_low, and SY_high_error to false
    p->setSynFlags(p->getSynFlags() & 3887);

    const bool isPartlyCloudy = (p->getSynFlags() & 256) == 256;
    const bool isPartlyWater = (p->getSynFlags() & 512) == 512;

    if(isPartlyCloudy || isPartlyWater) {
        return;
    }

    p->E_2 = numeric_limits<double>::infinity();

    valarray<int16_t> amins = auxdataProvider->getAMINs();
    for(size_t i = 0; i < amins.size(); i++) {
        const int16_t amin = amins[i];
        AerPixel q(*p);
        const valarray<float> nus = auxdataProvider->getInitialNus();
        const valarray<float> omegas = auxdataProvider->getInitialOmegas();
        float tau550 = auxdataProvider->getInitialTau550();
        q.setTau550(tau550);
        q.c_1 = ndv(q);
        q.c_2 = 1 - q.c_1;
    }
}

double Aer::ndv(AerPixel& q) {
    const valarray<int16_t> indices = auxdataProvider->getNdviIndices();
    double L1 = q.getRadiance(indices[0]);
    double L2 = q.getRadiance(indices[1]);
    double F1 = q.solarIrradiances[indices[0]];
    double F2 = q.solarIrradiances[indices[1]];
    if(q.isFillValue("L_" + lexical_cast<string>(indices[0])) ||
            q.isFillValue("L_" + lexical_cast<string>(indices[1])) ||
            F1 == q.solarIrradianceFillValues[indices[0]] ||
            F2 == q.solarIrradianceFillValues[indices[1]]) {
        return 0.5;
    }
    return ((L2 / F2) - (L1 / F1)) / ((L2 / F2) + (L1 / F1));
}

void Aer::applyMedianFiltering(map<size_t, shared_ptr<AerPixel> >& pixels) {

}

void Aer::setPixelsToSegment(map<size_t, shared_ptr<AerPixel> >& pixels) {
}

void Aer::initializeP(AerPixel& p) {
}
