/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
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

#if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100) > 40100
#pragma omp parallel for
#endif
    for (long l = firstL; l <= lastL; l++) {
		context.getLogging().progress("Processing line l = " + lexical_cast<string>(l), getId());
		for (long k = collocatedGrid.getMinK(); k <= collocatedGrid.getMaxK(); k++) {
			for (long m = collocatedGrid.getMinM(); m <= collocatedGrid.getMaxM(); m++) {
				const size_t index = collocatedGrid.getIndex(k, l, m);
				const double b0 = b0Accessor.getDouble(index);
				const double b2 = b2Accessor.getDouble(index);
				const double b3 = b3Accessor.getDouble(index);
				const double mir = mirAccessor.getDouble(index);

				uint8_t flags = vgtFlags.getUByte(index);
				flags &= Constants::VGT_CLEAR_VALUE;

				if (isClear(flags, b0, mir)) {
					// do nothing, cloud flags are already cleared
				} else if (isCloud(flags, b0, mir)) {
					flags |= Constants::VGT_CLOUD_VALUE;
				} else {
					flags |= Constants::VGT_UNCERTAIN_VALUE;
				}
				if (isSnowIce1(flags, b2) || isSnowIce2(flags, mir) || isSnowIce3(flags, b0, mir, b3) || isSnowIce4(flags, b0, mir) || isSnowIce5(flags, b0, b2, mir)) {
					flags |= Constants::VGT_ICE_OR_SNOW_FLAG;
				}

				vgtFlags.setUShort(index, flags);
			}
		}
	}
}
