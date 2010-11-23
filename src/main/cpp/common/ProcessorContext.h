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

#include "Segment.h"

using std::map;
using std::pair;
using std::string;
using std::vector;

class Module;

class ProcessorContext {
public:
    ProcessorContext();
    virtual ~ProcessorContext();

    void addSegment(Segment& segment);
    bool containsSegment(const Segment& segment) const;
    size_t getMaxComputedLine(const Segment& segment, const Module& module) const;
    size_t getMinRequiredLine(const Segment& segment) const;
    Segment& getSegment(const string& segmentId) const;

    void setMaxComputedLine(const Segment& segment, const Module& module, size_t line);
    void setMinRequiredLine(const Segment& segment, size_t line);

private:
    vector<Segment*> segments;

    size_t maxComputedLine;
    size_t minRequiredLine;
    
};

#endif	/* PROCESSORCONTEXT_H */

