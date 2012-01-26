/*
 * Copyright (C) 2011 by Brockmann Consult (info@brockmann-consult.de)
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
 *
 * File:   MapSegmentProvider.cpp
 * Author: thomasstorm
 *
 * Created on January 23, 2012
 */

#include <algorithm>

#include "../../../main/cpp/util/IOUtils.h"

#include "MapSegmentProvider.h"

using std::min;

MapSegmentProvider::MapSegmentProvider() :
		AbstractModule("MAP_SEGMENT_PROVIDER") {
}

MapSegmentProvider::~MapSegmentProvider() {
}

void MapSegmentProvider::start(Context& context) {
	testSegment = &context.addMapSegment("TEST", 6000, 6000);
	testSegment->addVariable("shorts", Constants::TYPE_SHORT);
}

void MapSegmentProvider::stop(Context& context) {
	reverse_foreach(const string id, context.getSegmentIds()) {
	    context.removeSegment(id);
	}
}

void MapSegmentProvider::process(Context& context) {
    const Grid& grid = testSegment->getGrid();

    for(long k = grid.getMinK(); k <= grid.getMaxK(); k++) {
        for(long l = grid.getMinL(); l <= grid.getMaxL(); l++) {
            for(long m = grid.getMinM(); m <= grid.getMaxM(); m++) {
                const size_t index = grid.getIndex(k, l, m);
                testSegment->getAccessor("testVariable").setInt(index, index % 10000);
            }
        }
    }
}
