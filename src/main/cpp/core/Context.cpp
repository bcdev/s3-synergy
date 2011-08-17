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
#include "SegmentImpl.h"
#include "Writer.h"

using std::invalid_argument;
using std::find;
using std::max;
using std::min;
using std::numeric_limits;

Context::Context() {
	logging = &NullLogging::getInstance();
	errorHandler = 0;
	dictionary = 0;
	jobOrder = 0;
}

Context::~Context() {
	reverse_foreach(Segment* segment, segmentList)
			{
				delete segment;
			}
}

void Context::addModule(Module& module) {
	moduleList.push_back(&module);
}

void Context::removeModule(Module& module) {
    vector<Module*>::iterator position = find(moduleList.begin(), moduleList.end(), &module);
    if (position != moduleList.end()) {
    	moduleList.erase(position);
    }
}

void Context::addObject(Object& object) throw (logic_error) {
	if (hasObject(object.getId())) {
		BOOST_THROW_EXCEPTION(
				invalid_argument("An object with ID '" + object.getId() + "' already exists in the context."));
	}
	objectMap[object.getId()] = &object;
}

Segment& Context::addSegment(const string& id, size_t sizeL, size_t sizeM,
		size_t sizeK, size_t minL, size_t maxL) throw (logic_error) {
	if (hasSegment(id)) {
		BOOST_THROW_EXCEPTION(
				invalid_argument("A segment with ID '" + id + "' already exists in the context."));
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
	if (contains(objectMap, id)) {
		return *objectMap.at(id);
	}
	BOOST_THROW_EXCEPTION(out_of_range("no object with id '" + id + "'."));
}

Segment& Context::getSegment(const string& id) const {
	if (contains(segmentMap, id)) {
		return *segmentMap.at(id);
	}
	BOOST_THROW_EXCEPTION(out_of_range("no segment with id '" + id + "'."));
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
	typedef pair<const Segment*, map<const Module*, size_t> > P;
	typedef pair<const Module*, size_t> Q;

	foreach(P p, lastLComputedMap)
			{
				const size_t maxL = p.first->getGrid().getMaxL();

				foreach(Q q, p.second)
						{
							if (q.second < maxL) {
								return false;
							}
						}
			}
	foreach(Segment* segment, segmentList)
			{
				const Grid& grid = segment->getGrid();
				if (grid.getLastL() < grid.getMaxL()) {
					return false;
				}
			}
	return true;
}

void Context::setErrorHandler(ErrorHandler* errorHandler) {
	this->errorHandler = errorHandler;
}

void Context::handleError(exception& e) {
	if (errorHandler == 0) {
		BOOST_THROW_EXCEPTION(e);
	} else {
		errorHandler->handleError(*this, e);
	}
}

void Context::moveForward(Segment& segment) const {
	size_t lastLComputed = segment.getGrid().getLastL();

	if (contains(lastLComputedMap, segment)) {
		typedef pair<const Module*, size_t> Q;

		foreach(Q q, lastLComputedMap.at(&segment))
				{
					lastLComputed = min(lastLComputed, q.second);
				}
	}
	const size_t minLRequired = getFirstLRequired(segment, lastLComputed + 1);
	segment.moveForward(minLRequired);
}

void Context::moveSegmentsForward() const {
	foreach(Segment* segment, segmentList)
			{
				moveForward(*segment);
			}
}

size_t Context::getLastLComputed(const Segment& segment,
		const Module& module) const {
	if (hasLastLComputed(segment, module)) {
		return lastLComputedMap.at(&segment).at(&module);
	}
	BOOST_THROW_EXCEPTION(
			invalid_argument("No information on segment '" + segment.getId() + "' and module '" + module.getId() + "'."));
}

size_t Context::getLastLWritable(const Segment& segment,
		const Writer& writer) const {
	if (!contains(segmentList, segment)) {
		BOOST_THROW_EXCEPTION(
				invalid_argument("Unknown segment '" + segment.getId() + "'."));
	}
	if (!contains(moduleList, dynamic_cast<const Module&>(writer))) {
		BOOST_THROW_EXCEPTION(
				invalid_argument("Unknown module '" + writer.getId() + "'."));
	}
	size_t lastLWritable = segment.getGrid().getLastL();
	if (contains(lastLComputedMap, segment)) {
		typedef pair<const Module*, size_t> Q;

		foreach(Q q, lastLComputedMap.at(&segment))
				{
					if (q.first != (const Module*) &writer) {
						lastLWritable = min(lastLWritable, q.second);
					}
				}
	}
	return lastLWritable;
}

bool Context::hasLastLComputed(const Segment& segment,
		const Module& module) const {
	return contains(lastLComputedMap, segment)
			&& contains(lastLComputedMap.at(&segment), module);
}

void Context::setLastLComputed(const Segment& segment, const Module& module,
		size_t l) {
	if (!contains(segmentList, segment)) {
		BOOST_THROW_EXCEPTION(
				invalid_argument("Unknown segment '" + segment.getId() + "'."));
	}
	if (!contains(moduleList, module)) {
		BOOST_THROW_EXCEPTION(
				invalid_argument("Unknown module '" + module.getId() + "'."));
	}
	if ((hasLastLComputed(segment, module)
			&& l < getLastLComputed(segment, module))
			|| l > segment.getGrid().getMaxL()) {
		BOOST_THROW_EXCEPTION(
				invalid_argument("Invalid row index l = " + lexical_cast<string > (l)));
	}
	lastLComputedMap[&segment][&module] = l;
}

size_t Context::getFirstLComputable(const Segment& segment,
		const Module& module) const {
	if (hasLastLComputed(segment, module)) {
		return max(segment.getGrid().getFirstL(),
				getLastLComputed(segment, module) + 1);
	}
	return segment.getGrid().getFirstL();
}

size_t Context::getLastLComputable(const Segment& segment) const {
	if (!contains(segmentList, segment)) {
		BOOST_THROW_EXCEPTION(
				invalid_argument("Unknown segment '" + segment.getId() + "'."));
	}
	return min(segment.getGrid().getLastL(), segment.getGrid().getMaxL());
}

size_t Context::getFirstLRequired(const Segment& segment, size_t l) const {
	size_t firstLRequired = l;
	for (size_t i = 0; i < moduleList.size(); i++) {
		firstLRequired = min(firstLRequired,
				moduleList[i]->getFirstLRequired(segment, l));
	}
	return firstLRequired;
}
