/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
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
 */

#include <algorithm>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "Context.h"
#include "MapSegment.h"
#include "Module.h"
#include "NullLogging.h"
#include "SwathSegment.h"

using std::find;
using std::find_if;
using std::max;
using std::min;
using std::numeric_limits;

using boost::lexical_cast;

Context::Context() :
		logging(shared_ptr<Logging>(new NullLogging())), dictionary(), jobOrder(), errorHandler() {
}

Context::~Context() {
}

void Context::addObject(shared_ptr<Identifiable> object) throw (logic_error) {
	if (hasObject(object->getId())) {
		BOOST_THROW_EXCEPTION( invalid_argument("An object with ID '" + object->getId() + "' already exists in the context."));
	}
	objectMap[object->getId()] = object;
}

Segment& Context::addMapSegment(const string& id, long sizeL, long sizeM, long sizeK) throw (logic_error) {
	if (hasSegment(id)) {
		BOOST_THROW_EXCEPTION( invalid_argument("A segment with ID '" + id + "' already exists in the context."));
	}
	shared_ptr<Segment> segment = shared_ptr<Segment>(new MapSegment(id, sizeL, sizeM, sizeK));
	segmentMap[id] = segment;
	segmentList.push_back(segment);

	return *segment;
}

Segment& Context::addSingleLineSegment(const string& id, long size) throw (logic_error) {
	if (hasSegment(id)) {
		BOOST_THROW_EXCEPTION( invalid_argument("A segment with ID '" + id + "' already exists in the context."));
	}
	// TODO - use a concrete SingleLineSegment implementation
	shared_ptr<Segment> segment = shared_ptr<Segment>(new SwathSegment(id, 1, size, 1, 0, 0));
	segmentMap[id] = segment;
	segmentList.push_back(segment);

	return *segment;
}

Segment& Context::addSwathSegment(const string& id, long sizeL, long sizeM, long sizeK, long minL, long maxL) throw (logic_error) {
	if (hasSegment(id)) {
		BOOST_THROW_EXCEPTION( invalid_argument("A segment with ID '" + id + "' already exists in the context."));
	}
	shared_ptr<Segment> segment = shared_ptr<Segment>(new SwathSegment(id, sizeL, sizeM, sizeK, minL, maxL));
	segmentMap[id] = segment;
	segmentList.push_back(segment);

	return *segment;
}

bool Context::removeSegment(const string& id) {
	const vector<shared_ptr<Segment> >::iterator position = find_if(segmentList.begin(), segmentList.end(), Id<Segment>(id));
	if (position != segmentList.end()) {
		lastComputedLMap.erase(position->get());
		firstRequiredLMap.erase(position->get());
		segmentList.erase(position);
		segmentMap.erase(id);
		return true;
	}
	return false;
}

Identifiable& Context::getObject(const string& id) const throw (invalid_argument) {
	if (contains(objectMap, id)) {
		return *objectMap.at(id);
	}
	BOOST_THROW_EXCEPTION( invalid_argument("Object with id '" + id + "' does not exist in the context."));
}

Segment& Context::getSegment(const string& id) const throw (invalid_argument) {
	if (contains(segmentMap, id)) {
		return *segmentMap.at(id);
	}
	BOOST_THROW_EXCEPTION( invalid_argument("Segment with id '" + id + "' does not exist in the context."));
}

bool Context::hasObject(const string& id) const {
	return objectMap.find(id) != objectMap.end();
}

bool Context::hasSegment(const string& id) const {
	return segmentMap.find(id) != segmentMap.end();
}

