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
#include <stdexcept>

#include "Module.h"
#include "ProcessorContext.h"

using std::find;

ProcessorContext::ProcessorContext() : segments() {
    maxComputedLine = 0;
    minRequiredLine = 0;
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

Segment& ProcessorContext::getSegment(const string& segmentId) const {
    for (size_t i = 0; i < segments.size(); i++) {
        Segment& segment = *segments[i];
        if (segment.getId().compare(segmentId) == 0) {
            return segment;
        }
    }
}

size_t ProcessorContext::getMaxComputedLine(const Segment& segment, const Module& module) const {
    return maxComputedLine;
}

size_t ProcessorContext::getMinRequiredLine(const Segment& segment) const {
    return minRequiredLine;
}

void ProcessorContext::setMaxComputedLine(const Segment& segment, const Module& module, size_t line) {
    maxComputedLine = line;
}

void ProcessorContext::setMinRequiredLine(const Segment& segment, size_t line) {
    minRequiredLine = line;
}