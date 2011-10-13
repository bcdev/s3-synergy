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
#include "../util/ErrorMetric.h"
#include "../util/LookupTableReader.h"

using std::min;
using std::numeric_limits;
using std::set;

Aer::Aer() : BasicModule("AER"), amins(40), ndviIndices(2), initialNu(2), initialOmega(6), aerosolAngstromExponents(40),
        spectralWeights(30), totalAngularWeights(4), angularWeights(2, 6), vegetationSpectrum(30), soilReflectance(30) {
}

Aer::~Aer() {
}

void Aer::start(Context& context) {
    readAuxdata();
    averagedSegment = &context.getSegment(Constants::SEGMENT_SYN_AVERAGED);
    SegmentDescriptor& averagedSegmentDescriptor = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2).getSegmentDescriptor(Constants::SEGMENT_SYN_AVERAGED);
    averagedSegment->addVariable(averagedSegmentDescriptor.getVariableDescriptor("T550"));
    averagedSegment->addVariable(averagedSegmentDescriptor.getVariableDescriptor("T550_er"));
    averagedSegment->addVariable(averagedSegmentDescriptor.getVariableDescriptor("A550"));
    averagedSegment->addVariable(averagedSegmentDescriptor.getVariableDescriptor("AMIN"));
    averagedGrid = &averagedSegment->getGrid();
}

void Aer::stop(Context& context) {
    // todo cleanup
}

void Aer::process(Context& context) {
    context.getLogging().progress("Performing aerosol retrieval...", getId());

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
    context.setLastComputedL(*averagedSegment, *this, lastL);
}

shared_ptr<AerPixel> Aer::initPixel(Context& context, long k, long l, long m) const {
    shared_ptr<AerPixel> p = shared_ptr<AerPixel>(new AerPixel(*averagedSegment, k, l, m));
    const Accessor& synFlags = averagedSegment->getAccessor("SYN_flags");
    const size_t index = averagedGrid->getIndex(k, l, m);
    p->setSynFlags(synFlags.getUShort(index));
    const Segment& olcInfoSegment = context.getSegment(Constants::SEGMENT_OLC_INFO);
    const Segment& slnInfoSegment = context.getSegment(Constants::SEGMENT_SLN_INFO);
    const Segment& sloInfoSegment = context.getSegment(Constants::SEGMENT_SLO_INFO);
    const Grid& olcInfoGrid = olcInfoSegment.getGrid();
    const Grid& slnInfoGrid = slnInfoSegment.getGrid();
    const Grid& sloInfoGrid = sloInfoSegment.getGrid();
    const Accessor& solarIrrOlcAccessor = olcInfoSegment.getAccessor("solar_irradiance");
    for(size_t channel = 0; channel < 18; channel++) {
        const size_t index = olcInfoGrid.getIndex(k, channel, m);
        p->solarIrradiances[channel] = solarIrrOlcAccessor.getFloat(index);
        p->solarIrradianceFillValues[channel] = lexical_cast<float>(solarIrrOlcAccessor.getFillValue());
    }
    for (size_t i = 1; i <= 6; i++) {
        const Accessor& solarIrrSlnAccessor = slnInfoSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(i));
        const size_t channel = 17 + i;
        p->solarIrradiances[channel] = solarIrrSlnAccessor.getDouble(slnInfoGrid.getIndex(0, 0, 1));
    }
    for (size_t i = 1; i <= 6; i++) {
        const Accessor& solarIrrSloAccessor = sloInfoSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(i));
        const size_t channel = 23 + i;
        p->solarIrradiances[channel] = solarIrrSloAccessor.getDouble(sloInfoGrid.getIndex(0, 0, 1));
    }
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

    for(size_t i = 0; i < amins.size(); i++) {
        const int16_t amin = amins[i];
        AerPixel q(*p);
        const valarray<float> nu = initialNu;
        const valarray<float> omegas = initialOmega;
        float tau550 = initialTau550;
        q.setTau550(tau550);
        q.setTau550(tau550);
        q.c_1 = ndv(q, ndviIndices);
        q.c_2 = 1 - q.c_1;
        q.nu[0] = nu[0];
        q.nu[1] = nu[1];
        for(size_t j = 0; j < omegas.size(); j++) {
            q.omega[j] = omegas[j];
        }
        bool success = e2(q, amin);
        if(success && q.E_2 < p->E_2) {
            p = shared_ptr<AerPixel>(new AerPixel(q));
            p->setAlpha550(aerosolAngstromExponents[amin]);
            p->setAMIN(amin);
        }
    }
    if(!p->isFillValue("AMIN")) {
        if(p->getTau550() > 0.0001 ) {
            double a = aotStandardError(p->getTau550());
            if(a > 0) {
                p->setDeltaTau550(kappa * sqrt(p->E_2 / a));
                if(p->getTau550() > 0.1 && p->getDeltaTau550() > 5 * p->getTau550()) {
                    p->setSynFlags(p->getSynFlags() | 32768);
                } else {
                    p->setSynFlags(p->getSynFlags() | 4096);
                }
            } else {
                p->setSynFlags(p->getSynFlags() | 8192);
            }
        } else {
            p->setSynFlags(p->getSynFlags() | 16384);
        }
    }
}

