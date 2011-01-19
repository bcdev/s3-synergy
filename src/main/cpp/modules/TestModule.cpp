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
 * File:   TestModule.cpp
 * Author: ralf
 * 
 * Created on January 19, 2011, 5:07 PM
 */

#include "../util/IOUtils.h"
#include "TestModule.h"

TestModule::TestModule() : DefaultModule("TEST") {
}

TestModule::~TestModule() {
}

void TestModule::start(Context& context) {
    Segment& segment = context.getSegment("SYN_COLLOCATION");
    SegmentDescriptor& sd = context.getDictionary()->getProductDescriptor(Constants::SYMBOLIC_NAME_SYN_L2).getSegmentDescriptor("SYN_COLLOCATION");

    VariableDescriptor& vd = sd.getVariableDescriptor("SDR_1");
    // TODO - put this information into dictionary
    vd.addDimension("K");
    vd.addDimension("L");
    vd.addDimension("M");
    vd.setType(NC_SHORT);
    // TODO - attribute types in dictionary
    vd.clearAttributes();

    IOUtils::addVariableToSegment(vd.getName(), vd.getType(), segment);
}

void TestModule::stop(Context& context) {
}

void TestModule::process(Context& context) {
    Segment& segment = context.getSegment("SYN_COLLOCATION");
    context.setMaxLComputed(segment, *this, segment.getGrid().getStartL() + segment.getGrid().getSizeL() - 1);
}
