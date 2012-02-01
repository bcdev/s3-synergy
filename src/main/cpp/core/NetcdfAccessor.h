/*
 * NetcdfAccessor.h
 *
 *  Created on: Feb 1, 2012
 *      Author: ralf
 */

#ifndef NETCDFACCESSOR_H_
#define NETCDFACCESSOR_H_

#include "Accessor.h"
#include "Boost.h"
#include "Dictionary.h"
#include "Grid.h"
#include "NetCDF.h"

using std::logic_error;

template<class T, int N>
class NetcdfAccessor: public virtual Accessor {
public:
	NetcdfAccessor(const VariableDescriptor& variableDescriptor, const Grid& grid, const string& directoryPath);
	~NetcdfAccessor();

	int8_t getByte(size_t i) const throw (bad_cast, out_of_range);
	void setByte(size_t i, int8_t value) throw (bad_cast, out_of_range);
	double getDouble(size_t i) const throw (bad_cast, out_of_range);
	void setDouble(size_t i, double value) throw (bad_cast, out_of_range);
	float getFloat(size_t i) const throw (bad_cast, out_of_range);
	void setFloat(size_t i, float value) throw (bad_cast, out_of_range);
	int32_t getInt(size_t i) const throw (bad_cast, out_of_range);
	void setInt(size_t i, int32_t value) throw (bad_cast, out_of_range);
	int64_t getLong(size_t i) const throw (bad_cast, out_of_range);
	void setLong(size_t i, int64_t value) throw (bad_cast, out_of_range);
	int16_t getShort(size_t i) const throw (bad_cast, out_of_range);
	void setShort(size_t i, int16_t value) throw (bad_cast, out_of_range);
	uint8_t getUByte(size_t i) const throw (bad_cast, out_of_range);
	void setUByte(size_t i, uint8_t value) throw (bad_cast, out_of_range);
	uint32_t getUInt(size_t i) const throw (bad_cast, out_of_range);
	void setUInt(size_t i, uint32_t value) throw (bad_cast, out_of_range);
	uint64_t getULong(size_t i) const throw (bad_cast, out_of_range);
	void setULong(size_t i, uint64_t value) throw (bad_cast, out_of_range);
	uint16_t getUShort(size_t i) const throw (bad_cast, out_of_range);
	void setUShort(size_t i, uint16_t value) throw (bad_cast, out_of_range);
	const size_t getSampleCount() const;