bool Aer::e2(AerPixel& p, size_t amin) {
    E2 e1(p, gamma, amin, totalAngularWeights, vegetationSpectrum, soilReflectance, ndviIndices, angularWeights);
    Bracket bracket;
    const bool success = Min::brent(e1, bracket, Min::ACCURACY_GOAL);
    return success;
}

double Aer::ndv(AerPixel& q, valarray<int16_t> ndviIndices) {
    double L1 = q.getRadiance(ndviIndices[0]);
    double L2 = q.getRadiance(ndviIndices[1]);
    double F1 = q.solarIrradiances[ndviIndices[0]];
    double F2 = q.solarIrradiances[ndviIndices[1]];
    if(q.isFillValue("L_" + lexical_cast<string>(ndviIndices[0])) ||
            q.isFillValue("L_" + lexical_cast<string>(ndviIndices[1])) ||
            isSolarIrradianceFillValue(F1, q.solarIrradianceFillValues, ndviIndices[0]) ||
            isSolarIrradianceFillValue(F2, q.solarIrradianceFillValues, ndviIndices[1])) {
        return 0.5;
    }
    return ((L2 / F2) - (L1 / F1)) / ((L2 / F2) + (L1 / F1));
}

bool Aer::isSolarIrradianceFillValue(double f, const valarray<double> fillValues, int16_t index) {
    if(index >= 18) {
        return false;
    }
    return f == fillValues[index];
}

double Aer::aotStandardError(float tau550) {
    // todo - implement
    return 0.0;
}

void Aer::applyMedianFiltering(map<size_t, shared_ptr<AerPixel> >& pixels) {
    // todo - implement
}

void Aer::readAuxdata() {
    const LookupTableReader configReader(getAuxdataPath() + "S3__SY_2_SYCPAX.nc");
    const LookupTableReader radiometricReader(getAuxdataPath() + "S3__SY_2_SYRTAX.nc");
    const size_t zeroIndex = 0;
    initialTau550 = configReader.readScalarLookupTable<float>("T550_ini")->getValue(zeroIndex);

    valarray<int16_t> aminCoordinates(40);
    for(size_t coord = 0; coord < 40; coord++){
        aminCoordinates[coord] = coord;
    }
    configReader.readVectorLookupTable<int16_t>("AMIN")->getValues(&aminCoordinates[0], amins);

    valarray<float> nuCoordinates(2);
    nuCoordinates[0] = 0;
    nuCoordinates[1] = 1;
    configReader.readVectorLookupTable<float>("v_ini")->getValues(&nuCoordinates[0], initialNu);

    valarray<float> omegaCoordinates(6);
    for(size_t coord = 0;coord < 6;coord++){
        omegaCoordinates[coord] = coord;
    }
    configReader.readVectorLookupTable<float>("w_ini")->getValues(&omegaCoordinates[0], initialOmega);

    valarray<float> a550Coordinates(40);
    for(size_t coord = 0;coord < 40;coord++){
        a550Coordinates[coord] = coord;
    }
    radiometricReader.readVectorLookupTable<float>("A550")->getValues(&a550Coordinates[0], aerosolAngstromExponents);

    kappa = configReader.readScalarLookupTable<float>("kappa")->getValue(zeroIndex);

    valarray<int16_t> ndviCoordinates(2);
    for(size_t coord = 0;coord < 6;coord++){
        ndviCoordinates[coord] = coord;
    }
    configReader.readVectorLookupTable<int16_t>("NDV_channel")->getValues(&ndviCoordinates[0], ndviIndices);

    valarray<float> synChannelCoordinates(30);
    for(size_t coord = 0;coord < 30;coord++){
        synChannelCoordinates[coord] = coord;
    }
    configReader.readVectorLookupTable<float>("weight_spec")->getValues(&synChannelCoordinates[0], spectralWeights);

    valarray<float> watCoordinates(4);
    for(size_t coord = 0;coord < 4;coord++){
        watCoordinates[coord] = coord;
    }
    configReader.readVectorLookupTable<float>("weight_ang_tot")->getValues(&watCoordinates[0], totalAngularWeights);

    shared_ptr<MatrixLookupTable<float> > weightAngLut = configReader.readMatrixLookupTable<float>("weight_ang");
    valarray<float> f(weightAngLut->getDimensionCount());
    valarray<float> w(weightAngLut->getWorkspaceSize());
    matrix<float> values(2, 6);
    valarray<float> angWeightCoords(12);
    for(size_t coord = 0;coord < 12;coord++){
        angWeightCoords[coord] = coord;
    }
    configReader.readMatrixLookupTable<float>("weight_ang")->getValues(&angWeightCoords[0], angularWeights, f, w);

    valarray<float> rVegCoordinates(30);
    for(size_t coord = 0; coord < 30; coord++){
        rVegCoordinates[coord] = coord;
    }
    configReader.readVectorLookupTable<float>("R_veg")->getValues(&rVegCoordinates[0], vegetationSpectrum);

    valarray<float> rSoilCoordinates(30);
    for(size_t coord = 0; coord < 30; coord++){
        rSoilCoordinates[coord] = coord;
    }
    configReader.readVectorLookupTable<float>("R_soil")->getValues(&rSoilCoordinates[0], soilReflectance);

    gamma = configReader.readScalarLookupTable<float>("gamma")->getValue(zeroIndex);

}
