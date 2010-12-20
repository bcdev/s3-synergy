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
 * File:   SegmentImpl.cpp
 * Author: ralf
 *
 * Created on December 15, 2010, 5:28 PM
 */

#include <algorithm>
#include <limits>
#include <sstream>
#include <stdexcept>

#include "Accessors.h"
#include "SegmentImpl.h"

using std::logic_error;
using std::min;
using std::numeric_limits;
using std::ostringstream;

SegmentImpl::SegmentImpl(const string& s, size_t l, size_t m, size_t k) : id(s), grid(k, l, m), accessorMap() {
}

SegmentImpl::~SegmentImpl() {
    for (size_t i = accessorList.size(); i-- > 0;) {
        delete accessorList[i];
    }
}

void SegmentImpl::addVariableByte(const string& varName) throw (logic_error) {
    unique(varName);
    Accessor* accessor = new ByteAccessor(grid.getSize());
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableDouble(const string& varName) throw (logic_error) {
    unique(varName);
    Accessor* accessor = new DoubleAccessor(grid.getSize());
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableFloat(const string& varName) throw (logic_error) {
    unique(varName);
    Accessor* accessor = new FloatAccessor(grid.getSize());
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableInt(const string& varName) throw (logic_error) {
    unique(varName);
    Accessor* accessor = new IntAccessor(grid.getSize());
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableLong(const string& varName) throw (logic_error) {
    unique(varName);
    Accessor* accessor = new LongAccessor(grid.getSize());
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableShort(const string& varName) throw (logic_error) {
    unique(varName);
    Accessor* accessor = new ShortAccessor(grid.getSize());
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableUByte(const string& varName) throw (logic_error) {
    unique(varName);
    Accessor* accessor = new UByteAccessor(grid.getSize());
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableUInt(const string& varName) throw (logic_error) {
    unique(varName);
    Accessor* accessor = new UIntAccessor(grid.getSize());
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableULong(const string& varName) throw (logic_error) {
    unique(varName);
    Accessor* accessor = new ULongAccessor(grid.getSize());
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableUShort(const string& varName) throw (logic_error) {
    unique(varName);
    Accessor* accessor = new UShortAccessor(grid.getSize());
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

inline
bool SegmentImpl::hasVariable(const string& varName) const {
    return accessorMap.find(varName) != accessorMap.end();
}

inline
const string& SegmentImpl::getId() const {
    return id;
}

inline
Grid& SegmentImpl::getGrid() {
    return grid;
}

Accessor& SegmentImpl::getAccessor(const string& varName) const {
    return *accessorMap.at(varName);
}

string const SegmentImpl::toString() {
    std::ostringstream oss;
    oss << className << "[";
    oss << "id = " << getId() << ", ";
    oss << "startL = " << getGrid().getStartL() << ", ";
    oss << "sizeL = " << getGrid().getSizeL() << "]";

    return oss.str();
}

void SegmentImpl::unique(const string& varName) const throw (logic_error) {
    if (hasVariable(varName)) {
        throw logic_error("variable '" + varName + "' has already been added to segment '" + id + "'.");
    }
}

const string SegmentImpl::className = "SegmentImpl";