bool Context::isCompleted() const {
	typedef pair<const Segment*, map<const Module*, long> > P;
	typedef pair<const Module*, long> Q;

	foreach(P p, lastComputedLMap)
			{
				const long maxL = p.first->getGrid().getMaxL();

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
				if (grid.getMaxInMemoryL() < grid.getMaxL()) {
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

time_t Context::getStartTime() const {
    return startTime;
}

void Context::setStartTime(time_t t) {
    startTime = t;
}

void Context::moveForward(shared_ptr<Segment> segment) const {
	long lastComputedL = segment->getGrid().getMaxInMemoryL();

	if (contains(lastComputedLMap, segment.get())) {
		typedef pair<const Module*, long> Q;

		foreach(Q q, lastComputedLMap.at(segment.get()))
				{
					lastComputedL = min(lastComputedL, q.second);
				}
	}
	long l = min(lastComputedL + 1, getFirstRequiredL(*segment));
	if (l + segment->getGrid().getSizeL() - 1 > segment->getGrid().getMaxL()) {
		l = segment->getGrid().getMaxL() - segment->getGrid().getSizeL() + 1;
	}
	if (l > segment->getGrid().getMinInMemoryL()) {
		getLogging().debug("Moving segment [" + segment->toString() + "] forward to line " + lexical_cast<string>(l), "Context");
		segment->moveForward(l);
	}
}

void Context::moveSegmentsForward() const {
	foreach(shared_ptr<Segment> segment, segmentList)
			{
				moveForward(segment);
			}
}

long Context::getLastComputedL(const Segment& segment, const Module& module) const {
	if (hasLastComputedL(segment, module)) {
		return lastComputedLMap.at(&segment).at(&module);
	}
	return -1;
}

long Context::getLastComputableL(const Segment& segment, const Module& module) const {
	if (!contains(segmentList, segment)) {
		BOOST_THROW_EXCEPTION( invalid_argument("Unknown segment '" + segment.getId() + "'."));
	}
	if (!contains(moduleList, module)) {
		BOOST_THROW_EXCEPTION( invalid_argument("Unknown module '" + module.getId() + "'."));
	}
	long lastComputedL = min(segment.getGrid().getMaxInMemoryL(), segment.getGrid().getMaxL());
	foreach(shared_ptr<Module> m, moduleList)
			{
				if (m.get() == &module) {
					break;
				}
				if (hasLastComputedL(segment, *m)) {
					lastComputedL = min(lastComputedL, lastComputedLMap.at(&segment).at(m.get()));
				}
			}
	return lastComputedL;
}

bool Context::hasFirstRequiredL(const Segment& segment, const Module& module) const {
	return contains(firstRequiredLMap, segment) && contains(firstRequiredLMap.at(&segment), module) && firstRequiredLMap.at(&segment).at(&module) > 0;
}

bool Context::hasLastComputedL(const Segment& segment, const Module& module) const {
	return contains(lastComputedLMap, segment) && contains(lastComputedLMap.at(&segment), module) && lastComputedLMap.at(&segment).at(&module) > 0;
}

void Context::setFirstRequiredL(const Segment& segment, const Module& module, long l) {
	if (!contains(segmentList, segment)) {
		BOOST_THROW_EXCEPTION( invalid_argument("Unknown segment '" + segment.getId() + "'."));
	}
	if (!contains(moduleList, module)) {
		BOOST_THROW_EXCEPTION( invalid_argument("Unknown module '" + module.getId() + "'."));
	}
	getLogging().debug("Segment " + segment.toString() + ": first required L = " + lexical_cast<string>(l), module.getId());
	if (l < 0) {
		firstRequiredLMap[&segment].erase(&module);
	} else {
		firstRequiredLMap[&segment][&module] = l;
	}
}

void Context::setLastComputedL(const Segment& segment, const Module& module, long l) {
	if (!contains(segmentList, segment)) {
		BOOST_THROW_EXCEPTION( invalid_argument("Unknown segment '" + segment.getId() + "'."));
	}
	if (!contains(moduleList, module)) {
		BOOST_THROW_EXCEPTION( invalid_argument("Unknown module '" + module.getId() + "'."));
	}
	if (l >= 0 && ((hasLastComputedL(segment, module) && l < getLastComputedL(segment, module)) || l > segment.getGrid().getMaxL())) {
		BOOST_THROW_EXCEPTION( invalid_argument("Invalid row index l = " + lexical_cast<string > (l)));
	}
	getLogging().debug("Segment " + segment.toString() + ": last computed L = " + lexical_cast<string>(l), module.getId());
	if (l < 0) {
		lastComputedLMap[&segment].erase(&module);
	} else {
		lastComputedLMap[&segment][&module] = l;
	}
}

long Context::getFirstComputableL(const Segment& segment, const Module& module) const {
	if (hasLastComputedL(segment, module)) {
		return max(segment.getGrid().getMinInMemoryL(), getLastComputedL(segment, module) + 1);
	}
	return segment.getGrid().getMinInMemoryL();
}

long Context::getFirstRequiredL(const Segment& segment) const {
	long firstRequiredL = min(segment.getGrid().getMaxInMemoryL() + 1, segment.getGrid().getMaxL());
	foreach (shared_ptr<Module> module, moduleList)
			{
				if (hasFirstRequiredL(segment, *module)) {
					firstRequiredL = min(firstRequiredL, firstRequiredLMap.at(&segment).at(module.get()));
				}
			}
	return firstRequiredL;
}

void Context::setSourcePath(const string& sourcePath) {
    this->sourcePath = sourcePath;
}

const string& Context::getSourcePath() const {
    return sourcePath;
}
