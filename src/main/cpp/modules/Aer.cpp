/*
 * Aer.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
 */

#include <algorithm>
#include <cmath>
#include <limits>
#include <set>

#include "Aer.h"
#include "../core/TiePointInterpolator.h"
#include "../util/ErrorMetric.h"
#include "../util/LookupTableReader.h"
#include "../util/AuxdataProvider.h"

using std::copy;
using std::min;
using std::numeric_limits;
using std::set;

static const double FILL_VALUE_DOUBLE = -numeric_limits<double>::max();

class PixelInitializer {

public:
    PixelInitializer(const Context& context);
    ~PixelInitializer();
    shared_ptr<Pixel> getPixel(long k, long l, long m) const;

private:
    const Context& context;

    const Segment& averagedSegment;
    const Segment& olcInfoSegment;
    const Segment& slnInfoSegment;
    const Segment& sloInfoSegment;

    const Grid& averagedGrid;
    const Grid& olcInfoGrid;
    const Grid& slnInfoGrid;
    const Grid& sloInfoGrid;

    const Accessor& solarIrradiancesOlc;
    const Accessor& synFlags;
    const Accessor& lat;
    const Accessor& lon;

    const TiePointInterpolator<double> tiePointInterpolatorOlc;
    const TiePointInterpolator<double> tiePointInterpolatorSln;
    const TiePointInterpolator<double> tiePointInterpolatorSlo;

    vector<Accessor*> radiances;
    vector<Accessor*> solarIrradiancesSln;
    vector<Accessor*> solarIrradiancesSlo;

    valarray<double> cO3;

    valarray<double> szaTiePointsOlc;
    valarray<double> saaTiePointsOlc;
    valarray<double> vzaTiePointsOlc;
    valarray<double> vaaTiePointsOlc;
    valarray<double> vzaTiePointsSln;
    valarray<double> vaaTiePointsSln;
    valarray<double> vzaTiePointsSlo;
    valarray<double> vaaTiePointsSlo;

    valarray<double> ozoneTiePoints;
    valarray<double> airPressureTiePoints;
    valarray<double> waterVapourTiePoints;
};

