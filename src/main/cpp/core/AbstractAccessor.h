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
 * File:   AbstractAccessor.h
 * Author: ralf
 *
 * Created on December 17, 2010, 2:05 PM
 */

#ifndef ABSTRACTACCESSOR_H
#define	ABSTRACTACCESSOR_H

#include <algorithm>
#include <limits>
#include <typeinfo>

#include "Accessor.h"
#include "Boost.h"
#include "Constants.h"

using std::copy;
using std::fill;
using std::invalid_argument;
using std::numeric_limits;

template<class T, int N>
class AbstractAccessor: public virtual Accessor {
public:

	int8_t getByte(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<int8_t>(data[at(i)]);
	}

	void setByte(size_t i, int8_t value) throw (bad_cast, out_of_range) {
		data[at(i)] = boost::numeric_cast<T>(value);
	}

	double getDouble(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<double>(data[at(i)]) * scaleFactor + addOffset;
	}

	void setDouble(size_t i, double value) throw (bad_cast, out_of_range) {
		value = (value - addOffset) / scaleFactor;
		if (value > numeric_limits<T>::max()) {
			data[at(i)] = numeric_limits<T>::max();
		} else {
			data[at(i)] = boost::numeric_cast<T>(value);
		}
	}

	float getFloat(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<float>(data[at(i)]) * float(scaleFactor) + float(addOffset);
	}

	void setFloat(size_t i, float value) throw (bad_cast, out_of_range) {
		value = (value - float(addOffset)) / float(scaleFactor);
		if (value > numeric_limits<T>::max()) {
			data[at(i)] = numeric_limits<T>::max();
		} else {
			data[at(i)] = boost::numeric_cast<T>(value);
		}
	}

	int32_t getInt(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<int32_t>(data[at(i)]);
	}

	void setInt(size_t i, int32_t value) throw (bad_cast, out_of_range) {
		data[at(i)] = boost::numeric_cast<T>(value);
	}

	int64_t getLong(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<int64_t>(data[at(i)]);
	}

	void setLong(size_t i, int64_t value) throw (bad_cast, out_of_range) {
		data[at(i)] = boost::numeric_cast<T>(value);
	}

	int16_t getShort(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<int16_t>(data[at(i)]);
	}

	void setShort(size_t i, int16_t value) throw (bad_cast, out_of_range) {
		data[at(i)] = boost::numeric_cast<int16_t>(value);
	}

	uint8_t getUByte(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<uint8_t>(data[at(i)]);
	}

	void setUByte(size_t i, uint8_t value) throw (bad_cast, out_of_range) {
		data[at(i)] = boost::numeric_cast<T>(value);
	}

	uint32_t getUInt(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<uint32_t>(data[at(i)]);
	}

	void setUInt(size_t i, uint32_t value) throw (bad_cast, out_of_range) {
		data[at(i)] = boost::numeric_cast<T>(value);
	}

	uint64_t getULong(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<uint64_t>(data[at(i)]);
	}

	void setULong(size_t i, uint64_t value) throw (bad_cast, out_of_range) {
		data[at(i)] = boost::numeric_cast<T>(value);
	}

	uint16_t getUShort(size_t i) const throw (bad_cast, out_of_range) {
		return boost::numeric_cast<uint16_t>(data[at(i)]);
	}

	void setUShort(size_t i, uint16_t value) throw (bad_cast, out_of_range) {
		data[at(i)] = boost::numeric_cast<T>(value);
	}

	const void* getUntypedValue(size_t i) const throw (bad_cast, out_of_range) {
		return &data[at(i)];
	}

	const size_t getSampleCount() const {
		return data.size();
	}

	virtual valarray<int8_t>& getByteData() const throw (bad_cast) {
		BOOST_THROW_EXCEPTION(bad_cast());
	}

	virtual valarray<double>& getDoubleData() const throw (bad_cast) {
		BOOST_THROW_EXCEPTION(bad_cast());
	}

	virtual valarray<float>& getFloatData() const throw (bad_cast) {
		BOOST_THROW_EXCEPTION(bad_cast());
	}

	virtual valarray<int32_t>& getIntData() const throw (bad_cast) {
		BOOST_THROW_EXCEPTION(bad_cast());
	}

	virtual valarray<int64_t>& getLongData() const throw (bad_cast) {
		BOOST_THROW_EXCEPTION(bad_cast());
	}

	virtual valarray<int16_t>& getShortData() const throw (bad_cast) {
		BOOST_THROW_EXCEPTION(bad_cast());
	}

	virtual valarray<uint8_t>& getUByteData() const throw (bad_cast) {
		BOOST_THROW_EXCEPTION(bad_cast());
	}

	virtual valarray<uint32_t>& getUIntData() const throw (bad_cast) {
		BOOST_THROW_EXCEPTION(bad_cast());
	}

	virtual valarray<uint64_t>& getULongData() const throw (bad_cast) {
		BOOST_THROW_EXCEPTION(bad_cast());
	}

	virtual valarray<uint16_t>& getUShortData() const throw (bad_cast) {
		BOOST_THROW_EXCEPTION(bad_cast());
	}

	virtual valarray<double> getDoubles() const {
		valarray<double> a(data.size());
		for (size_t i = 0; i < data.size(); i++) {
			a[i] = boost::numeric_cast<double>(data[i]) * scaleFactor + addOffset;
		}
		return a;
	}

	virtual valarray<float> getFloats() const {
		valarray<float> a(data.size());
		for (size_t i = 0; i < data.size(); i++) {
			a[i] = boost::numeric_cast<float>(data[i]) * float(scaleFactor) + float(addOffset);
		}
		return a;
	}

	void* getUntypedData() const {
		return (void*) &data[0];
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
		return fillValue == data[at(i)];
	}

	void setFillValue(size_t i) throw (out_of_range) {
		data[at(i)] = fillValue;
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
		if (n * strideL > strideK) {
			BOOST_THROW_EXCEPTION(invalid_argument("n * strideL > strideK"));
		}
		if (strideK % strideL != 0) {
			BOOST_THROW_EXCEPTION(invalid_argument("strideK % strideL != 0"));
		}
		if (data.size() % strideK != 0) {
			BOOST_THROW_EXCEPTION( invalid_argument("data.size() % strideK != 0"));
		}
#pragma omp parallel for
		for (size_t k = 0; k < data.size(); k += strideK) {
			copy(&data[k + n * strideL], &data[k + strideK], &data[k]);
			fill(&data[k + strideK - n * strideL], &data[k + strideK], T(0));
		}
	}

	bool canReturnDataPointer() const {
		return true;
	}

protected:

	AbstractAccessor(size_t n, T fillValue = numeric_limits<T>::min(), double scaleFactor = 1.0, double addOffset = 0.0) :
			Accessor(), fillValue(fillValue), scaleFactor(scaleFactor), addOffset(addOffset), data(n) {
	}

	virtual ~AbstractAccessor() {
	}

	valarray<T>& getTypedData() const {
		return const_cast<valarray<T>&>(data);
	}

private:

	size_t at(size_t i) const {
		if (i < data.size()) {
			return i;
		}
		BOOST_THROW_EXCEPTION(out_of_range("Index i = " + boost::lexical_cast<string>(i) + " is out of range."));
	}

	const T fillValue;
	const double scaleFactor;
	const double addOffset;

	valarray<T> data;
};

#endif	/* ABSTRACTACCESSOR_H */
