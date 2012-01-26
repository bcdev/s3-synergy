/*
 * MapAccessor.h
 *
 *  Created on: Jan 17, 2012
 *      Author: ralf
 */

#ifndef MAPACCESSOR_H_
#define MAPACCESSOR_H_

#include <algorithm>
#include <limits>
#include <map>
#include <typeinfo>

#include "Accessor.h"
#include "Boost.h"
#include "Constants.h"

using std::copy;
using std::fill;
using std::invalid_argument;
using std::map;
using std::numeric_limits;
using std::logic_error;

template<class T, int N>
class MapAccessor: public virtual Accessor {
public:
	MapAccessor(T fillValue = numeric_limits<T>::min(), double scaleFactor = 1.0, double addOffset = 0.0) :
			Accessor(), fillValue(fillValue), scaleFactor(scaleFactor), addOffset(addOffset), data() {
	}

	~MapAccessor() {
	}

	int8_t getByte(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<int8_t>(getValue(i));
	}

	void setByte(size_t i, int8_t value) throw (bad_cast, out_of_range) {
		data[i] = boost::numeric_cast<T>(value);
	}

	double getDouble(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<double>(getValue(i)) * scaleFactor + addOffset;
	}

	void setDouble(size_t i, double value) throw (bad_cast, out_of_range) {
		value = (value - addOffset) / scaleFactor;
		if (value > numeric_limits<T>::max()) {
			data[i] = numeric_limits<T>::max();
		} else {
			data[i] = boost::numeric_cast<T>(value);
		}
	}

	float getFloat(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<float>(getValue(i)) * float(scaleFactor) + float(addOffset);
	}

	void setFloat(size_t i, float value) throw (bad_cast, out_of_range) {
		value = (value - float(addOffset)) / float(scaleFactor);
		if (value > numeric_limits<T>::max()) {
			data[i] = numeric_limits<T>::max();
		} else {
			data[i] = boost::numeric_cast<T>(value);
		}
	}

	int32_t getInt(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<int32_t>(getValue(i));
	}

	void setInt(size_t i, int32_t value) throw (bad_cast, out_of_range) {
		data[i] = boost::numeric_cast<T>(value);
	}

	int64_t getLong(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<int64_t>(getValue(i));
	}

	void setLong(size_t i, int64_t value) throw (bad_cast, out_of_range) {
		data[i] = boost::numeric_cast<T>(value);
	}

	int16_t getShort(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<int16_t>(getValue(i));
	}

	void setShort(size_t i, int16_t value) throw (bad_cast, out_of_range) {
		data[i] = boost::numeric_cast<int16_t>(value);
	}

	uint8_t getUByte(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<uint8_t>(getValue(i));
	}

	void setUByte(size_t i, uint8_t value) throw (bad_cast, out_of_range) {
		data[i] = boost::numeric_cast<T>(value);
	}

	uint32_t getUInt(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<uint32_t>(getValue(i));
	}

	void setUInt(size_t i, uint32_t value) throw (bad_cast, out_of_range) {
		data[i] = boost::numeric_cast<T>(value);
	}

	uint64_t getULong(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<uint64_t>(getValue(i));
	}

	void setULong(size_t i, uint64_t value) throw (bad_cast, out_of_range) {
		data[i] = boost::numeric_cast<T>(value);
	}

	uint16_t getUShort(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<uint16_t>(getValue(i));
	}

	void setUShort(size_t i, uint16_t value) throw (bad_cast, out_of_range) {
		data[i] = boost::numeric_cast<T>(value);
	}

	const T* getValue(size_t i) const throw (out_of_range) {
        if (isFillValue(i)) {
            return &fillValue;
        }
        return &(data.at(i));
	}

	const void* getUntypedValue(size_t i) const throw (bad_cast, out_of_range) {
	    return getValue(i);
	}

	const size_t getSampleCount() const {
		return data.size();
	}

	virtual valarray<int8_t>& getByteData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	virtual valarray<double>& getDoubleData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	virtual valarray<float>& getFloatData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	virtual valarray<int32_t>& getIntData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	virtual valarray<int64_t>& getLongData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	virtual valarray<int16_t>& getShortData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	virtual valarray<uint8_t>& getUByteData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	virtual valarray<uint32_t>& getUIntData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	virtual valarray<uint64_t>& getULongData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	virtual valarray<uint16_t>& getUShortData() const throw (logic_error) {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	virtual valarray<double> getDoubles() const {
		BOOST_THROW_EXCEPTION(logic_error("Not implemented."));
	}

	virtual valarray<float> getFloats() const {
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

	bool isFillValue(size_t i) const throw (out_of_range) {
		if (data.find(i) == data.end()) {
			return true;
		}
		return data.at(i) == fillValue;
	}

	void setFillValue(size_t i) throw (out_of_range) {
		data.erase(i);
	}

	string getFillValue() const {
		if (getType() == Constants::TYPE_BYTE) {
			return lexical_cast<string>(numeric_cast<int16_t>(fillValue));
		}
		if (getType() == Constants::TYPE_UBYTE) {
			return lexical_cast<string>(numeric_cast<uint16_t>(fillValue));
		}
		return lexical_cast<string>(fillValue);
	}

	void shift(long n, long strideK, long strideL) {
		// intentionally doing nothing
	}

	bool canReturnDataPointer() const {
		return false;
	}

private:

	const T fillValue;
	const double scaleFactor;
	const double addOffset;

	map<long, T> data;
};

#endif /* MAPACCESSOR_H_ */