PixelInitializer::PixelInitializer(const Context& context) :
        context(context),
        averagedSegment(context.getSegment(Constants::SEGMENT_SYN_AVERAGED)),
        olcInfoSegment(context.getSegment(Constants::SEGMENT_OLC_INFO)),
        slnInfoSegment(context.getSegment(Constants::SEGMENT_SLN_INFO)),
        sloInfoSegment(context.getSegment(Constants::SEGMENT_SLO_INFO)),
        averagedGrid(averagedSegment.getGrid()),
        olcInfoGrid(olcInfoSegment.getGrid()),
        slnInfoGrid(slnInfoSegment.getGrid()),
        sloInfoGrid(sloInfoSegment.getGrid()),
        solarIrradiancesOlc(olcInfoSegment.getAccessor("solar_irradiance")),
        synFlags(averagedSegment.getAccessor("SYN_flags")),
        lat(averagedSegment.getAccessor("latitude")),
        lon(averagedSegment.getAccessor("longitude")),
        tiePointInterpolatorOlc(context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("OLC_TP_lon").getDoubles(), context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("OLC_TP_lat").getDoubles()),
        tiePointInterpolatorSln(context.getSegment(Constants::SEGMENT_SLN_TP).getAccessor("SLN_TP_lon").getDoubles(), context.getSegment(Constants::SEGMENT_SLN_TP).getAccessor("SLN_TP_lat").getDoubles()),
        tiePointInterpolatorSlo(context.getSegment(Constants::SEGMENT_SLO_TP).getAccessor("SLO_TP_lon").getDoubles(), context.getSegment(Constants::SEGMENT_SLO_TP).getAccessor("SLO_TP_lat").getDoubles()) {
    for (size_t b = 0; b < 30; b++) {
        radiances.push_back(&averagedSegment.getAccessor("L_" + lexical_cast<string>(b + 1)));
    }
    for (size_t b = 0; b < 6; b++) {
        solarIrradiancesSln.push_back(&slnInfoSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(b + 1)));
    }
    for (size_t b = 0; b < 6; b++) {
        solarIrradiancesSlo.push_back(&sloInfoSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(b + 1)));
    }

    AuxdataProvider& radiometricAuxdataProvider = (AuxdataProvider&) context.getObject(Constants::AUXDATA_RADIOMETRIC_ID);
    cO3 = radiometricAuxdataProvider.getDoubleArray("C_O3");

    const Segment& olciTiepointSegment = context.getSegment(Constants::SEGMENT_OLC_TP);
    const Segment& slnTiepointSegment = context.getSegment(Constants::SEGMENT_SLN_TP);
    const Segment& sloTiepointSegment = context.getSegment(Constants::SEGMENT_SLO_TP);

    const Accessor& latOlc = olciTiepointSegment.getAccessor("OLC_TP_lat");
    const Accessor& lonOlc = olciTiepointSegment.getAccessor("OLC_TP_lon");
    const Accessor& latSln = slnTiepointSegment.getAccessor("SLN_TP_lat");
    const Accessor& lonSln = slnTiepointSegment.getAccessor("SLN_TP_lon");
    const Accessor& latSlo = sloTiepointSegment.getAccessor("SLO_TP_lat");
    const Accessor& lonSlo = sloTiepointSegment.getAccessor("SLO_TP_lon");

    const valarray<double> lonTiePointsOlc = lonOlc.getDoubles();
    const valarray<double> latTiePointsOlc = latOlc.getDoubles();
    const valarray<double> lonTiePointsSln = lonSln.getDoubles();
    const valarray<double> latTiePointsSln = latSln.getDoubles();
    const valarray<double> lonTiePointsSlo = lonSlo.getDoubles();
    const valarray<double> latTiePointsSlo = latSlo.getDoubles();

    const Accessor& szaOlc = olciTiepointSegment.getAccessor("SZA");
    const Accessor& saaOlc = olciTiepointSegment.getAccessor("SAA");
    const Accessor& vzaOlc = olciTiepointSegment.getAccessor("OLC_VZA");
    const Accessor& vaaOlc = olciTiepointSegment.getAccessor("OLC_VAA");
    const Accessor& vzaSln = slnTiepointSegment.getAccessor("SLN_VZA");
    const Accessor& vaaSln = slnTiepointSegment.getAccessor("SLN_VAA");
    const Accessor& vzaSlo = sloTiepointSegment.getAccessor("SLO_VZA");
    const Accessor& vaaSlo = sloTiepointSegment.getAccessor("SLO_VAA");

    szaTiePointsOlc = szaOlc.getDoubles();
    saaTiePointsOlc = saaOlc.getDoubles();
    vzaTiePointsOlc = vzaOlc.getDoubles();
    vaaTiePointsOlc = vaaOlc.getDoubles();
    vzaTiePointsSln = vzaSln.getDoubles();
    vzaTiePointsSlo = vzaSlo.getDoubles();
    vaaTiePointsSln = vaaSln.getDoubles();
    vaaTiePointsSlo = vaaSlo.getDoubles();

    ozoneTiePoints = olciTiepointSegment.getAccessor("ozone").getDoubles();
    airPressureTiePoints = olciTiepointSegment.getAccessor("air_pressure").getDoubles();

    if (olciTiepointSegment.hasVariable("water_vapour")) {
        waterVapourTiePoints = olciTiepointSegment.getAccessor("water_vapour").getDoubles();
    }
}

PixelInitializer::~PixelInitializer() {
}

