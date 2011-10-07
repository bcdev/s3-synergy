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
}

Aer::~Aer() {
}

void Aer::start(Context& context) {
    averagedSegment = &context.getSegment(Constants::SEGMENT_SYN_AVERAGED);
    averagedGrid = &averagedSegment->getGrid();
    const ProductDescriptor& synL2Descriptor = context.getDictionary()->getProductDescriptor(Constants::PRODUCT_SY2);
    const SegmentDescriptor& synCollocatedDescriptor = synL2Descriptor.getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);
    const VariableDescriptor& aminDescriptor = synCollocatedDescriptor.getVariableDescriptor("AMIN");
    aminFillValue = aminDescriptor.getFillValue<uint8_t>();
    const VariableDescriptor& tau550Descriptor = synCollocatedDescriptor.getVariableDescriptor("T550");
    tau550FillValue = tau550Descriptor.getFillValue<int16_t>();
    const VariableDescriptor& deltaTau550Descriptor = synCollocatedDescriptor.getVariableDescriptor("T550_er");
    deltaTau550FillValue = deltaTau550Descriptor.getFillValue<int16_t>();
    const VariableDescriptor& alpha550Descriptor = synCollocatedDescriptor.getVariableDescriptor("A550");
    alpha550FillValue = alpha550Descriptor.getFillValue<uint8_t>();
}

void Aer::stop(Context& context) {
    // todo cleanup
}

void Aer::process(Context& context) {
    set<shared_ptr<AerPixel> > missingPixels;
    map<size_t, shared_ptr<AerPixel> > pixels;

    long firstL = context.getFirstComputableL(*averagedSegment, *this);
    long lastL = context.getLastComputableL(*averagedSegment, *this) - 8;

    for(long l_prime = averagedGrid->getFirstL(); l_prime <= min(averagedGrid->getMaxL(), averagedGrid->getLastL()); l_prime++) {
        for(long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
            for(long m_prime = averagedGrid->getFirstM(); m_prime <= averagedGrid->getMaxM(); m_prime++) {
                shared_ptr<AerPixel> p = initPixel(k, l_prime, m_prime);
                aer_s(p);
                if(p->getAMIN() == aminFillValue) {
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
        foreach(shared_ptr<AerPixel> p, missingPixels) {
            double tau_550, deltaTau_500, alpha550 = 0;
            uint8_t AMIN = 0;
            uint32_t K = 0;
            const long k = p->k;
            const long l_prime = p->l;
            const long m_prime = p->m;
            const vector<long> iPrimeIndices = createIndices(p->l, N_b);
            const vector<long> jPrimeIndices = createIndices(p->m, N_b);
            foreach(long i_prime, iPrimeIndices) {
                foreach(long j_prime, jPrimeIndices) {
                    if (averagedGrid->isValidPosition(k, i_prime, j_prime)) {
                        if (missingPixels.find(p) == missingPixels.end()) {
                            tau_550 += p->getTau550();
                            deltaTau_500 += p->getDeltaTau550();
                            alpha550 += p->getAlpha550();
                            if (isMinimal(i_prime, l_prime) && isMinimal(j_prime, m_prime)) {
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
                missingPixels.erase(p);
            }
        }
        I++;
    }
    applyMedianFiltering(pixels);
    setPixelsToSegment(pixels);

    context.setLastComputedL(*averagedSegment, *this, lastL);
}

shared_ptr<AerPixel> Aer::initPixel(long k, long l, long m) const {
    shared_ptr<AerPixel> p = shared_ptr<AerPixel>(new AerPixel(*averagedSegment, k, l, m));
    const Accessor& synFlags = averagedSegment->getAccessor("SYN_flags");
    const size_t index = averagedGrid->getIndex(k, l, m);
    p->setSynFlags(synFlags.getUShort(index));
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
    p->setTau550(tau550FillValue);
    p->setDeltaTau550(deltaTau550FillValue);
    p->setAlpha550(alpha550FillValue);
    p->setAMIN(aminFillValue);
    p->setSynFlags(p->getSynFlags() & 3887);

    const bool isPartlyCloudy = (p->getSynFlags() & 256) == 256;
    const bool isPartlyWater = (p->getSynFlags() & 512) == 512;

    if(isPartlyCloudy || isPartlyWater) {
        return;
    }

    p->E_2 = numeric_limits<double>::infinity();

    vector<size_t> amins = getListOfAerosolModelIndexNumbers();
    foreach(size_t amin, amins) {
        AerPixel q(*p);
        initializeP(*p);
//        q.E_2 = e2(q, amin);
    }
}

void Aer::applyMedianFiltering(map<size_t, shared_ptr<AerPixel> >& pixels) {

}

bool Aer::isMinimal(long a, long b) const {
    return false;
}

void Aer::setPixelsToSegment(map<size_t, shared_ptr<AerPixel> >& pixels) {

}

vector<size_t> Aer::getListOfAerosolModelIndexNumbers() {
    vector<size_t> amins;
    return amins;
}

void Aer::initializeP(AerPixel& p) {

}
