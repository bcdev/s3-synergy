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
 * File:   VgtSegmentProvider.cpp
 * Author: thomasstorm
 *
 * Created on January 16, 2012
 */

#include <algorithm>
#include <stdio.h>
#include <string.h>
#include "../../../main/cpp/util/IOUtils.h"
#include "VgtSegmentProvider.h"

using std::min;

VgtSegmentProvider::VgtSegmentProvider() :
		AbstractModule("VGT_SEGMENT_PROVIDER") {
}

VgtSegmentProvider::~VgtSegmentProvider() {
}

void VgtSegmentProvider::start(Context& context) {
    addCommonSegments(context);
    addNonCommonSegments(context);
}

void VgtSegmentProvider::stop(Context& context) {
	reverse_foreach(const string id, context.getSegmentIds()) {
	    context.removeSegment(id);
	}
}

void VgtSegmentProvider::process(Context& context) {
    setLastComputedLines(context, Constants::PRODUCT_VGP);
    setLastComputedLines(context, Constants::PRODUCT_VGS);
}

void VgtSegmentProvider::addNonCommonSegments(Context& context) {
    vector<SegmentDescriptor*> segmentDescriptors = getNonCommonSegmentDescriptors(context.getDictionary());
    foreach(SegmentDescriptor* segDesc, segmentDescriptors) {
        vector<VariableDescriptor*> variableDescriptors = segDesc->getVariableDescriptors();
        foreach(VariableDescriptor* varDesc, variableDescriptors) {
            const string& segmentName = segDesc->getName();
            if (!context.hasSegment(segmentName)) {
                valarray<size_t> dimensionSizes = IOUtils::getDimensionSizes(varDesc);
                context.addMapSegment(segmentName, dimensionSizes[1], dimensionSizes[2]);
            }
            Segment& segment = context.getSegment(segmentName);
            if (!segment.hasVariable(varDesc->getName())) {
                segment.addVariable(*varDesc);
            }
        }
    }
}

void VgtSegmentProvider::addCommonSegments(Context& context) {
    vector<SegmentDescriptor*> segmentDescriptors = getCommonSegmentDescriptors(context.getDictionary());
    foreach(SegmentDescriptor* segDesc, segmentDescriptors) {
        vector<VariableDescriptor*> variableDescriptors = segDesc->getVariableDescriptors();
        foreach(VariableDescriptor* varDesc, variableDescriptors) {
            const string& segmentName = segDesc->getName();
            if (!context.hasSegment(segmentName)) {
                valarray<size_t> dimensionSizes = IOUtils::getDimensionSizes(varDesc);
                context.addSwathSegment(segmentName, dimensionSizes[0], dimensionSizes[1], dimensionSizes[2], 0, dimensionSizes[1] - 1);
            }
            Segment& segment = context.getSegment(segmentName);
            if (!segment.hasVariable(varDesc->getName())) {
                segment.addVariable(*varDesc);
            }
        }
    }
}

void VgtSegmentProvider::setLastComputedLines(Context& context, const string& identifier) {
    vector<SegmentDescriptor*> segmentDescriptors = context.getDictionary().getProductDescriptor(identifier).getSegmentDescriptors();
    foreach(SegmentDescriptor* segDesc, segmentDescriptors) {
        Segment& segment = context.getSegment(segDesc->getName());
        context.setLastComputedL(segment, *this, segment.getGrid().getLastL());
    }
}

const vector<SegmentDescriptor*> VgtSegmentProvider::getCommonSegmentDescriptors(const Dictionary& dict) const {
    const ProductDescriptor& productDescriptor = dict.getProductDescriptor(Constants::PRODUCT_VGP);
    vector<SegmentDescriptor*> allSegmentDescriptors = productDescriptor.getSegmentDescriptors();
    vector<SegmentDescriptor*> commonSegmentDescriptors;
    foreach(SegmentDescriptor* segmentDescriptor, allSegmentDescriptors) {
        if(isCommonDescriptor(*segmentDescriptor)) {
            commonSegmentDescriptors.push_back(segmentDescriptor);
        }
    }
    return commonSegmentDescriptors;
}

const vector<SegmentDescriptor*> VgtSegmentProvider::getNonCommonSegmentDescriptors(const Dictionary& dict) const {
    const ProductDescriptor& productDescriptor = dict.getProductDescriptor(Constants::PRODUCT_VGP);
    vector<SegmentDescriptor*> allSegmentDescriptors = productDescriptor.getSegmentDescriptors();
    vector<SegmentDescriptor*> nonCommonSegmentDescriptors;
    foreach(SegmentDescriptor* segmentDescriptor, allSegmentDescriptors) {
        if(!isCommonDescriptor(*segmentDescriptor)) {
            nonCommonSegmentDescriptors.push_back(segmentDescriptor);
        }
    }
    return nonCommonSegmentDescriptors;
}

bool VgtSegmentProvider::isCommonDescriptor(const SegmentDescriptor& segmentDescriptor) const {
    const string& segmentName = segmentDescriptor.getName();
    return segmentName.compare(Constants::SEGMENT_VGP_LAT) == 0
            || segmentName.compare(Constants::SEGMENT_VGP_LAT_TP) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LAT) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LAT_TP) == 0
            || segmentName.compare(Constants::SEGMENT_VGP_LON) == 0
            || segmentName.compare(Constants::SEGMENT_VGP_LON_TP) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LON) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LON_TP) == 0
            || segmentName.compare(Constants::SEGMENT_VGP_LAT_BNDS) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LAT_BNDS) == 0
            || segmentName.compare(Constants::SEGMENT_VGP_LAT_TP_BNDS) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LAT_TP_BNDS) == 0
            || segmentName.compare(Constants::SEGMENT_VGP_LON_BNDS) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LON_BNDS) == 0
            || segmentName.compare(Constants::SEGMENT_VGP_LON_TP_BNDS) == 0
            || segmentName.compare(Constants::SEGMENT_VGS_LON_TP_BNDS) == 0;
}
