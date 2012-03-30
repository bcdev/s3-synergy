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

#include <algorithm>

#include "../../../main/cpp/core/Constants.h"

#include "MapSegmentProvider.h"

using std::min;

MapSegmentProvider::MapSegmentProvider() :
		AbstractModule("MAP_SEGMENT_PROVIDER") {
}

MapSegmentProvider::~MapSegmentProvider() {
}

void MapSegmentProvider::start(Context& context) {
	context.addMapSegment("VGT", 6000, 6000).addVariable("SM", Constants::TYPE_BYTE);
}

void MapSegmentProvider::stop(Context& context) {
	reverse_foreach(const string id, context.getSegmentIds()) {
	    context.removeSegment(id);
	}
}

void MapSegmentProvider::process(Context& context) {
	const Segment& s = context.getSegment("VGT");
    const Grid& g = s.getGrid();

    Accessor& a = s.getAccessor("SM");

    for (long k = g.getMinK(); k <= g.getMaxK(); k++) {
        for (long l = g.getMinL(); l <= g.getMaxL(); l++) {
            for (long m = g.getMinM(); m <= g.getMaxM(); m++) {
                const size_t index = g.getIndex(k, l, m);
                a.setByte(index, index % 100);
            }
        }
    }
}
