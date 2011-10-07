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
 * File:   Context.h
 * Author: ralf
 *
 * Created on December 20, 2010, 12:34 PM
 */

#ifndef CONTEXT_H
#define	CONTEXT_H

#include <algorithm>
#include <stdexcept>
#include <map>
#include <vector>

#include "Boost.h"
#include "Constants.h"
#include "Dictionary.h"
#include "ErrorHandler.h"
#include "Identifiable.h"
#include "JobOrder.h"
#include "Logging.h"
#include "Segment.h"

using std::exception;
using std::map;
using std::vector;

class Module;

/**
 * Represents the context of a processing.
 */
// todo - divide context into ModuleContext, ProcessorContext, RuntimeContext (nf-20110419)
class Context {
public:
	/**
	 * Constructs a new instance of this class.
	 */
	Context();

	/**
	 * Destructor.
	 */
	~Context();

	/**
	 * Adds a module to the context.
	 * @param module The module.
	 */
	void addModule(shared_ptr<Module> module) {
		moduleList.push_back(module);
	}

	bool removeModule(shared_ptr<Module> module);

	/**
	 * Adds an object to the context.
	 * @param object The object.
	 */
	void addObject(shared_ptr<Identifiable> object) throw (logic_error);

	/**
	 * Adds a segment to the context.
	 * @param id The segment ID.
	 * @param sizeL The size of the segment's row index dimension.
	 * @param sizeM The size of the segment's column index dimension.
	 * @param sizeK The size of the segment's camera index dimension.
	 * @param minL The minimum valid row index.
	 * @param maxL The maximum valid row index.
	 * @return a reference to the segment added.
	 */
	Segment& addSegment(const string& id, long sizeL, long sizeM, long sizeK, long minL, long maxL) throw (logic_error);

	bool removeSegment(const string& id);

	/**
	 * Returns a list of modules, which have been added to the context.
	 * @return a list of modules, which have been added to the context.
	 */
	vector<shared_ptr<Module> > getModules() const {
		return moduleList;
	}

	/**
	 * Returns the dictionary.
	 * @return the dictionary.
	 */
	Dictionary& getDictionary() const throw (logic_error) {
		if (dictionary == 0) {
			BOOST_THROW_EXCEPTION(logic_error("No dictionary set."));
		}
		return *dictionary;
	}

	/**
	 * Sets the dictionary.
	 * @param dictionary The dictionary.
	 */
	void setDictionary(shared_ptr<Dictionary> dictionary) {
		this->dictionary = dictionary;
	}

	/**
	 * Returns the job order.
	 * @return the job order.
	 */
	JobOrder& getJobOrder() const throw (logic_error) {
		if (jobOrder == 0) {
			BOOST_THROW_EXCEPTION(logic_error("No job order set."));
		}
		return *jobOrder;
	}

	/**
	 * Sets the job order.
	 * @param jobOrder The job order.
	 */
	void setJobOrder(shared_ptr<JobOrder> jobOrder) {
		this->jobOrder = jobOrder;
	}

	/**
	 * Returns the logging.
	 * @return the logging.
	 */
	shared_ptr<Logging> getLogging() const {
		return logging;
	}

	/**
	 * Sets the logging.
	 * @param logging The logging.
	 */
	void setLogging(shared_ptr<Logging> logging) {
		this->logging = logging;
	}

	/**
	 * Returns the object associated with the supplied object ID.
	 * @param id The object ID.
	 * @return the object associated with {@code id}.
	 */
	Identifiable& getObject(const string& id) const;

	/**
	 * Returns the segment associated with the supplied segment ID.
	 * @param id The segment ID.
	 * @return the segment associated with {@code id}.
	 */
	Segment& getSegment(const string& id) const;

	/**
	 * Returns a vector containing all segment ids.
	 * @return A vector containing all segment ids.
	 */
	vector<string> getSegmentIds() const {
		vector<string> ids;
		foreach (shared_ptr<Segment> s, segmentList)
				{
					ids.push_back(s->getId());
				}
		return ids;
	}

	/**
	 * Returns the index of the last row in a segment, which has been
	 * computed by a certain module.
	 * @param segment The segment.
	 * @param module The module.
	 * @return the index of the last row in {@code segment}, which has
	 *         been computed by {@code module}.
	 */
	long getLastComputedL(const Segment& segment, const Module& module) const;

