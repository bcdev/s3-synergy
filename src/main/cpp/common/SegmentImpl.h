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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * File:   SegmentImpl.h
 * Author: thomass
 *
 * Created on November 11, 2010, 10:04 AM
 */

#ifndef SEGMENTIMPL_H
#define	SEGMENTIMPL_H

#include <map>
#include <set>

#include "Segment.h"

using std::map;
using std::string;
using std::set;

class SegmentImpl : public Segment {
public:
    SegmentImpl(const string& segmentId, size_t minL, size_t maxL, size_t minK = 0, size_t maxK = 4, size_t minM = 0, size_t maxM = 760);
    ~SegmentImpl();

    void addIntVariable(const string& varName);
    void addIntVariable(Variable* variable);
    Variable* getIntVariable(const string& varName);
    size_t computePosition(size_t k, size_t l, size_t m) const;
    const string& getId() const;
    size_t getMaxK() const;
    size_t getMinK() const;
    size_t getMaxL() const;
    size_t getMinL() const;
    size_t getMaxM() const;
    size_t getMinM() const;
    void setMaxL(size_t maxL);
    void setMinL(size_t minL);

    string toString() const;

    int getSampleInt(const string& varName, size_t position);
    void setSampleInt(const string& varName, size_t position, int value);
    void setSamplesInt( const string& varName, int* values );

private:
    size_t getValueCount() const;

    const string id;
    
    size_t minL;
    size_t maxL;
    size_t minK;
    size_t maxK;
    size_t minM;
    size_t maxM;

    map<const string*, void*> dataMap;
    set<Variable*> variables;
};

#endif	/* SEGMENTIMPL_H */

