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
 * File:   SegmentImpl.h
 * Author: ralf
 *
 * Created on December 15, 2010, 5:28 PM
 */

#ifndef SEGMENTIMPL_H
#define	SEGMENTIMPL_H

#include <map>
#include <vector>

#include "Constants.h"
#include "GridImpl.h"
#include "Segment.h"

using std::map;
using std::vector;

class SegmentImpl : public virtual Segment {
public:
    SegmentImpl(const string& id, size_t sizeL, size_t sizeM = Constants::N_DET_CAM, size_t sizeK = Constants::N_CAM);
    virtual ~SegmentImpl();

    void addVariableByte(const string& varName) throw (logic_error);
    void addVariableDouble(const string& varName) throw (logic_error);
    void addVariableFloat(const string& varName) throw (logic_error);
    void addVariableInt(const string& varName) throw (logic_error);
    void addVariableLong(const string& varName) throw (logic_error);
    void addVariableShort(const string& varName) throw (logic_error);
    void addVariableUByte(const string& varName) throw (logic_error);
    void addVariableUInt(const string& varName) throw (logic_error);
    void addVariableULong(const string& varName) throw (logic_error);
    void addVariableUShort(const string& varName) throw (logic_error);

    bool hasVariable(const string& varName) const;

    const string& getId() const;
    Grid& getGrid();
    Accessor& getAccessor(const string& varName) const;
    
    const string toString();

private:
    void unique(const string& varName) const throw (logic_error);

    static const string className;

    const string id;
    GridImpl grid;
    map<string, Accessor*> accessorMap;
    vector<Accessor*> accessorList;
};

#endif	/* SEGMENTIMPL_H */

