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
    toa4Accessor = &collocatedSegment->getAccessor("B4");

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
                const uint16_t value = getValue(index);
                uint16_t flags = vgtFlags.getUShort(index);
                vgtFlags.setUShort(index, flags | value);
            }
        }
    }
}

uint16_t Vfl::getValue(const size_t index) const {
    const double toa1 = toa1Accessor->getDouble(index);
    const double toa2 = toa2Accessor->getDouble(index);

    const double thresholdCloud1 = thresholdsCloud[0];
    const double thresholdCloud2 = thresholdsCloud[1];

    uint16_t value = 2;

    if(toa1 < thresholdCloud1) {
        value = Constants::VGT_CLEAR;
    } else if(toa1 > thresholdCloud2) {
        value = Constants::VGT_CLOUD;
    }

// todo - apply snow/ice detection
}
