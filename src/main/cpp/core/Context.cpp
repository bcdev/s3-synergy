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
#include <iostream>
#include <limits>

#include "Boost.h"
#include "Context.h"
#include "NullLogging.h"
#include "DefaultModule.h"
#include "SegmentImpl.h"

using std::invalid_argument;
using std::find;
using std::min;
using std::numeric_limits;

Context::Context(Logging& logging) : logging(logging) {
    errorHandler = 0;
    dictionary = 0;
    jobOrder = 0;
}

Context::~Context() {

    reverse_foreach(Segment* segment, segmentList) {
        delete segment;
    }
}

void Context::addModule(Module& module) {
    moduleList.push_back(&module);
}

void Context::addObject(Object& object) throw (logic_error) {
    if (hasObject(object.getId())) {
        BOOST_THROW_EXCEPTION(logic_error("an object with ID '" + object.getId() + "' has already been added to the context."));
    }
    objectMap[object.getId()] = &object;
}

Segment& Context::addSegment(const string& id, size_t sizeL, size_t sizeM, size_t sizeK, size_t minL, size_t maxL) throw (logic_error) {
    if (hasSegment(id)) {
        BOOST_THROW_EXCEPTION(logic_error("a segment with ID '" + id + "' has already been added to the context."));
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

Logging& Context::getLogging() const {
    return logging;
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
            if (!hasMaxLComputed(*(segmentList[j]), *(moduleList[i]))) {
                return false;
            }
            size_t maxLComputed = getMaxLComputed(*(segmentList[j]), *(moduleList[i]));
            if (maxLComputed != segmentList[j]->getGrid().getMaxL()) {
                return false;
            }
        }
    }
    return true;
}

void Context::setErrorHandler(ErrorHandler* errorHandler) {
    this->errorHandler = errorHandler;
}

void Context::handleError(exception& e) {
    if (errorHandler == 0) {
        throw e;
    } else {
        errorHandler->handleError(*this, e);
    }
}

void Context::shift(Segment& segment) const {
    size_t maxLComputed = segment.getGrid().getStartL() + segment.getGrid().getSizeL() - 1;
    const ModuleMaxLComputedMap& map = maxLComputedMap.at(&segment);
    for (ModuleMaxLComputedMap::const_iterator iter = map.begin(); iter != map.end(); iter++) {
        maxLComputed = min(maxLComputed, iter->second);
    }
    size_t minRequiredLine = getMinLRequired(segment, maxLComputed + 1);
    segment.setStartL(minRequiredLine);
}

void Context::shift() const {

    foreach(Segment* segment, segmentList) {
        shift(*segment);
    }
}

size_t Context::getMaxLComputed(const Segment& segment, const Module& module) const {
    if (hasMaxLComputed(segment, module)) {
        return maxLComputedMap.at(&segment).at(&module);
    }
    BOOST_THROW_EXCEPTION(invalid_argument("Context::getMaxLComputed(): no maxL defined for segment '" + segment.getId() + "' and module '" + module.getId() + "'."));
}

size_t Context::getMaxLWritable(const Segment& segment, const Writer& writer) const {
    size_t maxLWritable = segment.getGrid().getStartL() + segment.getGrid().getSizeL() - 1;
    if (contains(maxLComputedMap, &segment)) {
        const ModuleMaxLComputedMap& map = maxLComputedMap.at(&segment);
        for (ModuleMaxLComputedMap::const_iterator iter = map.begin(); iter != map.end(); iter++) {
            if (iter->first != (const Module*) &writer) {
                maxLWritable = min(maxLWritable, iter->second);
            }
        }
    }
    return maxLWritable;
}

bool Context::hasMaxLComputed(const Segment& segment, const Module& module) const {
    return contains(maxLComputedMap, &segment)
            && contains(maxLComputedMap.at(&segment), &module);
}

void Context::setMaxLComputed(const Segment& segment, const Module& module, size_t l) {
    if ((l < hasMaxLComputed(segment, module) && getMaxLComputed(segment, module))
            || l > segment.getGrid().getMaxL()) {
        BOOST_THROW_EXCEPTION(invalid_argument("Context::setMaxLComputed(): invalid argument l = " + lexical_cast<string > (l)));
    }
    maxLComputedMap[&segment][&module] = l;
}

size_t Context::getMinLRequired(const Segment& segment, size_t l) const {
    size_t minLineRequired = l;
    for (size_t i = 0; i < moduleList.size(); i++) {
        minLineRequired = min(minLineRequired, moduleList[i]->getMinLRequired(segment, l));
    }
    return minLineRequired;
}
