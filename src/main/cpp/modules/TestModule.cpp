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

#include <algorithm>

#include "../util/Utils.h"
#include "TestModule.h"

using std::min;

TestModule::TestModule() : DefaultModule("TEST") {
}

TestModule::~TestModule() {
}

void TestModule::start(Context& context) {
    vector<SegmentDescriptor*> segmentDescriptors = context.getDictionary()->getProductDescriptor(Constants::PRODUCT_SYN_L2).getSegmentDescriptors();

    foreach(SegmentDescriptor* segDesc, segmentDescriptors) {
        vector<VariableDescriptor*> variableDescriptors = segDesc->getVariableDescriptors();

        foreach(VariableDescriptor* varDesc, variableDescriptors) {
            const string& segmentName = segDesc->getName();
            if (!context.hasSegment(segmentName)) {
                valarray<int> gridParams = getGridParams(varDesc);
                context.addSegment(segmentName, min(1000, gridParams[3]), gridParams[0], gridParams[1], gridParams[2], gridParams[3] - 1);
            } 
            Segment& segment = context.getSegment(segmentName);
            if (!segment.hasVariable(varDesc->getName())) {
                Utils::addVariableToSegment(varDesc->getName(), varDesc->getType(), segment);
            }
        }
    }
}

void TestModule::stop(Context& context) {
}

void TestModule::process(Context& context) {
    vector<SegmentDescriptor*> segmentDescriptors = context.getDictionary()->getProductDescriptor(Constants::PRODUCT_SYN_L2).getSegmentDescriptors();
    foreach(SegmentDescriptor* segDesc, segmentDescriptors) {
        Segment& segment = context.getSegment(segDesc->getName());
        context.setLastLComputed(segment, *this, segment.getGrid().getLastL());
    }
}

valarray<int> TestModule::getGridParams(VariableDescriptor* varDesc) {
    vector<Dimension*> dims = varDesc->getDimensions();
    valarray<int> result(4);
    switch(dims.size()) {
        case 3: {
            result[0] = dims[2]->getSize();     // sizeM
            result[1] = dims[0]->getSize();     // sizeK
            result[2] = 0;                      // startL
            result[3] = dims[1]->getSize();     // sizeL
            break;
        }
        case 2:
        {
            result[0] = dims[1]->getSize();
            result[1] = 1;
            result[2] = 0;
            result[3] = dims[0]->getSize();
            break;
        }
        case 1:
        {
            result[0] = dims[0]->getSize();
            result[1] = 1;
            result[2] = 0;
            result[3] = 1;
            break;
        }
    }
    return result;
}