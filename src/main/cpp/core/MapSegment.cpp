/*
 * MapSegment.cpp
 *
 *  Created on: Jan 17, 2012
 *      Author: ralf
 */

#include <sstream>
#include <stdexcept>

#include "MapAccessor.h"
#include "MapSegment.h"

using std::invalid_argument;
using std::logic_error;
using std::ostringstream;

MapSegment::MapSegment(const string& s, long l, long m) :
		id(s), grid(1, l, m, 0, l - 1), accessorMap() {
}

MapSegment::~MapSegment() {
}

Accessor& MapSegment::addVariable(const VariableDescriptor& d) throw (logic_error) {
	return addVariable(d, d.getName());
}

Accessor& MapSegment::addVariable(const VariableDescriptor& d, const string& targetName) throw (logic_error) {
	switch (d.getType()) {
	case Constants::TYPE_BYTE:
		return addVariableByte(targetName, numeric_cast<int8_t>(d.getFillValue<int16_t>()), d.getScaleFactor(), d.getAddOffset());
	case Constants::TYPE_UBYTE:
		return addVariableUByte(targetName, numeric_cast<uint8_t>(d.getFillValue<uint16_t>()), d.getScaleFactor(), d.getAddOffset());
	case Constants::TYPE_SHORT:
		return addVariableShort(targetName, d.getFillValue<int16_t>(), d.getScaleFactor(), d.getAddOffset());
	case Constants::TYPE_USHORT:
		return addVariableUShort(targetName, d.getFillValue<uint16_t>(), d.getScaleFactor(), d.getAddOffset());
	case Constants::TYPE_INT:
		return addVariableInt(targetName, d.getFillValue<int32_t>(), d.getScaleFactor(), d.getAddOffset());
	case Constants::TYPE_UINT:
		return addVariableUInt(targetName, d.getFillValue<uint32_t>(), d.getScaleFactor(), d.getAddOffset());
	case Constants::TYPE_LONG:
		return addVariableLong(targetName, d.getFillValue<int64_t>());
	case Constants::TYPE_ULONG:
		return addVariableULong(targetName, d.getFillValue<uint64_t>());
	case Constants::TYPE_FLOAT:
		return addVariableFloat(targetName, d.getFillValue<float>());
	case Constants::TYPE_DOUBLE:
		return addVariableDouble(targetName, d.getFillValue<double>());
	default:
		BOOST_THROW_EXCEPTION( std::invalid_argument("Cannot add variable '" + targetName + "' to segment '" + getId() + "'. Unsupported data type."));
	}
}

Accessor& MapSegment::addVariable(const string& name, int type, double scaleFactor, double addOffset) throw (logic_error) {
	switch (type) {
	case Constants::TYPE_BYTE:
		return addVariableByte(name, numeric_limits<int8_t>::min(), scaleFactor, addOffset);
	case Constants::TYPE_UBYTE:
		return addVariableUByte(name, 0, scaleFactor, addOffset);
	case Constants::TYPE_SHORT:
		return addVariableShort(name, numeric_limits<int16_t>::min(), scaleFactor, addOffset);
	case Constants::TYPE_USHORT:
		return addVariableUShort(name, 0, scaleFactor, addOffset);
	case Constants::TYPE_INT:
		return addVariableInt(name, numeric_limits<int32_t>::min(), scaleFactor, addOffset);
	case Constants::TYPE_UINT:
		return addVariableUInt(name, 0, scaleFactor, addOffset);
	case Constants::TYPE_LONG:
		return addVariableLong(name);
	case Constants::TYPE_ULONG:
		return addVariableULong(name);
	case Constants::TYPE_FLOAT:
		return addVariableFloat(name);
	case Constants::TYPE_DOUBLE:
		return addVariableDouble(name);
	default:
		BOOST_THROW_EXCEPTION( std::invalid_argument("Cannot add variable '" + name + "' to segment '" + getId() + "'. Unsupported data type."));
	}
}

Accessor& MapSegment::addVariableAlias(const string& alias, const Segment& segment, const string& name) throw (logic_error) {
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

Accessor& MapSegment::addVariableByte(const string& name, int8_t fillValue, double scaleFactor, double addOffset) throw (logic_error) {
	unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new MapAccessor<int8_t, Constants::TYPE_BYTE>(fillValue, scaleFactor, addOffset));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& MapSegment::addVariableDouble(const string& name, double fillValue) throw (logic_error) {
	unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new MapAccessor<double, Constants::TYPE_DOUBLE>(fillValue));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& MapSegment::addVariableFloat(const string& name, float fillValue) throw (logic_error) {
	unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new MapAccessor<float, Constants::TYPE_FLOAT>(fillValue));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& MapSegment::addVariableInt(const string& name, int32_t fillValue, double scaleFactor, double addOffset) throw (logic_error) {
	unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new MapAccessor<int32_t, Constants::TYPE_INT>(fillValue, scaleFactor, addOffset));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& MapSegment::addVariableLong(const string& name, int64_t fillValue) throw (logic_error) {
	unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new MapAccessor<int64_t, Constants::TYPE_LONG>(fillValue));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& MapSegment::addVariableShort(const string& name, int16_t fillValue, double scaleFactor, double addOffset) throw (logic_error) {
	unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new MapAccessor<int16_t, Constants::TYPE_SHORT>(fillValue, scaleFactor, addOffset));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& MapSegment::addVariableUByte(const string& name, uint8_t fillValue, double scaleFactor, double addOffset) throw (logic_error) {
	unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new MapAccessor<uint8_t, Constants::TYPE_UBYTE>(fillValue, scaleFactor, addOffset));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& MapSegment::addVariableUInt(const string& name, uint32_t fillValue, double scaleFactor, double addOffset) throw (logic_error) {
	unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new MapAccessor<uint32_t, Constants::TYPE_UINT>(fillValue, scaleFactor, addOffset));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& MapSegment::addVariableULong(const string& name, uint64_t fillValue) throw (logic_error) {
	unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new MapAccessor<uint64_t, Constants::TYPE_ULONG>(fillValue));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& MapSegment::addVariableUShort(const string& name, uint16_t fillValue, double scaleFactor, double addOffset) throw (logic_error) {
	unique(name);
	try {
		shared_ptr<Accessor> accessor = shared_ptr<Accessor>(new MapAccessor<uint16_t, Constants::TYPE_USHORT>(fillValue, scaleFactor, addOffset));
		accessorMap[name] = accessor;
		accessorList.push_back(accessor);
		return *accessor;
	} catch (std::exception& e) {
		BOOST_THROW_EXCEPTION(e);
	}
}

Accessor& MapSegment::getAccessor(const string& name) const throw (logic_error) {
	if (!hasVariable(name)) {
		BOOST_THROW_EXCEPTION( logic_error("No accessor for variable '" + name + "'."));
	}
	return *accessorMap.at(name);
}

void MapSegment::moveForward(long l) throw (logic_error) {
	// intentionally doing nothing
}

string MapSegment::toString() const {
	ostringstream oss;
	oss << className << "[";
	oss << "id = " << getId() << ", ";
	oss << "firstL = " << getGrid().getFirstL() << ", ";
	oss << "sizeL = " << getGrid().getSizeL() << "]";

	return oss.str();
}

void MapSegment::unique(const string& name) const throw (logic_error) {
	if (hasVariable(name)) {
		BOOST_THROW_EXCEPTION( logic_error("Variable '" + name + "' has already been added to segment '" + id + "'."));
	}
}

const string MapSegment::className = "MapSegment";
