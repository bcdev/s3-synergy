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
 * File:   ProcessorContext.h
 * Author: ralf
 *
 * Created on November 22, 2010, 10:58 AM
 */

#ifndef PROCESSORCONTEXT_H
#define	PROCESSORCONTEXT_H

#include <map>
#include <string>
#include <vector>

#include "JobOrder.h"
#include "Logger.h"
#include "../core/Segment.h"

using std::map;
using std::pair;
using std::string;
using std::vector;

class Module;

class ProcessorContext {
public:
    ProcessorContext(JobOrder jobOrder);
    virtual ~ProcessorContext();

    void addSegment(Segment& segment);
    bool containsSegment(const Segment& segment) const;
    bool containsSegment(const string& segmentId) const;
    /**
     * Returns the maximum line in the product data associated with a certain
     * segment, which is going to be processed in this context.
     * @param segment The segment.
     * @return the maximum line in the product data to be processed.
     */
    size_t getMaxLine(const Segment& segment) const;
    /**
     * Returns the maximum line in the product data associated with a certain
     * segment, which has been computed by a certain module.
     * @param segment The segment.
     * @param module The module.
     * @return the maximum line in the product data, which has been computed by
     * the module.
     */
    size_t getMaxLineComputed(const Segment& segment, const Module& module) const;
    /**
     * Returns the minimum line in the product data associated with a certain
     * segment, which is required for processing lines succeeding the maximum
     * computed line.
     * @param segment The segment.
     * @return the minimum line in the product data required for processing
     * lines beyond the maximum computed line.
     */
    size_t getMinLineRequired(const Segment& segment) const;
    /**
     * Returns the segment associated with a given segment ID.
     * @param segmentId The segment ID.
     * @return the segment associated with the ID.
     */
    Segment& getSegment(const string& segmentId) const;
    bool hasMaxLineComputed(const Segment& segment, const Module& module) const;
    bool hasMinLineRequired(const Segment& segment) const;

    void setMaxLine(const Segment& segment, size_t line);
    void setMaxLineComputed(const Segment& segment, const Module& module, size_t line);
    Logger* getLogger();
    JobOrder getJobOrder() const;

private:
    template <class K, class V>
    bool exists(const map<K, V>& map, const K& key) const {
        return map.find(key) != map.end();
    }

    vector<Segment*> segments;

    typedef map<const Module*, size_t> ModuleLineMap;
    map<const Segment*, ModuleLineMap> maxLineComputedMap;

    size_t maxLine;

    JobOrder jobOrder;
};

#endif	/* PROCESSORCONTEXT_H */

