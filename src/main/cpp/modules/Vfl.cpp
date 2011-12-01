/*
 * Vfl.cpp
 *
 *  Created on: Nov 30, 2011
 *      Author: thomasstorm
 */

#include "Vfl.h"

using std::abs;
using std::fill;

Vfl::Vfl() : BasicModule("VFL") {
}

Vfl::~Vfl() {
}

void Vfl::start(Context& context) {
    collocatedSegment = &context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
    toa1Accessor = &collocatedSegment->getAccessor("B0");
    toa2Accessor = &collocatedSegment->getAccessor("B2");
    toa3Accessor = &collocatedSegment->getAccessor("B3");
    toa4Accessor = &collocatedSegment->getAccessor("MIR");

    AuxdataProvider& thresholdsAuxdata = getAuxdataProvider(context, Constants::AUX_ID_VPCPAX);
    thresholdsCloud = thresholdsAuxdata.getVectorDouble("tcl");
    thresholdsSnowIce = thresholdsAuxdata.getVectorDouble("tsn");
}

void Vfl::process(Context& context) {
    Accessor& vgtFlags = collocatedSegment->getAccessor("SM");

    const Grid& collocatedGrid = collocatedSegment->getGrid();
    const long firstL = context.getFirstComputableL(*collocatedSegment, *this);
    const long lastL = context.getLastComputableL(*collocatedSegment, *this);
    const long firstK = collocatedGrid.getFirstK();
    const long lastK = collocatedGrid.getMaxK();
    const long firstM = collocatedGrid.getFirstM();
    const long lastM = collocatedGrid.getMaxM();

    for(long l = firstL; l <= lastL; l++) {
        context.getLogging().progress("Computing flags for line " + lexical_cast<string>(l), getId());
        for(long k = firstK; k <= lastK; k++ ) {
            for(long m = firstM; m <= lastM; m++ ) {
                const size_t index = collocatedGrid.getIndex(k, l, m);
                const uint16_t flags = vgtFlags.getUShort(index);
                const uint16_t value = getValue(index);
                vgtFlags.setUShort(index, flags | value);
            }
        }
    }
}

uint16_t Vfl::getValue(const size_t index) const {
    const double toa1 = toa1Accessor->getDouble(index);
    const double toa2 = toa2Accessor->getDouble(index);
    const double toa3 = toa3Accessor->getDouble(index);
    const double toa4 = toa4Accessor->getDouble(index);

    const double thresholdCloud1 = thresholdsCloud[0];
    const double thresholdCloud2 = thresholdsCloud[1];

    uint16_t value = 2;

    if(toa1 < thresholdCloud1) {
        value = Constants::VGT_CLEAR;
    } else if(toa1 > thresholdCloud2) {
        value = Constants::VGT_CLOUD;
    }

    const double thresholdSeaIce1 = thresholdsSnowIce[0];
    const double thresholdSeaIce2 = thresholdsSnowIce[1];
    const double thresholdSeaIce3 = thresholdsSnowIce[2];
    const double thresholdSeaIce4 = thresholdsSnowIce[3];
    const double thresholdSeaIce5 = thresholdsSnowIce[4];

    if(toa2 > thresholdSeaIce1 ||
            toa4 < thresholdSeaIce2 ||
            (toa1 - toa4) / ((toa1 + toa3) * 1000) > thresholdSeaIce3 ||
            (toa1 - toa4) / ((toa1 + toa4) * 1000) > thresholdSeaIce4 ||
            (toa1 + toa1) / 2 - toa4 > thresholdSeaIce5) { // todo - (toa1 + toa1) / 2 == toa1; is this really what is meant in the DPM?
        value |= Constants::VGT_ICE_OR_SNOW;
    }
}
