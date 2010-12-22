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

#include <map>
#include <stdexcept>
#include <vector>

#include "Constants.h"
#include "Logging.h"
#include "Segment.h"
#include "Object.h"

using std::map;
using std::logic_error;
using std::vector;

class Dictionary;
class JobOrder;
class Module;

/**
 * Represents the context of a processing.
 */
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
    void addModule(Module& module);

    /**
     * Adds an object to the context.
     * @param object The object.
     */
    void addObject(Object& object) throw (logic_error);

    /**
     * Adds a segement to the context.
     * @param id The segment ID.
     * @param sizeL The size of the segment's row index dimension.
     * @param sizeM The size of the segment's column index dimension.
     * @param sizeK The size of the segment's camara index dimension.
     * @return a reference to the segment added.
     */
    Segment& addSegment(const string& id, size_t sizeL, size_t sizeM = Constants::N_DET_CAM, size_t sizeK = Constants::N_CAM, size_t minL = 0, size_t maxL = Constants::N_LINE_OLC - 1) throw (logic_error);

    /**
     * Returns a list of modules, which have been added to the context.
     * @return a list of modules, which have been added to the context.
     */
    vector<Module*> getModules() const;

    /**
     * Returns the dictionary.
     * @return the dictionary.
     */
    Dictionary* getDictionary() const;

    /**
     * Sets the dictionary.
     * @param dictionary The dictionary.
     */
    void setDictionary(Dictionary* dictionary);

    /**
     * Returns the job order.
     * @return the job order.
     */
    JobOrder* getJobOrder() const;

    /**
     * Sets the job order.
     * @param jobOrder The job order.
     */
    void setJobOrder(JobOrder* jobOrder);

    /**
     * Returns the logging.
     * @return the logging.
     */
    Logging* getLogging() const;

    /**
     * Sets the logging.
     * @param logging The logging.
     */
    void setLogging(Logging* logging);

    /**
     * Returns the object associated with the supplied object ID.
     * @param id The object ID.
     * @return the object associated with {@code id}.
     */
    Object& getObject(const string& id) const;

    /**
     * Returns the segment associated with the supplied segment ID.
     * @param id The segment ID.
     * @return the segment associated with {@code id}.
     */
    Segment& getSegment(const string& id) const;

    /**
     * Returns the index of the maximum row in a segment, which has been
     * computed by a certain module.
     * @param segment The segment.
     * @param module The module.
     * @return the index of the maximum row in {@code segment}, which has
     *         been computed by {@code module}.
     */
    size_t getMaxLComputed(const Segment& segment, const Module& module) const;

    /**
     * Sets the index of the maximum row in a segment, which has been
     * computed by a certain module.
     * @param segment The segment.
     * @param module The module.
     * @param l The index of the maximum row in {@code segment}, which has
     *          been computed by {@code module}.
     */
    void setMaxLComputed(const Segment& segment, const Module& module, size_t l);

    /**
     * Returns the index of the minimum row in a segment, which is required for
     * processing any block of rows starting with a certain row index.
     * @param segment The segment.
     * @param l The row index.
     * @return the index of the minimum row in {@code segment}, which is
     *         required for processing a block of rows starting with row
     *         index {@code l}.
     */
    size_t getMinLRequired(const Segment& segment, size_t l) const;

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
     * Inquires the context about the index of the maximum row in a segment,
     * which has been computed by a certain module.
     * @param segment The segment.
     * @param module The module.
     * @return {@code true} if the context has the requested information,
     *         {@code false} otherwise.
     */
    bool hasMaxLComputed(const Segment& segment, const Module& module) const;

    bool isCompleted() const;

private:
    void removeObject(Object& object);
    void removeSegment(Segment& segment);

    Dictionary* dictionary;
    JobOrder* jobOrder;
    Logging* logging;

    vector<Module*> moduleList;
    map<string, Object*> objectMap;
    map<string, Segment*> segmentMap;
    vector<Segment*> segmentList;
};

#endif	/* CONTEXT_H */