shared_ptr<Pixel> PixelInitializer::getPixel(long k, long l, long m) const {
    const size_t pixelIndex = averagedGrid.getIndex(k, l, m);
    shared_ptr<Pixel> p = shared_ptr<Pixel>(new Pixel(k, l, m, pixelIndex));

    /*
     * Radiances
     */
    for (size_t b = 0; b < 30; b++) {
        Accessor& radiance = *radiances[b];
        if (!radiance.isFillValue(pixelIndex)) {
            p->radiances[b] = radiance.getDouble(pixelIndex);
        } else {
            p->radiances[b] = FILL_VALUE_DOUBLE;
        }
    }

    /*
     * SDRs
     */
    // not needed

    /*
     * Solar irradiances
     */
    for (size_t channel = 0; channel < 18; channel++) {
        const size_t index = olcInfoGrid.getIndex(k, channel, m);
        if (!solarIrradiancesOlc.isFillValue(index)) {
            p->solarIrradiances[channel] = solarIrradiancesOlc.getDouble(index);
        } else {
            p->solarIrradiances[channel] = FILL_VALUE_DOUBLE;
        }
    }
    for (size_t i = 0; i < 6; i++) {
        const Accessor& accessor = *solarIrradiancesSln[i];
        const size_t channel = 18 + i;
        // TODO: replace m-index with SLN line % 4
        p->solarIrradiances[channel] = accessor.getDouble(slnInfoGrid.getIndex(0, 0, 1));
    }
    for (size_t i = 0; i < 6; i++) {
        const Accessor& accessor = *solarIrradiancesSlo[i];
        const size_t channel = 24 + i;
        // TODO: replace m-index with SLN line % 4
        p->solarIrradiances[channel] = accessor.getDouble(sloInfoGrid.getIndex(0, 0, 1));
    }

    /*
     * Ozone coefficients
     */
    copy(&cO3[0], &cO3[30], &(p->cO3[0]));

    /*
     * Flags
     */
    p->synFlags = synFlags.getUShort(pixelIndex);
    // set flags SYN_success, SYN_negative_curvature, SYN_too_low, and SY_high_error to false
    p->synFlags &= 3887;

    /*
     * Geo-location
     */
    p->lat = lat.getDouble(pixelIndex);
    p->lon = lon.getDouble(pixelIndex);

    /*
     * Tie Point data
     */
    valarray<double> tpiWeights(1);
    valarray<size_t> tpiIndexes(1);
    tiePointInterpolatorOlc.prepare(p->lat, p->lon, tpiWeights, tpiIndexes);

    p->sza = tiePointInterpolatorOlc.interpolate(szaTiePointsOlc, tpiWeights, tpiIndexes);
    p->saa = tiePointInterpolatorOlc.interpolate(saaTiePointsOlc, tpiWeights, tpiIndexes);
    p->vzaOlc = tiePointInterpolatorOlc.interpolate(vzaTiePointsOlc, tpiWeights, tpiIndexes);
    p->vaaOlc = tiePointInterpolatorOlc.interpolate(vaaTiePointsOlc, tpiWeights, tpiIndexes);

    p->airPressure = tiePointInterpolatorOlc.interpolate(airPressureTiePoints, tpiWeights, tpiIndexes);
    p->ozone = tiePointInterpolatorOlc.interpolate(ozoneTiePoints, tpiWeights, tpiIndexes);
    if (waterVapourTiePoints.size() != 0) {
        p->waterVapour = tiePointInterpolatorOlc.interpolate(waterVapourTiePoints, tpiWeights, tpiIndexes);
    } else {
        p->waterVapour = 0.2;
    }

    tiePointInterpolatorSln.prepare(p->lat, p->lon, tpiWeights, tpiIndexes);
    p->vaaSln = tiePointInterpolatorSln.interpolate(vaaTiePointsSln, tpiWeights, tpiIndexes);
    p->vzaSln = tiePointInterpolatorSln.interpolate(vzaTiePointsSln, tpiWeights, tpiIndexes);

    tiePointInterpolatorSlo.prepare(p->lat, p->lon, tpiWeights, tpiIndexes);
    p->vaaSlo = tiePointInterpolatorSlo.interpolate(vaaTiePointsSlo, tpiWeights, tpiIndexes);
    p->vzaSlo = tiePointInterpolatorSlo.interpolate(vzaTiePointsSlo, tpiWeights, tpiIndexes);


    /*
     * Anything else
     */
    p->tau550 = FILL_VALUE_DOUBLE;
    p->tau550err = FILL_VALUE_DOUBLE;
    p->tau550_filtered = FILL_VALUE_DOUBLE;
    p->tau550err_filtered = FILL_VALUE_DOUBLE;
    p->alpha550 = FILL_VALUE_DOUBLE;
    p->amin = numeric_limits<short>::min();
    p->E2 = numeric_limits<double>::max();

    return p;
}


