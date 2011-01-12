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
 * File:   Context.cpp
 * Author: ralf
 * 
 * Created on December 20, 2010, 12:34 PM
 */

#include <algorithm>
#include <limits>
#include <stdexcept>

#include <iostream>

#include "Context.h"
#include "DefaultModule.h"
#include "SegmentImpl.h"

using std::invalid_argument;
using std::find;
using std::min;
using std::numeric_limits;

Context::Context() : moduleList(), objectMap(), segmentMap(), segmentList() {
    dictionary = 0;
    jobOrder = 0;
    logging = 0;
}

Context::~Context() {
    for (size_t i = segmentList.size(); i-- > 0;) {
        delete segmentList[i];
    }
}

void Context::addModule(Module& module) {
    moduleList.push_back(&module);
}

void Context::addObject(Object& object) throw (logic_error) {
    if (hasObject(object.getId())) {
        throw logic_error("an object with ID '" + object.getId() + "' has already been added to the context.");
    }
    objectMap[object.getId()] = &object;
}

Segment& Context::addSegment(const string& id, size_t sizeL, size_t sizeM, size_t sizeK, size_t minL, size_t maxL) throw (logic_error) {
    if (hasSegment(id)) {
        throw logic_error("a segment with ID '" + id + "' has already been added to the context.");
    }
    Segment* segment = new SegmentImpl(id, sizeL, sizeM, sizeK, minL, maxL);
    segmentMap[id] = segment;
    segmentList.push_back(segment);

    return *segment;
}

Dictionary* Context::getDictionary() const {
    return dictionary;
}

void Context::setDictionary(Dictionary* dictionary) {
    this->dictionary = dictionary;
}

JobOrder* Context::getJobOrder() const {
    return jobOrder;
}

void Context::setJobOrder(JobOrder* jobOrder) {
    this->jobOrder = jobOrder;
}

Logging* Context::getLogging() const {
    return logging;
}

void Context::setLogging(Logging* logging) {
    this->logging = logging;
}

const vector<Module*> Context::getModules() const {
    return moduleList;
}

Object& Context::getObject(const string& id) const {
    return *objectMap.at(id);
}

Segment& Context::getSegment(const string& id) const {
    return *segmentMap.at(id);
}

const vector<Segment*> Context::getSegments() const {
    return segmentList;
}

bool Context::hasObject(const string& id) const {
    return objectMap.find(id) != objectMap.end();
}

bool Context::hasSegment(const string& id) const {
    return segmentMap.find(id) != segmentMap.end();
}

bool Context::isCompleted() const {
    if (segmentList.empty()) {
        return false;
    }
    for (size_t i = 0; i < moduleList.size(); i++) {
        for (size_t j = 0; j < segmentList.size(); j++) {
            size_t maxLComputed = getMaxLComputed(*(segmentList[j]), *(moduleList[i]));
            if (maxLComputed != segmentList[j]->getGrid().getMaxL()) {
                return false;
            }
        }
    }
    return true;
}

size_t Context::getMaxLComputed(const Segment& segment, const Module& module) const {
    if (hasMaxLComputed(segment, module)) {
        return maxLineComputedMap.at(&segment).at(&module);
    }
    return 0;
}

size_t Context::getMaxLWritable(const Segment& segment, const Writer& writer) {
    size_t maxLWritable = segment.getGrid().getStartL() + segment.getGrid().getSizeL() - 1;
    ModuleLineMap map = maxLineComputedMap[&segment];
    for (ModuleLineMap::iterator iter = map.begin(); iter != map.end(); iter++) {
        if (&iter->first != (const Module* const*)&writer) {
            maxLWritable = min(maxLWritable, iter->second);
        }
    }
    return maxLWritable;
}

bool Context::hasMaxLComputed(const Segment& segment, const Module& module) const {
    return exists(maxLineComputedMap, &segment)
            && exists(maxLineComputedMap.at(&segment), &module);
}

void Context::setMaxLComputed(const Segment& segment, const Module& module, size_t line) {
    maxLineComputedMap[&segment][&module] = line;
}

size_t Context::getMinLRequired(const Segment& segment, size_t l) const {
    size_t minLineRequired = l;
    for (size_t i = 0; i < moduleList.size(); i++) {
        minLineRequired = min(minLineRequired, moduleList[i]->getMinLRequired(segment, l));
    }

    return minLineRequired;
}
