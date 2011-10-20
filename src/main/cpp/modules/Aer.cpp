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
#include "../core/TiePointInterpolator.h"
#include "../util/ErrorMetric.h"
#include "../util/LookupTableReader.h"
#include "../util/AuxdataProvider.h"

using std::min;
using std::numeric_limits;
using std::set;

Aer::Aer() : BasicModule("AER"), amins(40), ndviIndices(2), initialNu(2), initialOmega(6), aerosolAngstromExponents(40) {
}

Aer::~Aer() {
}

void Aer::start(Context& context) {
    readAuxdata(context);
    averagedSegment = &context.getSegment(Constants::SEGMENT_SYN_AVERAGED);
    SegmentDescriptor& collocatedSegmentDescriptor = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2).getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);
    averagedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("T550"));
    averagedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("T550_er"));
    averagedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("A550"));
    averagedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("AMIN"));
    for(size_t i = 1; i <= 30; i++) {
        averagedSegment->addVariable("SDR_" + lexical_cast<string>(i), Constants::TYPE_DOUBLE, 1.0, 0.0);
    }
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

    // todo - pixel rausziehen
    for(long l_prime = firstL; l_prime <= lastL; l_prime++) {
        for(long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
            for(long m_prime = averagedGrid->getFirstM(); m_prime <= averagedGrid->getMaxM(); m_prime++) {
                shared_ptr<AerPixel> p = initPixel(context, k, l_prime, m_prime);
                aer_s(p, context);
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

    // todo - create class 'pixel initialiser', which memorizes multiply used data

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
        p->solarIrradianceFillValues[channel] = lexical_cast<double>(solarIrrOlcAccessor.getFillValue());
    }
    for (size_t i = 1; i <= 6; i++) {
        const Accessor& solarIrrSlnAccessor = slnInfoSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(i));
        const size_t channel = 17 + i;
        // todo - replace l-index (see Aco module)
        p->solarIrradiances[channel] = solarIrrSlnAccessor.getDouble(slnInfoGrid.getIndex(0, 0, 1));
    }
    for (size_t i = 1; i <= 6; i++) {
        const Accessor& solarIrrSloAccessor = sloInfoSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(i));
        const size_t channel = 23 + i;
        // todo - replace l-index (see Aco module)
        p->solarIrradiances[channel] = solarIrrSloAccessor.getDouble(sloInfoGrid.getIndex(0, 0, 1));
    }

    const Segment& averagedSegment = context.getSegment(Constants::SEGMENT_SYN_AVERAGED);
    const Segment& olciTiepointSegment = context.getSegment(Constants::SEGMENT_OLC_TP);
    const Segment& slnTiepointSegment = context.getSegment(Constants::SEGMENT_SLN_TP);
    const Segment& sloTiepointSegment = context.getSegment(Constants::SEGMENT_SLO_TP);

    const Accessor& tpLatOlc = olciTiepointSegment.getAccessor("OLC_TP_lat");
    const Accessor& tpLonOlc = olciTiepointSegment.getAccessor("OLC_TP_lon");
    const Accessor& tpLatSln = slnTiepointSegment.getAccessor("SLN_TP_lat");
    const Accessor& tpLonSln = slnTiepointSegment.getAccessor("SLN_TP_lon");
    const Accessor& tpLatSlo = sloTiepointSegment.getAccessor("SLO_TP_lat");
    const Accessor& tpLonSlo = sloTiepointSegment.getAccessor("SLO_TP_lon");
    const Accessor& tpSzaOlc = olciTiepointSegment.getAccessor("SZA");
    const Accessor& tpSaaOlc = olciTiepointSegment.getAccessor("SAA");
    const Accessor& tpVzaOlc = olciTiepointSegment.getAccessor("OZA");
    const Accessor& tpVaaOlc = olciTiepointSegment.getAccessor("OAA");
    const Accessor& tpVzaSln = slnTiepointSegment.getAccessor("SLN_VZA");
    const Accessor& tpVaaSln = slnTiepointSegment.getAccessor("SLN_VAA");
    const Accessor& tpVzaSlo = sloTiepointSegment.getAccessor("SLO_VZA");
    const Accessor& tpVaaSlo = sloTiepointSegment.getAccessor("SLO_VAA");

    const valarray<double> tpLonsOlc = tpLonOlc.getDoubles();
    const valarray<double> tpLatsOlc = tpLatOlc.getDoubles();
    const valarray<double> tpLonsSln = tpLonSln.getDoubles();
    const valarray<double> tpLatsSln = tpLatSln.getDoubles();
    const valarray<double> tpLonsSlo = tpLonSlo.getDoubles();
    const valarray<double> tpLatsSlo = tpLatSlo.getDoubles();
    const valarray<double> tpSzasOlc = tpSzaOlc.getDoubles();
    const valarray<double> tpSaasOlc = tpSaaOlc.getDoubles();
    const valarray<double> tpVzasOlc = tpVzaOlc.getDoubles();
    const valarray<double> tpVaasOlc = tpVaaOlc.getDoubles();
    const valarray<double> tpVzasSln = tpVzaSln.getDoubles();
    const valarray<double> tpVzasSlo = tpVzaSlo.getDoubles();
    const valarray<double> tpVaasSln = tpVaaSln.getDoubles();
    const valarray<double> tpVaasSlo = tpVaaSlo.getDoubles();

    const valarray<double> tpOzones = olciTiepointSegment.getAccessor("ozone").getDoubles();
    const valarray<double> tpAirPressureOlc = olciTiepointSegment.getAccessor("air_pressure").getDoubles();

    const TiePointInterpolator<double> tpiOlc = TiePointInterpolator<double>(tpLonsOlc, tpLatsOlc);
    valarray<double> tpiWeights(1);
    valarray<size_t> tpiIndexes(1);
    tpiOlc.prepare(p->getLatitude(), p->getLongitude(), tpiWeights, tpiIndexes);

    p->setTau550(initialTau550);
    p->nu[0] = initialNu[0];
    p->nu[1] = initialNu[2];
    for(size_t i = 0; i < 6; i++) {
        p->omega[i] = initialOmega[i];
    }

    p->sza = tpiOlc.interpolate(tpSzasOlc, tpiWeights, tpiIndexes);
    p->saa = tpiOlc.interpolate(tpSaasOlc, tpiWeights, tpiIndexes);
    p->vzaOlc = tpiOlc.interpolate(tpVzasOlc, tpiWeights, tpiIndexes);
    p->vaaOlc = tpiOlc.interpolate(tpVaasOlc, tpiWeights, tpiIndexes);

    p->airPressure = tpiOlc.interpolate(tpAirPressureOlc, tpiWeights, tpiIndexes);
    p->ozone = tpiOlc.interpolate(tpOzones, tpiWeights, tpiIndexes);
    if(olciTiepointSegment.hasVariable("water_vapour")) {
        const valarray<double> tpWaterVapourOlc = olciTiepointSegment.getAccessor("water_vapour").getDoubles();
        p->waterVapour = tpiOlc.interpolate(tpWaterVapourOlc, tpiWeights, tpiIndexes);
    } else {
        p->waterVapour = 0.2;
    }

    const TiePointInterpolator<double> tpiSln = TiePointInterpolator<double>(tpLonsSln, tpLatsSln);
    tpiSln.prepare(p->getLatitude(), p->getLongitude(), tpiWeights, tpiIndexes);

    p->vaaSln = tpiSln.interpolate(tpVaasSln, tpiWeights, tpiIndexes);
    p->vzaSln = tpiSln.interpolate(tpVzasSln, tpiWeights, tpiIndexes);

    const TiePointInterpolator<double> tpiSlo = TiePointInterpolator<double>(tpLonsSlo, tpLatsSlo);
    tpiSlo.prepare(p->getLatitude(), p->getLongitude(), tpiWeights, tpiIndexes);

    p->vaaSlo = tpiSlo.interpolate(tpVaasSlo, tpiWeights, tpiIndexes);
    p->vzaSlo = tpiSlo.interpolate(tpVzasSlo, tpiWeights, tpiIndexes);

    AuxdataProvider& radiometricAuxdataProvider = (AuxdataProvider&)context.getObject(Constants::AUXDATA_RADIOMETRIC_ID);
    p->cO3 = radiometricAuxdataProvider.getDoubleArray("C_O3");

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

