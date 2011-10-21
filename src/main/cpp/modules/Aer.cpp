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

    // todo - ts21Oct2011 - check
    long lastL = context.getLastComputableL(*averagedSegment, *this) - 8;
    map<size_t, shared_ptr<Pixel> > missingPixels;
    vector<shared_ptr<Pixel> > pixels = getPixels(context, lastL);

    foreach(shared_ptr<Pixel> p, pixels) {
        aer_s(p, context);
        if(p->amin < 0) {
            missingPixels[p->index] = p;
        }
    }

    long N_b = 1;
    long I = 0;

    while (!missingPixels.empty()) {
        if (I >= 5 && I <= 12) {
            N_b++;
        }
        set<size_t> completedPixels;
        typedef pair<size_t, shared_ptr<Pixel> > Entry;
        foreach(Entry entry, missingPixels) {
            shared_ptr<Pixel> p = entry.second;
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

                        const size_t pixelIndex = averagedGrid->getIndex(k, i_prime, j_prime);
                        if(!contains(missingPixels, pixelIndex) || contains(completedPixels, pixelIndex)) {
                            shared_ptr<Pixel> q = pixels[pixelIndex];

                            tau_550 += q->tau550;
                            deltaTau_500 += q->tau550err;
                            alpha550 += q->alpha550;

                            long iDiff = std::abs(i_prime - l_prime);
                            if(iDiff < minIDiff) {
                                minIDiff = iDiff;
                            }
                            long jDiff = std::abs(j_prime - m_prime);
                            if(jDiff < minIDiff) {
                                minIDiff = iDiff;
                            }

                            if (minIDiff == iDiff && minJDiff == jDiff) {
                                AMIN = q->amin;
                            }
                            K++;
                        }
                    }
                }
            }
            if (K > 1) {
                p->tau550 = tau_550 / K;
                p->tau550err = deltaTau_500 / K;
                p->alpha550 = alpha550 / K;
                p->amin = AMIN;
                p->synFlags |= 2048;
                completedPixels.insert(p->index);
            }
        }
        foreach(size_t index, completedPixels) {
            missingPixels.erase(index);
        }
        I++;
    }
    // todo: putPixels(pixels);
    // todo: applyMedianFiltering(pixels);
    context.setLastComputedL(*averagedSegment, *this, lastL);
}

vector<shared_ptr<Pixel> > Aer::getPixels(Context& context, long lastL) const {
    vector<shared_ptr<Pixel> > pixels(averagedGrid->getSize());
    // todo - ts11Oct2011 - check
    long firstL = context.getFirstComputableL(*averagedSegment, *this);
    for(long l = firstL; l <= lastL; l++) {
        for(long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
            for(long m = averagedGrid->getFirstM(); m <= averagedGrid->getMaxM(); m++) {
                const size_t index = averagedGrid->getIndex(k, l, m);
                pixels[index] = getPixel(context, k, l, m);
            }
        }
    }
    return pixels;
}

shared_ptr<Pixel> Aer::getPixel(Context& context, long k, long l, long m) const {

    // todo - create class 'pixel initialiser', which memorizes multiply used data

    const size_t index = averagedGrid->getIndex(k, l, m);
    shared_ptr<Pixel> p = shared_ptr<Pixel>(new Pixel(k, l, m, index));
    const Accessor& synFlags = averagedSegment->getAccessor("SYN_flags");
    p->synFlags = synFlags.getUShort(index);
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
    tpiOlc.prepare(p->lat, p->lon, tpiWeights, tpiIndexes);

    p->tau550 = initialTau550;
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
    tpiSln.prepare(p->lat, p->lon, tpiWeights, tpiIndexes);

    p->vaaSln = tpiSln.interpolate(tpVaasSln, tpiWeights, tpiIndexes);
    p->vzaSln = tpiSln.interpolate(tpVzasSln, tpiWeights, tpiIndexes);

    const TiePointInterpolator<double> tpiSlo = TiePointInterpolator<double>(tpLonsSlo, tpLatsSlo);
    tpiSlo.prepare(p->lat, p->lon, tpiWeights, tpiIndexes);

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

void Aer::aer_s(shared_ptr<Pixel> p, Context& context) {
    double NaN = numeric_limits<double>::quiet_NaN();
    p->tau550 = NaN;
    p->tau550err = NaN;
    p->alpha550 = NaN;
    p->amin = numeric_limits<short>::min();
    // set flags SYN_success, SYN_negative_curvature, SYN_too_low, and SY_high_error to false
    p->synFlags &= 3887;

    const bool isPartlyCloudy = (p->synFlags & 256) == 256;
    const bool isPartlyWater = (p->synFlags & 512) == 512;

    if(isPartlyCloudy || isPartlyWater) {
        return;
    }

    p->E2 = (numeric_limits<double>::infinity());

    for(size_t i = 0; i < amins.size(); i++) {
        const int16_t amin = amins[i];
        Pixel q = Pixel(*p);
        const valarray<double> nu = initialNu;
        const valarray<double> omegas = initialOmega;
        double tau550 = initialTau550;
        q.tau550 = tau550;
        q.c1 = ErrorMetric::ndv(q, ndviIndices);
        q.c2 = 1 - q.c1;
        q.nu[0] = nu[0];
        q.nu[1] = nu[1];
        for(size_t j = 0; j < omegas.size(); j++) {
            q.omega[j] = omegas[j];
        }
        bool success = e2(q, amin, context);
        if(success && q.E2 < p->E2) {
            p->assign(q);
            p->alpha550 = aerosolAngstromExponents[amin];
            p->amin = amin;
        }
    }
    if(p->amin > 0) {
        double tau550 = p->tau550;
        if(tau550 > 0.0001 ) {
            double a = errorCurvature(p, context);
            if(a > 0) {
                p->tau550err = kappa * sqrt(p->E2 / a);
                if(tau550 > 0.1 && p->tau550err > 5 * tau550) {
                    p->synFlags |= 32768;
                } else {
                    p->synFlags |= 4096;
                }
            } else {
                p->synFlags |= 8192;
            }
        } else {
            p->synFlags |= 16384;
        }
    }
}

bool Aer::e2(Pixel& p, size_t amin, Context& context) {
    E1 e1(p, amin, context);
    Bracket bracket;
    Min::brack(e1, 0.0, 3.0, bracket);
    const bool success = Min::brent(e1, bracket, Min::ACCURACY_GOAL);
    p.tau550 = bracket.minimumX;
    p.E2 = bracket.minimumF;
    return success;
}

double Aer::errorCurvature(shared_ptr<Pixel> p, Context& context) {
    E1 e1_1(*p, p->amin, context);
    E1 e1_2(*p, p->amin, context);
    double a = e1_1.value(0.8 * p->tau550);
    double b = e1_2.value(0.6 * p->tau550);
    return 25 * (p->E2 - 2 * a + b) / (2 * p->E2 * p->E2);
}

void Aer::applyMedianFiltering(map<size_t, shared_ptr<Pixel> >& pixels) {
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