	valarray<int8_t>& getByteData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<double>& getDoubleData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<float>& getFloatData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<int32_t>& getIntData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<int64_t>& getLongData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<int16_t>& getShortData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<uint8_t>& getUByteData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<uint32_t>& getUIntData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<uint64_t>& getULongData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<uint16_t>& getUShortData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<double> getDoubles() const {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	valarray<float> getFloats() const {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	void* getUntypedData() const {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	int getType() const {
		return N;
	}

	double getScaleFactor() const {
		return scaleFactor;
	}

	double getAddOffset() const {
		return addOffset;
	}

	bool isFillValue(size_t i) const throw (out_of_range);
	void setFillValue(size_t i) throw (out_of_range);
	string getFillValue() const;

	void shift(long n, long strideK, long strideL) {
		// intentionally doing nothing
	}

	bool canReturnDataPointer() const {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

private:

	const double scaleFactor;
	const double addOffset;
	const Grid& grid;

	T fillValue;

	int fileId;
	int varId;

	mutable valarray<size_t> origin;
	const valarray<size_t> shape;

	T getValue(size_t i) const;
	void setValue(size_t i, T value);
};

template<class T, int N>
T NetcdfAccessor<T, N>::getValue(size_t i) const {
    origin[0] = grid.getL(i);
    origin[1] = grid.getM(i);
    T result;
    NetCDF::getVariableData(fileId, varId, origin, shape, &result);
    return result;
}

template<class T, int N>
NetcdfAccessor<T, N>::NetcdfAccessor(const VariableDescriptor& variableDescriptor, const Grid& grid, const string& directoryPath) : scaleFactor(variableDescriptor.getScaleFactor()), addOffset(variableDescriptor.getAddOffset()), grid(grid), origin(2), shape(1, 2) {
    switch(N) {
    case Constants::TYPE_BYTE:
        fillValue = numeric_cast<T>(variableDescriptor.getFillValue<int16_t>());
        break;
    case Constants::TYPE_UBYTE:
        fillValue = numeric_cast<T>(variableDescriptor.getFillValue<uint16_t>());
        break;
    default:
        fillValue = variableDescriptor.getFillValue<T>();
        break;
    }
    if(variableDescriptor.getDimensions().size() != 2) {
        BOOST_THROW_EXCEPTION(logic_error("variableDescriptor.getDimensions().size() != 2"));
    }
    const path targetPath = path(directoryPath + "/" + variableDescriptor.getNcFileBasename() + ".nc");
    if (!boost::filesystem::exists(targetPath)) {
        if (!boost::filesystem::exists(targetPath.parent_path()) && !boost::filesystem::create_directories(targetPath.parent_path())) {
            BOOST_THROW_EXCEPTION( runtime_error("Cannot create directory '" + targetPath.parent_path().string() + "'."));
        }
        fileId = NetCDF::createFile(targetPath.string());
        // todo - add global attributes, but not here
    } else {
        fileId = NetCDF::openFileWritable(targetPath.string());
    }
    const string varName = variableDescriptor.getName();
    varId = NetCDF::getVariableId(fileId, variableDescriptor.getName());
    if(varId == -1) {
        valarray<int> dimIds;
        NetCDF::defineDimensions(fileId, varName, variableDescriptor.getDimensions(), grid, dimIds);
        varId = NetCDF::defineVariable(fileId, varName, N, dimIds, false);
        foreach(const Attribute* attribute, variableDescriptor.getAttributes()) {
            NetCDF::putAttribute(fileId, varId, *attribute);
        }
    }
}

template<class T, int N>
NetcdfAccessor<T, N>::~NetcdfAccessor() {
    NetCDF::closeFile(fileId);
}

template<class T, int N>
int8_t NetcdfAccessor<T, N>::getByte(size_t i) const throw (bad_cast, out_of_range) {
    return numeric_cast<int8_t>(getValue(i));
}

template<class T, int N>
void NetcdfAccessor<T, N>::setByte(size_t i, int8_t value) throw (bad_cast, out_of_range) {
    setValue(i, numeric_cast<T>(value));
}

template<class T, int N>
int16_t NetcdfAccessor<T, N>::getShort(size_t i) const throw (bad_cast, out_of_range) {
    return numeric_cast<int16_t>(getValue(i));
}

template<class T, int N>
void NetcdfAccessor<T, N>::setShort(size_t i, int16_t value) throw (bad_cast, out_of_range) {
    setValue(i, numeric_cast<T>(value));
}

template<class T, int N>
int32_t NetcdfAccessor<T, N>::getInt(size_t i) const throw (bad_cast, out_of_range) {
    return numeric_cast<int32_t>(getValue(i));
}

template<class T, int N>
void NetcdfAccessor<T, N>::setInt(size_t i, int32_t value) throw (bad_cast, out_of_range) {
    setValue(i, numeric_cast<T>(value));
}

template<class T, int N>
int64_t NetcdfAccessor<T, N>::getLong(size_t i) const throw (bad_cast, out_of_range) {
    return numeric_cast<int64_t>(getValue(i));
}

template<class T, int N>
void NetcdfAccessor<T, N>::setLong(size_t i, int64_t value) throw (bad_cast, out_of_range) {
    setValue(i, numeric_cast<T>(value));
}

template<class T, int N>
uint8_t NetcdfAccessor<T, N>::getUByte(size_t i) const throw (bad_cast, out_of_range) {
    return numeric_cast<uint8_t>(getValue(i));
}

template<class T, int N>
void NetcdfAccessor<T, N>::setUByte(size_t i, uint8_t value) throw (bad_cast, out_of_range) {
    setValue(i, numeric_cast<T>(value));
}

template<class T, int N>
uint16_t NetcdfAccessor<T, N>::getUShort(size_t i) const throw (bad_cast, out_of_range) {
    return numeric_cast<uint16_t>(getValue(i));
}

template<class T, int N>
void NetcdfAccessor<T, N>::setUShort(size_t i, uint16_t value) throw (bad_cast, out_of_range) {
    setValue(i, numeric_cast<T>(value));
}

template<class T, int N>
uint32_t NetcdfAccessor<T, N>::getUInt(size_t i) const throw (bad_cast, out_of_range) {
    return numeric_cast<uint32_t>(getValue(i));
}

template<class T, int N>
void NetcdfAccessor<T, N>::setUInt(size_t i, uint32_t value) throw (bad_cast, out_of_range) {
    setValue(i, numeric_cast<T>(value));
}

template<class T, int N>
uint64_t NetcdfAccessor<T, N>::getULong(size_t i) const throw (bad_cast, out_of_range) {
    return numeric_cast<uint64_t>(getValue(i));
}

template<class T, int N>
void NetcdfAccessor<T, N>::setULong(size_t i, uint64_t value) throw (bad_cast, out_of_range) {
    setValue(i, numeric_cast<T>(value));
}

template<class T, int N>
double NetcdfAccessor<T, N>::getDouble(size_t i) const throw (bad_cast, out_of_range) {
    return getValue(i) * scaleFactor + addOffset;
}

template<class T, int N>
void NetcdfAccessor<T, N>::setDouble(size_t i, double value) throw (bad_cast, out_of_range) {
    value = (value - addOffset) / scaleFactor;
    if (value > numeric_limits<T>::max()) {
        setValue(i, numeric_limits<T>::max());
    } else {
        setValue(i, boost::numeric_cast<T>(value));
    }
}

template<class T, int N>
float NetcdfAccessor<T, N>::getFloat(size_t i) const throw (bad_cast, out_of_range) {
    return getValue(i) * scaleFactor + addOffset;
}

template<class T, int N>
void NetcdfAccessor<T, N>::setFloat(size_t i, float value) throw (bad_cast, out_of_range) {
    value = (value - addOffset) / scaleFactor;
    if (value > numeric_limits<T>::max()) {
        setValue(i, numeric_limits<T>::max());
    } else {
        setValue(i, boost::numeric_cast<T>(value));
    }
}

template<class T, int N>
const size_t NetcdfAccessor<T, N>::getSampleCount() const {
    BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
}

template<class T, int N>
void NetcdfAccessor<T, N>::setValue(size_t i, T value) {
    origin[0] = grid.getL(i);
    origin[1] = grid.getM(i);
    NetCDF::putValue(fileId, varId, origin, &value);
}

template<class T, int N>
bool NetcdfAccessor<T, N>::isFillValue(size_t i) const throw (out_of_range) {
    return getValue(i) == fillValue;
}

template<class T, int N>
void NetcdfAccessor<T, N>::setFillValue(size_t i) throw (out_of_range) {
    setValue(i, fillValue);
}

template<class T, int N>
string NetcdfAccessor<T, N>::getFillValue() const {
    if (N == Constants::TYPE_BYTE) {
        return lexical_cast<string>(numeric_cast<int16_t>(fillValue));
    }
    if (N == Constants::TYPE_UBYTE) {
        return lexical_cast<string>(numeric_cast<uint16_t>(fillValue));
    }
    return lexical_cast<string>(fillValue);
}

#endif /* NETCDFACCESSOR_H_ */
