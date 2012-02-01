/*
 * NetcdfSegment.cpp
 *
 *  Created on: Feb 01, 2012
 *      Author: thomasstorm
 */

#include <sstream>
#include <stdexcept>

#include "NetcdfAccessor.h"
#include "NetcdfSegment.h"

using std::invalid_argument;
using std::logic_error;
using std::ostringstream;

NetcdfSegment::NetcdfSegment(const string& name, const string& targetDirPath, long l, long m) :
		id(name), targetDirPath(targetDirPath), grid(1, l, m, 0, l - 1), accessorMap() {
}

NetcdfSegment::~NetcdfSegment() {
}

Accessor& NetcdfSegment::addVariable(const VariableDescriptor& d) throw (logic_error) {
	return addVariable(d, d.getName());
}

Accessor& NetcdfSegment::addVariable(const VariableDescriptor& d, const string& targetName) throw (logic_error) {
	switch (d.getType()) {
	case Constants::TYPE_BYTE:
		return addVariableByte(d);
	case Constants::TYPE_UBYTE:
		return addVariableUByte(d);
	case Constants::TYPE_SHORT:
		return addVariableShort(d);
	case Constants::TYPE_USHORT:
		return addVariableUShort(d);
	case Constants::TYPE_INT:
		return addVariableInt(d);
	case Constants::TYPE_UINT:
		return addVariableUInt(d);
	case Constants::TYPE_LONG:
		return addVariableLong(d);
	case Constants::TYPE_ULONG:
		return addVariableULong(d);
	case Constants::TYPE_FLOAT:
		return addVariableFloat(d);
	case Constants::TYPE_DOUBLE:
		return addVariableDouble(d);
	default:
		BOOST_THROW_EXCEPTION( std::invalid_argument("Cannot add variable '" + targetName + "' to segment '" + getId() + "'. Unsupported data type."));
	}
}

Accessor& NetcdfSegment::addVariable(const string& name, int type, double scaleFactor, double addOffset) throw (logic_error) {
    BOOST_THROW_EXCEPTION(logic_error("not implemented."));
}

Accessor& NetcdfSegment::addVariableAlias(const string& alias, const Segment& segment, const string& name) throw (logic_error) {
	if (segment.hasVariable(name)) {
		if (segment.getGrid().getSizeK() == getGrid().getSizeK() && segment.getGrid().getSizeL() == getGrid().getSizeL() && segment.getGrid().getSizeM() == getGrid().getSizeM()) {
			unique(alias);
			shared_ptr<Accessor> accessor = segment.getSharedAccessor(name);
			accessorMap[alias] = accessor;
			return *accessor;
		}
	}
	BOOST_THROW_EXCEPTION( logic_error("Alias for variable '" + name + "' from segment '" + segment.getId() + "' cannot be added to segment '" + segment.getId() + "'."));
}

Accessor& NetcdfSegment::addVariableByte(const VariableDescriptor& d) throw (logic_error) {
    const string& name = d.getName();
	unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new NetcdfAccessor<int8_t, Constants::TYPE_BYTE>(d, grid, targetDirPath));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& NetcdfSegment::addVariableDouble(const VariableDescriptor& d) throw (logic_error) {
    const string& name = d.getName();
	unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new NetcdfAccessor<double, Constants::TYPE_DOUBLE>(d, grid, targetDirPath));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& NetcdfSegment::addVariableFloat(const VariableDescriptor& d) throw (logic_error) {
    const string& name = d.getName();
	unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new NetcdfAccessor<float, Constants::TYPE_FLOAT>(d, grid, targetDirPath));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& NetcdfSegment::addVariableInt(const VariableDescriptor& d) throw (logic_error) {
    const string& name = d.getName();
	unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new NetcdfAccessor<int32_t, Constants::TYPE_INT>(d, grid, targetDirPath));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& NetcdfSegment::addVariableLong(const VariableDescriptor& d) throw (logic_error) {
    const string& name = d.getName();
	unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new NetcdfAccessor<int64_t, Constants::TYPE_LONG>(d, grid, targetDirPath));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& NetcdfSegment::addVariableShort(const VariableDescriptor& d) throw (logic_error) {
    const string& name = d.getName();
    unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new NetcdfAccessor<int16_t, Constants::TYPE_SHORT>(d, grid, targetDirPath));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& NetcdfSegment::addVariableUByte(const VariableDescriptor& d) throw (logic_error) {
    const string& name = d.getName();
    unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new NetcdfAccessor<uint8_t, Constants::TYPE_UBYTE>(d, grid, targetDirPath));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& NetcdfSegment::addVariableUInt(const VariableDescriptor& d) throw (logic_error) {
    const string& name = d.getName();
    unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new NetcdfAccessor<uint32_t, Constants::TYPE_UINT>(d, grid, targetDirPath));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& NetcdfSegment::addVariableULong(const VariableDescriptor& d) throw (logic_error) {
    const string& name = d.getName();
    unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new NetcdfAccessor<uint64_t, Constants::TYPE_ULONG>(d, grid, targetDirPath));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& NetcdfSegment::addVariableUShort(const VariableDescriptor& d) throw (logic_error) {
	const string& name = d.getName();
	unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new NetcdfAccessor<uint16_t, Constants::TYPE_USHORT>(d, grid, targetDirPath));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& NetcdfSegment::getAccessor(const string& name) const throw (logic_error) {
	if (!hasVariable(name)) {
		BOOST_THROW_EXCEPTION( logic_error("No accessor for variable '" + name + "'."));
	}
	return *accessorMap.at(name);
}

void NetcdfSegment::moveForward(long l) throw (logic_error) {
	// intentionally doing nothing
}

string NetcdfSegment::toString() const {
	ostringstream oss;
	oss << className << "[";
	oss << "id = " << getId() << ", ";
	oss << "firstL = " << getGrid().getFirstL() << ", ";
	oss << "sizeL = " << getGrid().getSizeL() << "]";

	return oss.str();
}

void NetcdfSegment::unique(const string& name) const throw (logic_error) {
	if (hasVariable(name)) {
		BOOST_THROW_EXCEPTION( logic_error("Variable '" + name + "' has already been added to segment '" + id + "'."));
	}
}

const string NetcdfSegment::className = "NetcdfSegment";