void Aer::aer_s(shared_ptr<AerPixel> p, Context& context) {
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
        const valarray<double> nu = initialNu;
        const valarray<double> omegas = initialOmega;
        double tau550 = initialTau550;
        q.setTau550(tau550);
        q.c_1 = ndv(q, ndviIndices);
        q.c_2 = 1 - q.c_1;
        q.nu[0] = nu[0];
        q.nu[1] = nu[1];
        for(size_t j = 0; j < omegas.size(); j++) {
            q.omega[j] = omegas[j];
        }
        bool success = e2(q, amin, context);
        if(success && q.E_2 < p->E_2) {
            // todo - p.assign(q) instead of 'new'
            p = shared_ptr<AerPixel>(new AerPixel(q));
            p->setAlpha550(aerosolAngstromExponents[amin]);
            p->setAMIN(amin);
        }
    }
    if(!p->isFillValue("AMIN")) {
        double tau550 = p->getTau550();
        if(tau550 > 0.0001 ) {
            double a = aotStandardError(p, context);
            if(a > 0) {
                p->setDeltaTau550(kappa * sqrt(p->E_2 / a));
                if(tau550 > 0.1 && p->getDeltaTau550() > 5 * tau550) {
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

bool Aer::e2(AerPixel& p, size_t amin, Context& context) {
    E1 e1(p, amin, context);
    Bracket bracket;
    Min::brack(e1, 0.0, 3.0, bracket);
    const bool success = Min::brent(e1, bracket, Min::ACCURACY_GOAL);
    p.setTau550(bracket.minimumX);
    p.E_2 = bracket.minimumF;
    return success;
}

double Aer::ndv(AerPixel& q, const valarray<int16_t>& ndviIndices) {
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

double Aer::aotStandardError(shared_ptr<AerPixel> p, Context& context) {
    E1 e1_1(*p, p->getAMIN(), context);
    E1 e1_2(*p, p->getAMIN(), context);
    double a = e1_1.value(0.8 * p->getTau550());
    double b = e1_2.value(0.6 * p->getTau550());
    return 25 * (p->E_2 - 2 * a + b) / (2 * p->E_2 * p->E_2);
}

void Aer::applyMedianFiltering(map<size_t, shared_ptr<AerPixel> >& pixels) {
    // todo - implement
}

void Aer::readAuxdata(Context& context) {
    addMatrixLookupTable(context, "S3__SY_2_SYRTAX.nc", "OLC_R_atm");
    addMatrixLookupTable(context, "S3__SY_2_SYRTAX.nc", "SLN_R_atm");
    addMatrixLookupTable(context, "S3__SY_2_SYRTAX.nc", "SLO_R_atm");
    addMatrixLookupTable(context, "S3__SY_2_SYRTAX.nc", "t");
    addMatrixLookupTable(context, "S3__SY_2_SYRTAX.nc", "rho_atm");
    addVectorLookupTable(context, "S3__SY_2_SYRTAX.nc", "D");
    addScalarLookupTable(context, "S3__SY_2_SYCPAX.nc", "weight_ang_tot");
    addScalarLookupTable(context, "S3__SY_2_SYRTAX.nc", "C_O3");

    shared_ptr<AuxdataProvider> configurationAuxdataProvider = shared_ptr<AuxdataProvider>(new AuxdataProvider(Constants::AUXDATA_CONFIGURATION_ID, getAuxdataPath() + "S3__SY_2_SYCPAX.nc"));
    shared_ptr<AuxdataProvider> radiometricAuxdataProvider = shared_ptr<AuxdataProvider>(new AuxdataProvider(Constants::AUXDATA_RADIOMETRIC_ID, getAuxdataPath() + "S3__SY_2_SYRTAX.nc"));
    initialTau550 = configurationAuxdataProvider->getDouble("T550_ini");
    amins = configurationAuxdataProvider->getShortArray("AMIN");
    initialNu = configurationAuxdataProvider->getDoubleArray("v_ini");
    initialOmega = configurationAuxdataProvider->getDoubleArray("w_ini");
    kappa = configurationAuxdataProvider->getDouble("kappa");
    ndviIndices = configurationAuxdataProvider->getShortArray("NDV_channel");
    aerosolAngstromExponents = radiometricAuxdataProvider->getDoubleArray("A550");

    context.addObject(configurationAuxdataProvider);
    context.addObject(radiometricAuxdataProvider);
}
