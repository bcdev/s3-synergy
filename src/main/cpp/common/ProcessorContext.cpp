/*
 * Copyright (C) 2010 by Brockmann Consult (info@brockmann-consult.de)
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
 * File:   ProcessorContext.cpp
 * Author: ralf
 *
 * Created on November 22, 2010, 10:58 AM
 */

#include <algorithm>
#include <limits>
#include <stdexcept>

#include "Module.h"
#include "ProcessorContext.h"

using std::invalid_argument;
using std::find;
using std::min;
using std::numeric_limits;

ProcessorContext::ProcessorContext(JobOrder jobOrder) : segments(), jobOrder(jobOrder) {
    maxLine = 0;
}
JobOrder ProcessorContext::getJobOrder() const {
    return jobOrder;
}

ProcessorContext::~ProcessorContext() {
}

void ProcessorContext::addSegment(Segment& segment) {
    if (!containsSegment(segment)) {
        segments.push_back(&segment);
    }
}

bool ProcessorContext::containsSegment(const Segment& segment) const {
    return find(segments.begin(), segments.end(), &segment) != segments.end();
}

bool ProcessorContext::containsSegment(const string& segmentId) const {
    for (size_t i = 0; i < segments.size(); i++) {
        const Segment* segment = segments[i];
        if (segment->getId().compare(segmentId) == 0) {
            return true;
        }
    }
    return false;
}

Segment& ProcessorContext::getSegment(const string& segmentId) const {
    for (size_t i = 0; i < segments.size(); i++) {
        Segment* segment = segments[i];
        if (segment->getId().compare(segmentId) == 0) {
            return *segment;
        }
    }
    throw invalid_argument("unknown segment '" + segmentId + "'.");
}

size_t ProcessorContext::getMaxLine(const Segment& segment) const {
    return maxLine;
}

size_t ProcessorContext::getMaxLineComputed(const Segment& segment, const Module& module) const {
    if (hasMaxLineComputed(segment, module)) {
        return maxLineComputedMap.at(&segment).at(&module);
    }
    return 0;
}

bool ProcessorContext::hasMaxLineComputed(const Segment& segment, const Module& module) const {
    return exists(maxLineComputedMap, &segment)
            && exists(maxLineComputedMap.at(&segment), &module);
}

bool ProcessorContext::hasMinLineRequired(const Segment& segment) const {
    return exists(maxLineComputedMap, &segment);
}

size_t ProcessorContext::getMinLineRequired(const Segment& segment) const {
    size_t minLineRequired = numeric_limits<size_t>::max();
    const ModuleLineMap& moduleLineMap = maxLineComputedMap.at(&segment);
    for (ModuleLineMap::const_iterator i = moduleLineMap.begin(); i != moduleLineMap.end(); i++) {
        minLineRequired = min(minLineRequired, i->first->getMinLineRequired(i->second + 1));
    }
    return minLineRequired;
}

void ProcessorContext::setMaxLine(const Segment& segment, size_t line) {
    maxLine = line;
}

void ProcessorContext::setMaxLineComputed(const Segment& segment, const Module& module, size_t line) {
    maxLineComputedMap[&segment][&module] = line;
}