	void setFirstRequiredL(const Segment& segment, const Module& module, long l);

	/**
	 * Sets the index of the last row in a segment, which has been
	 * computed by a certain module.
	 * @param segment The segment.
	 * @param module The module.
	 * @param l The index of the last row in {@code segment}, which has
	 *          been computed by {@code module}.
	 */
	void setLastComputedL(const Segment& segment, const Module& module, long l);

	/**
	 * Returns the index of the first row in a given segment, which has not
	 * been computed by a module.
	 * @param segment The segment.
	 * @param module The module.
	 * @return The index of the first row, which has not been computed by the
	 *         given module.
	 */
	long getFirstComputableL(const Segment& segment, const Module& module) const;

	/**
	 * Returns the index of the last row in a given segment, which has been
	 * computed by all preceding modules, and can therefore be computed.
	 * @param segment The segment.
	 * @param module The module.
	 * @return The index of the last row in the given segment, which can be
	 *         computed.
	 */
	long getLastComputableL(const Segment& segment, const Module& module) const;

	/**
	 * Inquires the context about an object.
	 * @param id The object ID.
	 * @return {@code true} if an object with the given ID has been added to
	 *         the context, {@code false} otherwise.
	 */
	bool hasObject(const string& id) const;

	/**
	 * Inquires the context about a segment.
	 * @param id The segment ID.
	 * @return {@code true} if a segement with the given ID has been added to
	 *         the context, {@code false} otherwise.
	 */
	bool hasSegment(const string& id) const;

	/**
	 * Inquires the context about the index of the last row in a segment,
	 * which has been computed by a certain module.
	 * @param segment The segment.
	 * @param module The module.
	 * @return {@code true} if the context has the requested information,
	 *         {@code false} otherwise.
	 */
	bool hasLastComputedL(const Segment& segment, const Module& module) const;

	bool isCompleted() const;

	void moveSegmentsForward() const;

	void setErrorHandler(shared_ptr<ErrorHandler> errorHandler);

	void handleError(exception& e);

private:
	/**
	 * Returns the index of the first row in a segment, which is required for
	 * processing the given segment.
	 * @param segment The segment.
	 * @return the index of the first row in {@code segment}, which is
	 *         required for processing.
	 */
	long getFirstRequiredL(const Segment& segment) const;

	/**
	 * Inquires the context about the index of the first row in a segment,
	 * which is required by a certain module.
	 * @param segment The segment.
	 * @param module The module.
	 * @return {@code true} if the context has the requested information,
	 *         {@code false} otherwise.
	 */
	bool hasFirstRequiredL(const Segment& segment, const Module& module) const;

	void moveForward(shared_ptr<Segment> segment) const;

	template<class Identifiable>
	class Id {
	public:
		Id(const string& id) :
				id(id) {
		}
		~Id() {
		}

		bool operator()(shared_ptr<Identifiable> identifiable) {
			return identifiable->getId().compare(id) == 0;
		}
	private:
		const string& id;
	};

	template<class K, class V>
	static bool contains(const map<K, V>& map, const K& key) {
		return map.find(key) != map.end();
	}

	template<class K, class V>
	static bool contains(const map<const K*, V>& map, const K* key) {
		return map.find(key) != map.end();
	}

	template<class K, class V>
	static bool contains(const map<const K*, V>& map, const K& key) {
		return map.find(&key) != map.end();
	}

	template<class T>
	static bool contains(const vector<T*>& vector, const T& value) {
		return std::find(vector.begin(), vector.end(), &value) != vector.end();
	}

	template<class T>
	static bool contains(const vector<shared_ptr<T> >& vector, const T& value) {
		foreach (shared_ptr<T> p, vector)
				{
					if (p.get() == &value) {
						return true;
					}
				}
		return false;
	}

	shared_ptr<Logging> logging;
	shared_ptr<Dictionary> dictionary;
	shared_ptr<JobOrder> jobOrder;
	shared_ptr<ErrorHandler> errorHandler;

	vector<shared_ptr<Module> > moduleList;
	map<string, shared_ptr<Identifiable> > objectMap;
	map<string, shared_ptr<Segment> > segmentMap;
	vector<shared_ptr<Segment> > segmentList;

	map<const Segment*, map<const Module*, long> > firstRequiredLMap;
	map<const Segment*, map<const Module*, long> > lastComputedLMap;
};

#endif	/* CONTEXT_H */
