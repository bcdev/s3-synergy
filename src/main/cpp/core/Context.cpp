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
#include <stdexcept>

#include "Context.h"
#include "NullLogging.h"
#include "SegmentImpl.h"
#include "Writer.h"

using std::invalid_argument;
using std::find;
using std::find_if;
using std::max;
using std::min;
using std::numeric_limits;

Context::Context() :
		logging(NullLogging::getInstance()), dictionary(), jobOrder(), errorHandler() {
}

Context::~Context() {
}

void Context::removeModule(shared_ptr<Module> module) {
	vector<shared_ptr<Module> >::iterator position = find(moduleList.begin(),
			moduleList.end(), module);
	if (position != moduleList.end()) {
		typedef pair<const Segment*, map<const Module*, size_t> > P;
		typedef pair<const Module*, size_t> Q;

		foreach(P p, lastComputedLMap)
				{
					p.second.erase(position->get());
				}
		moduleList.erase(position);
	}
}

void Context::addObject(shared_ptr<Identifiable> object) throw (logic_error) {
	if (hasObject(object->getId())) {
		BOOST_THROW_EXCEPTION(
				invalid_argument("An object with ID '" + object->getId() + "' already exists in the context."));
	}
	objectMap[object->getId()] = object;
}

Segment& Context::addSegment(const string& id, size_t sizeL, size_t sizeM,
		size_t sizeK, size_t minL, size_t maxL) throw (logic_error) {
	if (hasSegment(id)) {
		BOOST_THROW_EXCEPTION(
				invalid_argument("A segment with ID '" + id + "' already exists in the context."));
	}
	shared_ptr<Segment> segment = shared_ptr<Segment>(
			new SegmentImpl(id, sizeL, sizeM, sizeK, minL, maxL));
	segmentMap[id] = segment;
	segmentList.push_back(segment);

	return *segment;
}

void Context::removeSegment(const string& id) {
	const vector<shared_ptr<Segment> >::iterator position = find_if(
			segmentList.begin(), segmentList.end(), Id<Segment>(id));
	if (position != segmentList.end()) {
		lastComputedLMap.erase(position->get());
		segmentList.erase(position);
	}
}

shared_ptr<Identifiable> Context::getObject(const string& id) const {
	if (contains(objectMap, id)) {
		return objectMap.at(id);
	}
	BOOST_THROW_EXCEPTION(
			invalid_argument("No object with id '" + id + "' exists in the context."));
}

Segment& Context::getSegment(const string& id) const {
	if (contains(segmentMap, id)) {
		return *segmentMap.at(id);
	}
	BOOST_THROW_EXCEPTION(
			invalid_argument("No segment with id '" + id + "' exists in the context."));
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

	foreach(P p, lastComputedLMap)
			{
				const size_t maxL = p.first->getGrid().getMaxL();

				foreach(Q q, p.second)
						{
							if (q.second < maxL) {
								return false;
							}
						}
			}
	foreach(shared_ptr<Segment> segment, segmentList)
			{
				const Grid& grid = segment->getGrid();
				if (grid.getLastL() < grid.getMaxL()) {
					return false;
				}
			}
	return true;
}

void Context::setErrorHandler(shared_ptr<ErrorHandler> errorHandler) {
	this->errorHandler = errorHandler;
}

void Context::handleError(exception& e) {
	if (errorHandler == 0) {
		throw e;
	} else {
		errorHandler->handleError(*this, e);
	}
}

void Context::moveForward(shared_ptr<Segment> segment) const {
	size_t lastComputedL = segment->getGrid().getLastL();

	if (contains(lastComputedLMap, segment.get())) {
		typedef pair<const Module*, size_t> Q;

		foreach(Q q, lastComputedLMap.at(segment.get()))
				{
					lastComputedL = min(lastComputedL, q.second);
				}
	}
	const size_t minRequiredL = getFirstRequiredL(*segment, lastComputedL + 1);
	segment->moveForward(minRequiredL);
}

void Context::moveSegmentsForward() const {
	foreach(shared_ptr<Segment> segment, segmentList)
			{
				moveForward(segment);
			}
}

size_t Context::getLastComputedL(const Segment& segment,
		const Module& module) const {
	if (hasLastComputedL(segment, module)) {
		return lastComputedLMap.at(&segment).at(&module);
	}
	BOOST_THROW_EXCEPTION(
			invalid_argument("No information on segment '" + segment.getId() + "' and module '" + module.getId() + "'."));
}

size_t Context::getLastWritableL(const Segment& segment,
		const Writer& writer) const {
	if (!contains(segmentList, segment)) {
		BOOST_THROW_EXCEPTION(
				invalid_argument("Unknown segment '" + segment.getId() + "'."));
	}
	if (!contains(moduleList, dynamic_cast<const Module&>(writer))) {
		BOOST_THROW_EXCEPTION(
				invalid_argument("Unknown module '" + writer.getId() + "'."));
	}
	size_t lastWritableL = segment.getGrid().getLastL();
	if (contains(lastComputedLMap, segment)) {
		typedef pair<const Module*, size_t> Q;

		foreach(Q q, lastComputedLMap.at(&segment))
				{
					if (q.first != &writer) {
						lastWritableL = min(lastWritableL, q.second);
					}
				}
	}
	return lastWritableL;
}

bool Context::hasLastComputedL(const Segment& segment,
		const Module& module) const {
	return contains(lastComputedLMap, segment)
			&& contains(lastComputedLMap.at(&segment), module);
}

void Context::setLastComputedL(const Segment& segment, const Module& module,
		size_t l) {
	if (!contains(segmentList, segment)) {
		BOOST_THROW_EXCEPTION(
				invalid_argument("Unknown segment '" + segment.getId() + "'."));
	}
	if (!contains(moduleList, module)) {
		BOOST_THROW_EXCEPTION(
				invalid_argument("Unknown module '" + module.getId() + "'."));
	}
	if ((hasLastComputedL(segment, module)
			&& l < getLastComputedL(segment, module))
			|| l > segment.getGrid().getMaxL()) {
		BOOST_THROW_EXCEPTION(
				invalid_argument("Invalid row index l = " + lexical_cast<string > (l)));
	}
	lastComputedLMap[&segment][&module] = l;
}

size_t Context::getFirstComputableL(const Segment& segment,
		const Module& module) const {
	if (hasLastComputedL(segment, module)) {
		return max(segment.getGrid().getFirstL(),
				getLastComputedL(segment, module) + 1);
	}
	return segment.getGrid().getFirstL();
}

size_t Context::getLastComputableL(const Segment& segment) const {
	if (!contains(segmentList, segment)) {
		BOOST_THROW_EXCEPTION(
				invalid_argument("Unknown segment '" + segment.getId() + "'."));
	}
	return min(segment.getGrid().getLastL(), segment.getGrid().getMaxL());
}

size_t Context::getFirstRequiredL(const Segment& segment, size_t l) const {
	size_t firstRequiredL = l;
	for (size_t i = 0; i < moduleList.size(); i++) {
		firstRequiredL = min(firstRequiredL,
				moduleList[i]->getFirstRequiredL(segment, l));
	}
	return firstRequiredL;
}
