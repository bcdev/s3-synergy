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

#include <limits>
#include <sstream>
#include <stdexcept>

#include "Accessors.h"
#include "SegmentImpl.h"

using std::invalid_argument;
using std::logic_error;
using std::ostringstream;

SegmentImpl::SegmentImpl(const string& s, size_t l, size_t m, size_t k, size_t minL, size_t maxL) : id(s), grid(k, l, m, minL, maxL), accessorMap() {
}

SegmentImpl::~SegmentImpl() {
}

void SegmentImpl::addVariable(const string& name, int type)  throw (logic_error) {
	switch (type) {
	case Constants::TYPE_BYTE:
		addVariableByte(name);
		break;
	case Constants::TYPE_UBYTE:
		addVariableUByte(name);
		break;
	case Constants::TYPE_SHORT:
		addVariableShort(name);
		break;
	case Constants::TYPE_USHORT:
		addVariableUShort(name);
		break;
	case Constants::TYPE_INT:
		addVariableInt(name);
		break;
	case Constants::TYPE_UINT:
		addVariableUInt(name);
		break;
	case Constants::TYPE_LONG:
		addVariableLong(name);
		break;
	case Constants::TYPE_ULONG:
		addVariableULong(name);
		break;
	case Constants::TYPE_FLOAT:
		addVariableFloat(name);
		break;
	case Constants::TYPE_DOUBLE:
		addVariableDouble(name);
		break;
	default:
		BOOST_THROW_EXCEPTION(
				std::invalid_argument("Cannot add variable '" + name + "' to segment '" + getId() + "' because of invalid type."));
		break;
	}
}

void SegmentImpl::addVariableByte(const string& varName) throw (logic_error) {
    unique(varName);
    shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new ByteAccessor(grid.getSize()));
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableDouble(const string& varName) throw (logic_error) {
    unique(varName);
    shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new DoubleAccessor(grid.getSize()));
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableFloat(const string& varName) throw (logic_error) {
    unique(varName);
    shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new FloatAccessor(grid.getSize()));
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableInt(const string& varName) throw (logic_error) {
    unique(varName);
    shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new IntAccessor(grid.getSize()));
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableLong(const string& varName) throw (logic_error) {
    unique(varName);
    shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new LongAccessor(grid.getSize()));
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableShort(const string& varName) throw (logic_error) {
    unique(varName);
    shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new ShortAccessor(grid.getSize()));
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableUByte(const string& varName) throw (logic_error) {
    unique(varName);
    shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new UByteAccessor(grid.getSize()));
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableUInt(const string& varName) throw (logic_error) {
    unique(varName);
    shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new UIntAccessor(grid.getSize()));
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableULong(const string& varName) throw (logic_error) {
    unique(varName);
    shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new ULongAccessor(grid.getSize()));
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

void SegmentImpl::addVariableUShort(const string& varName) throw (logic_error) {
    unique(varName);
    shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new UShortAccessor(grid.getSize()));
    accessorMap[varName] = accessor;
    accessorList.push_back(accessor);
}

bool SegmentImpl::hasVariable(const string& varName) const {
    return accessorMap.find(varName) != accessorMap.end();
}

const string& SegmentImpl::getId() const {
    return id;
}

const Grid& SegmentImpl::getGrid() const {
    return grid;
}

Accessor& SegmentImpl::getAccessor(const string& varName) const throw (logic_error) {
    if (!hasVariable(varName)) {
        BOOST_THROW_EXCEPTION(logic_error("No accessor for variable " + varName + "."));
    }
    return *accessorMap.at(varName);
}

void SegmentImpl::moveForward(size_t l) throw (logic_error) {
    if (l < grid.getFirstL()) {
        BOOST_THROW_EXCEPTION(logic_error("Class: " + className + ": l < grid.getStartL()."));
    }
    if (l > grid.getFirstL() + grid.getSizeL()) {
        BOOST_THROW_EXCEPTION(logic_error("Class: " + className + ": l > grid.getStartL() + grid.getSizeL()."));
    }
    if (l + grid.getSizeL() - 1 > grid.getMaxL()) {
        l = grid.getMaxL() - grid.getSizeL() + 1;
    }
    for (size_t i = 0; i < accessorList.size(); i++) {
        accessorList[i]->shift(l - grid.getFirstL(), grid.getStrideK(), grid.getStrideL());
    }
    grid.setFirstL(l);
}

string SegmentImpl::toString() const {
    ostringstream oss;
    oss << className << "[";
    oss << "id = " << getId() << ", ";
    oss << "startL = " << getGrid().getFirstL() << ", ";
    oss << "sizeL = " << getGrid().getSizeL() << "]";

    return oss.str();
}

void SegmentImpl::unique(const string& varName) const throw (logic_error) {
    if (hasVariable(varName)) {
        BOOST_THROW_EXCEPTION(logic_error("Variable '" + varName + "' has already been added to segment '" + id + "'."));
    }
}

const string SegmentImpl::className = "SegmentImpl";
