/*
 * Vbm.cpp
 *
 *  Created on: Nov 17, 2011
 *      Author: thomasstorm
 */

#include "Vbm.h"

Vbm::Vbm() :
		BasicModule("PCL") {
}

Vbm::~Vbm() {
}

void Vbm::start(Context& context) {
    this->context = &context;
    AuxdataProvider& radiativeTransfer = getAuxdataProvider(context, Constants::AUX_ID_VPRTAX);
    cO3 = radiativeTransfer.getVectorDouble("C_O3");
    amin = radiativeTransfer.getShort("AMIN");
    collocatedSegment = &context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
}

void Vbm::stop(Context& context) {
}

void Vbm::process(Context& context) {
    const Accessor& latitudeAccessor = collocatedSegment->getAccessor("latitude");
    const Grid& collocatedGrid = collocatedSegment->getGrid();

    // todo - verify
    long firstL = context.getFirstComputableL(*collocatedSegment, *this);
    long lastL = context.getLastComputableL(*collocatedSegment, *this);

    for(long l = firstL; l <= lastL; l++) {
        for(long m = collocatedGrid.getFirstM(); m <= collocatedGrid.getMaxM(); m++) {
            for(long k = collocatedGrid.getFirstK(); k <= collocatedGrid.getMaxK(); k++) {
                const size_t index = collocatedGrid.getIndex(k, l, m);
                double lat = latitudeAccessor.getDouble(index);
                double t550 = computeT550(lat);

            }
        }
    }
}

void Vbm::downscale(const Pixel& p, uint16_t aerosolModelIndex, double t550, valarray<double>& surfReflNadirSyn) {
// todo - extract somewhere; don't do all of this for every pixel!
    const LookupTable<double>& lutRhoAtm = getLookupTable(*context, Constants::AUX_ID_VPRTAX, "rho_atm");
    const LookupTable<double>& lutRatm = getLookupTable(*context, Constants::AUX_ID_VPRTAX, "R_atm");
    const LookupTable<double>& lutT = getLookupTable(*context, Constants::AUX_ID_VPRTAX, "t");

    valarray<double> coordinates(6);
    coordinates[0] = t550;
    coordinates[1] = p.waterVapour;
    coordinates[2] = p.airPressure;
    coordinates[3] = p.vzaOlc;
    coordinates[4] = p.sza;
    coordinates[5] = std::abs(p.vaaOlc - p.saa);

    valarray<double> f(lutRhoAtm.getDimensionCount());
    valarray<double> w(lutRhoAtm.getMatrixWorkspaceSize());

    valarray<double> rhoAtmOlc(18);
    valarray<double> rAtmOlc(18);
    valarray<double> tSunOlc(18);
    valarray<double> tViewOlc(18);

    lutRhoAtm.getVector(&coordinates[0], rhoAtmOlc, f, w);
    lutRatm.getVector(&coordinates[0], rAtmOlc, f, w);

    coordinates[3] = p.sza;
    lutT.getVector(&coordinates[0], tSunOlc, f, w);

    coordinates[3] = p.vzaOlc;
    lutT.getVector(&coordinates[0], tViewOlc, f, w);

    for(size_t i = 0; i < 18; i++) {
        surfReflNadirSyn[i] = surfaceReflectance(
                i,
                aerosolModelIndex,
                t550,
                p.waterVapour,
                p.airPressure,
                p.vzaOlc,
                p.sza,
                p.vaaOlc,
                p.saa,
                p.solarIrradiances[i],
                p.radiances[i],
                rhoAtmOlc[i],
                rAtmOlc[i],
                tSunOlc[i],
                tViewOlc[i]);
    }

    coordinates[3] = p.vzaOlc;
    coordinates[4] = p.sza;
    coordinates[5] = std::abs(p.vaaSln - p.saa);

    valarray<double> rhoAtmSln(6);
    valarray<double> rAtmSln(6);
    valarray<double> tSunSln(6);
    valarray<double> tViewSln(6);

    lutRhoAtm.getVector(&coordinates[0], rhoAtmSln, f, w);
    lutRatm.getVector(&coordinates[0], rAtmSln, f, w);

    coordinates[3] = p.sza;
    lutT.getVector(&coordinates[0], tSunSln, f, w);

    coordinates[3] = p.vzaSln;
    lutT.getVector(&coordinates[0], tViewSln, f, w);

    for(size_t i = 18; i < 24; i++) {
        surfReflNadirSyn[i] = surfaceReflectance(
                i,
                aerosolModelIndex,
                t550,
                p.waterVapour,
                p.airPressure,
                p.vzaSln,
                p.sza,
                p.vaaSln,
                p.saa,
                p.solarIrradiances[i],
                p.radiances[i],
                rhoAtmSln[i],
                rAtmSln[i],
                tSunSln[i],
                tViewSln[i]);
    }
}

double Vbm::surfaceReflectance(size_t i, uint16_t aerosolModelIndex, double t550, double waterVapour, double airPressure, double vzaOlc,
        double sza, double vaaOlc, double saa, double solarIrradiance, double radiance, double rhoAtm, double rAtm, double tSun, double tView) {
    if(radiance == Constants::FILL_VALUE_DOUBLE) {
        return Constants::FILL_VALUE_DOUBLE;
    }
    return 0.0;
}
