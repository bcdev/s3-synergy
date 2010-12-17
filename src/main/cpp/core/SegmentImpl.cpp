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

#include "SegmentImpl.h"

using std::min;
using std::numeric_limits;
using std::ostringstream;

SegmentImpl::SegmentImpl(const string& s, uint16_t r) : id(s), grid(N_CAM, r, N_DET_CAM) {
}

SegmentImpl::~SegmentImpl() {
}

void SegmentImpl::addVariableByte(const string& varName) {

}

void SegmentImpl::addVariableDouble(const string& varName) {

}

void SegmentImpl::addVariableFloat(const string& varName) {

}

void SegmentImpl::addVariableLong(const string& varName) {

}

void SegmentImpl::addVariableInt(const string& varName) {

}

void SegmentImpl::addVariableShort(const string& varName) {

}

void SegmentImpl::addVariableUByte(const string& varName) {

}

void SegmentImpl::addVariableUInt(const string& varName) {

}

void SegmentImpl::addVariableULong(const string& varName) {

}

void SegmentImpl::addVariableUShort(const string& varName) {

}

inline
const string& SegmentImpl::getId() const {
    return id;
}

inline
const Grid& SegmentImpl::getGrid() const {
    return grid;
}

inline
const Grid& SegmentImpl::setGridL(uint16_t l) {
    // todo - implement
}

string SegmentImpl::toString() const {
    std::ostringstream oss;
    oss << className << "[";
    oss << "id = " << getId() << ", ";
    oss << "l = " << getGrid().getL() << ", ";
    oss << "rowCount = " << getGrid().getSizeL() << "]";

    return oss.str();
}

const string SegmentImpl::className = "SegmentImpl";