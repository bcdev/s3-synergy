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
    const AuxdataProvider& thresholdsAuxdata = getAuxdataProvider(context, Constants::AUX_ID_VPCPAX);
    thresholdsAuxdata.getVectorDouble("tcl", thresholdsCloud);
    thresholdsAuxdata.getVectorDouble("tsn", thresholdsSnowIce);
}

void Vfl::process(Context& context) {
    const Segment& collocatedSegment = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
    const Grid& collocatedGrid = collocatedSegment.getGrid();

    const Accessor& b0Accessor = collocatedSegment.getAccessor("B0");
    const Accessor& b2Accessor = collocatedSegment.getAccessor("B2");
    const Accessor& b3Accessor = collocatedSegment.getAccessor("B3");
    const Accessor& mirAccessor = collocatedSegment.getAccessor("MIR");

    Accessor& vgtFlags = collocatedSegment.getAccessor("SM");

    const long firstL = context.getFirstComputableL(collocatedSegment, *this);
    const long lastL = context.getLastComputableL(collocatedSegment, *this);

#pragma omp parallel for
    for (long l = firstL; l <= lastL; l++) {
		context.getLogging().progress("Processing line l = " + lexical_cast<string>(l), getId());
		for (long k = collocatedGrid.getFirstK(); k <= collocatedGrid.getMaxK(); k++) {
			for (long m = collocatedGrid.getFirstM(); m <= collocatedGrid.getMaxM(); m++) {
				const size_t index = collocatedGrid.getIndex(k, l, m);
				const double b0 = b0Accessor.getDouble(index);
				const double b2 = b2Accessor.getDouble(index);
				const double b3 = b3Accessor.getDouble(index);
				const double mir = mirAccessor.getDouble(index);

				uint8_t flags = vgtFlags.getUByte(index);
				flags &= Constants::VGT_CLEAR_VALUE;
				flags |= Constants::VGT_UNCERTAIN_VALUE;

				if (isSet(flags, Constants::VGT_B0_GOOD_FLAG)) {
					if (b0 < thresholdsCloud[0]) {
						flags &= Constants::VGT_CLEAR_VALUE;
					} else if (b0 > thresholdsCloud[1]) {
						flags |= Constants::VGT_CLOUD_VALUE;
					}
				}
				if (isSnowIce1(flags, b2) || isSnowIce2(flags, mir) || isSnowIce3(flags, b0, mir, b3) || isSnowIce4(flags, b0, mir) || isSnowIce5(flags, b0, b2, mir)) {
					flags |= Constants::VGT_ICE_OR_SNOW_FLAG;
				}

				vgtFlags.setUShort(index, flags);
			}
		}
	}
}