Aer::Aer() :
        BasicModule("AER"), amins(40), ndviIndices(2), initialNu(2), initialOmega(6), aerosolAngstromExponents(40) {
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
    averagedGrid = &averagedSegment->getGrid();
}

void Aer::process(Context& context) {
    context.getLogging().progress("Performing aerosol retrieval...", getId());

    long firstL = context.getFirstComputableL(*averagedSegment, *this);
    long lastL = context.getLastComputableL(*averagedSegment, *this);
    if (lastL < averagedGrid->getMaxL()) {
        lastL -= 10;
    }

    map<size_t, shared_ptr<Pixel> > missingPixels;
    vector<shared_ptr<Pixel> > pixels = getPixels(context, firstL, lastL < averagedGrid->getMaxL() ? lastL + 1: lastL);

    foreach(shared_ptr<Pixel> p, pixels) {
        context.getLogging().debug("...for pixel with index " + lexical_cast<string>(p->index), getId());
        aer_s(p, context);
        if (p->amin == 0) {
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
            uint8_t amin = 0;
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
                        if (!contains(missingPixels, pixelIndex) || contains(completedPixels, pixelIndex)) {
                            shared_ptr<Pixel> q = pixels[pixelIndex];

                            tau_550 += q->tau550;
                            deltaTau_500 += q->tau550err;
                            alpha550 += q->alpha550;

                            long iDiff = std::abs(i_prime - l_prime);
                            if (iDiff < minIDiff) {
                                minIDiff = iDiff;
                            }
                            long jDiff = std::abs(j_prime - m_prime);
                            if (jDiff < minIDiff) {
                                minIDiff = iDiff;
                            }

                            if (minIDiff == iDiff && minJDiff == jDiff) {
                                amin = q->amin;
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
                p->amin = amin;
                p->synFlags |= 2048;
                completedPixels.insert(p->index);
            }
        }
        foreach(size_t index, completedPixels) {
            missingPixels.erase(index);
        }
        I++;
    }
    applyMedianFiltering(pixels, firstL, lastL);
    putPixels(pixels);
    context.setLastComputedL(*averagedSegment, *this, lastL);
    context.setFirstRequiredL(*averagedSegment, *this, lastL + 1);
}

vector<shared_ptr<Pixel> > Aer::getPixels(Context& context, long firstL, long lastL) const {
    const PixelInitializer pixelInitializer(context);
    vector<shared_ptr<Pixel> > pixels(averagedGrid->getSize());
    for (long l = firstL; l <= lastL; l++) {
        for (long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
            for (long m = averagedGrid->getFirstM(); m <= averagedGrid->getMaxM(); m++) {
                const size_t index = averagedGrid->getIndex(k, l, m);
                pixels[index] = pixelInitializer.getPixel(k, l, m);
            }
        }
    }
    return pixels;
}

const vector<long> Aer::createIndices(long base, long bound) const {
    vector<long> result;
    for (long index = base - bound; index <= base + bound; index++) {
        if (index != base) {
            result.push_back(index);
        }
    }
    return result;
}

void Aer::aer_s(shared_ptr<Pixel> p, Context& context) {
    const bool isPartlyCloudy = (p->synFlags & 256) == 256;
    const bool isPartlyWater = (p->synFlags & 512) == 512;

    if (isPartlyCloudy || isPartlyWater) {
        return;
    }

    for (size_t i = 0; i < amins.size(); i++) {
        const int16_t amin = amins[i];
        Pixel q = Pixel(*p);
        q.nu[0] = initialNu[0];
        q.nu[1] = initialNu[1];
        for (size_t j = 0; j < initialOmega.size(); j++) {
            q.omega[j] = initialOmega[j];
        }
        q.tau550 = initialTau550;
        q.c1 = ErrorMetric::ndv(q, ndviIndices);
        q.c2 = 1 - q.c1;
        bool success = e2(q, amin, context);
        if (success && q.E2 < p->E2) {
            p->assign(q);
            p->alpha550 = aerosolAngstromExponents[amin];
            p->amin = amin;
        }
    }
    if (p->amin > 0) {
        double tau550 = p->tau550;
        if (tau550 > 0.0001) {
            double a = errorCurvature(p, context);
            if (a > 0) {
                p->tau550err = kappa * sqrt(p->E2 / a);
                if (tau550 > 0.1 && p->tau550err > 5 * tau550) {
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

void Aer::applyMedianFiltering(vector<shared_ptr<Pixel> >& pixels, long firstL, long lastL) {
    valarray<double> tau550Values(9);
    valarray<double> tau550ErrValues(9);
    for (long l = firstL; l <= lastL; l++) {
        for (long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
            for (long m = averagedGrid->getFirstM(); m <= averagedGrid->getMaxM(); m++) {
                const size_t index = averagedGrid->getIndex(k, l, m);
                shared_ptr<Pixel> p = pixels[index];
                size_t i = 0;
                for (long l = p->l - 1; l <= p->l + 1; l++) {
                    for (long m = p->m - 1; m <= p->m + 1; m++) {
                        if (averagedGrid->isValidPosition(p->k, l, m)) {
                            const size_t index = averagedGrid->getIndex(p->k, l, m);
                            tau550Values[i] = pixels.at(index)->tau550;
                            tau550ErrValues[i] = pixels.at(index)->tau550err;
                            i++;
                        }
                    }
                }
                std::nth_element(&tau550Values[0], &tau550Values[i / 2], &tau550Values[i + 1]);
                std::nth_element(&tau550ErrValues[0], &tau550ErrValues[i / 2], &tau550ErrValues[i + 1]);
                p->tau550_filtered = tau550Values[i / 2];
                p->tau550err_filtered = tau550ErrValues[i / 2];
            }
        }
    }
}

void Aer::readAuxdata(Context& context) {
    getMatrixLookupTable(context, "S3__SY_2_SYRTAX.nc", "OLC_R_atm");
    getMatrixLookupTable(context, "S3__SY_2_SYRTAX.nc", "SLN_R_atm");
    getMatrixLookupTable(context, "S3__SY_2_SYRTAX.nc", "SLO_R_atm");
    getMatrixLookupTable(context, "S3__SY_2_SYRTAX.nc", "t");
    getMatrixLookupTable(context, "S3__SY_2_SYRTAX.nc", "rho_atm");
    getVectorLookupTable(context, "S3__SY_2_SYRTAX.nc", "D");
    getScalarLookupTable(context, "S3__SY_2_SYCPAX.nc", "weight_ang_tot");
    getScalarLookupTable(context, "S3__SY_2_SYRTAX.nc", "C_O3");

    AuxdataProvider& configurationAuxdataProvider = getAuxdataProvider(context, Constants::AUXDATA_CONFIGURATION_ID);
    AuxdataProvider& radiometricAuxdataProvider = getAuxdataProvider(context, Constants::AUXDATA_RADIOMETRIC_ID);

    initialTau550 = configurationAuxdataProvider.getDouble("T550_ini");
    amins = configurationAuxdataProvider.getShortArray("AMIN");
    initialNu = configurationAuxdataProvider.getDoubleArray("v_ini");
    initialOmega = configurationAuxdataProvider.getDoubleArray("w_ini");
    kappa = configurationAuxdataProvider.getDouble("kappa");
    ndviIndices = configurationAuxdataProvider.getShortArray("NDV_channel");
    aerosolAngstromExponents = radiometricAuxdataProvider.getDoubleArray("A550");
}

void Aer::putPixels(vector<shared_ptr<Pixel> > pixels) const {
    Accessor& amin = averagedSegment->getAccessor("AMIN");
    Accessor& t550 = averagedSegment->getAccessor("T550");
    Accessor& t550err = averagedSegment->getAccessor("T550_err");
    Accessor& a550 = averagedSegment->getAccessor("A550");
    Accessor& synFlags = averagedSegment->getAccessor("SYN_flags");

    foreach(shared_ptr<Pixel> p, pixels) {
        amin.setUByte(p->index, p->amin);
        t550.setDouble(p->index, p->tau550_filtered);
        t550err.setDouble(p->index, p->tau550err_filtered);
        a550.setDouble(p->index, p->alpha550);
        synFlags.setUShort(p->index, p->synFlags);
    }
}

