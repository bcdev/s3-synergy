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
 * File:   Accessor.h
 * Author: ralf
 *
 * Created on December 15, 2010, 4:01 PM
 */

#ifndef ACCESSOR_H
#define	ACCESSOR_H

#include <stdexcept>
#include <typeinfo>
#include <valarray>

#include <boost/cstdint.hpp>

using std::bad_cast;
using std::out_of_range;
using std::valarray;

using boost::int8_t;
using boost::int16_t;
using boost::int32_t;
using boost::int64_t;
using boost::uint8_t;
using boost::uint16_t;
using boost::uint32_t;
using boost::uint64_t;

/**
 * Represents an accessor to the samples of a variable. An accessor provides
 * accessor and mutator methods for each value type. 
 */
class Accessor {
public:

	/**
	 * Destructor.
	 */
	virtual ~Accessor() {
	}

	/**
	 * Returns the value of the sample at a given index {@code i}.
	 * @param i The index.
	 * @return the value of the sample at {@code i}.
	 */
	virtual int8_t getByte(size_t i) const throw (bad_cast, out_of_range) = 0;

	/**
	 * Returns the value of the sample at a given index {@code i}.
	 * @param i The index.
	 * @return the value of the sample at {@code i}.
	 */
	virtual uint8_t getUByte(size_t i) const throw (bad_cast, out_of_range) = 0;

	/**
	 * Returns the value of the sample at a given index {@code i}.
	 * @param i The index.
	 * @return the value of the sample at {@code i}.
	 */
	virtual int16_t getShort(size_t i) const throw (bad_cast, out_of_range) = 0;

	/**
	 * Returns the value of the sample at a given index {@code i}.
	 * @param i The index.
	 * @return the value of the sample at {@code i}.
	 */
	virtual uint16_t getUShort(size_t i) const throw (bad_cast, out_of_range) = 0;

	/**
	 * Returns the value of the sample at a given index {@code i}.
	 * @param i The index.
	 * @return the value of the sample at {@code i}.
	 */
	virtual int32_t getInt(size_t i) const throw (bad_cast, out_of_range) = 0;

	/**
	 * Returns the value of the sample at a given index {@code i}.
	 * @param i The index.
	 * @return the value of the sample at {@code i}.
	 */
	virtual uint32_t getUInt(size_t i) const throw (bad_cast, out_of_range) = 0;

	/**
	 * Returns the value of the sample at a given index {@code i}.
	 * @param i The index.
	 * @return the value of the sample at {@code i}.
	 */
	virtual int64_t getLong(size_t i) const throw (bad_cast, out_of_range) = 0;

	/**
	 * Returns the value of the sample at a given index {@code i}.
	 * @param i The index.
	 * @return the value of the sample at {@code i}.
	 */
	virtual uint64_t getULong(size_t i) const throw (bad_cast, bad_cast, out_of_range) = 0;

	/**
	 * Returns the value of the sample at a given index {@code i}.
	 * @param i The index.
	 * @return the value of the sample at {@code i}.
	 */
	virtual float getFloat(size_t i) const throw (bad_cast, out_of_range) = 0;

	/**
	 * Returns the value of the sample at a given index {@code i}.
	 * @param i The index.
	 * @return the value of the sample at {@code i}.
	 */
	virtual double getDouble(size_t i) const throw (bad_cast, out_of_range) = 0;

	/**
	 * Sets the value of the sample at index {@code i} to a given value.
	 * @param i The index.
	 * @param value The value.
	 */
	virtual void setByte(size_t i, int8_t value) throw (bad_cast, out_of_range) = 0;

	/**
	 * Sets the value of the sample at index {@code i} to a given value.
	 * @param i The index.
	 * @param value The value.
	 */
	virtual void setUByte(size_t i, uint8_t value) throw (bad_cast, out_of_range) = 0;

	/**
	 * Sets the value of the sample at index {@code i} to a given value.
	 * @param i The index.
	 * @param value The value.
	 */
	virtual void setShort(size_t i, int16_t value) throw (bad_cast, out_of_range) = 0;

	/**
	 * Sets the value of the sample at index {@code i} to a given value.
	 * @param i The index.
	 * @param value The value.
	 */
	virtual void setUShort(size_t i, uint16_t value) throw (bad_cast, out_of_range) = 0;

	/**
	 * Sets the value of the sample at index {@code i} to a given value.
	 * @param i The index.
	 * @param value The value.
	 */
	virtual void setInt(size_t i, int32_t value) throw (bad_cast, out_of_range) = 0;

	/**
	 * Sets the value of the sample at index {@code i} to a given value.
	 * @param i The index.
	 * @param value The value.
	 */
	virtual void setUInt(size_t i, uint32_t value) throw (bad_cast, out_of_range) = 0;

	/**
	 * Sets the value of the sample at index {@code i} to a given value.
	 * @param i The index.
	 * @param value The value.
	 */
	virtual void setLong(size_t i, int64_t value) throw (bad_cast, out_of_range) = 0;

	/**
	 * Sets the value of the sample at index {@code i} to a given value.
	 * @param i The index.
	 * @param value The value.
	 */
	virtual void setULong(size_t i, uint64_t value) throw (bad_cast, out_of_range) = 0;

	/**
	 * Sets the value of the sample at index {@code i} to a given value.
	 * @param i The index.
	 * @param value The value.
	 */
	virtual void setFloat(size_t i, float value) throw (bad_cast, out_of_range) = 0;

	/**
	 * Sets the value of the sample at index {@code i} to a given value.
	 * @param i The index.
	 * @param value The value.
	 */
	virtual void setDouble(size_t i, double value) throw (bad_cast, out_of_range) = 0;

	/**
	 * Returns the number of sample values.
	 * @return the grid of the parent segment.
	 */
	virtual const size_t getSampleCount() const = 0;

	/**
	 * Returns the container of samples values. If the samples are not
	 * of type {@code int8_t} a {@code bad_cast} exception is thrown.
	 * @return the container of sample values.
	 */
	virtual valarray<int8_t>& getByteData() const throw (bad_cast) = 0;

	/**
	 * Returns the container of samples values. If the samples are not
	 * of type {@code uint8_t} a {@code bad_cast} exception is thrown.
	 * @return the container of sample values.
	 */
	virtual valarray<uint8_t>& getUByteData() const throw (bad_cast) = 0;

	/**
	 * Returns the container of samples values. If the samples are not
	 * of type {@code int16_t} a {@code bad_cast} exception is thrown.
	 * @return the container of sample values.
	 */
	virtual valarray<int16_t>& getShortData() const throw (bad_cast) = 0;

	/**
	 * Returns the container of samples values. If the samples are not
	 * of type {@code uint16_t} a {@code bad_cast} exception is thrown.
	 * @return the container of sample values.
	 */
	virtual valarray<uint16_t>& getUShortData() const throw (bad_cast) = 0;

	/**
	 * Returns the container of samples values. If the samples are not
	 * of type {@code int32_t} a {@code bad_cast} exception is thrown.
	 * @return the container of sample values.
	 */
	virtual valarray<int32_t>& getIntData() const throw (bad_cast) = 0;

	/**
	 * Returns the container of samples values. If the samples are not
	 * of type {@code uint32_t} a {@code bad_cast} exception is thrown.
	 * @return the container of sample values.
	 */
	virtual valarray<uint32_t>& getUIntData() const throw (bad_cast) = 0;

	/**
	 * Returns the container of samples values. If the samples are not
	 * of type {@code int64_t} a {@code bad_cast} exception is thrown.
	 * @return the container of sample values.
	 */
	virtual valarray<int64_t>& getLongData() const throw (bad_cast) = 0;

	/**
	 * Returns the container of samples values. If the samples are not
	 * of type {@code uint64_t} a {@code bad_cast} exception is thrown.
	 * @return the container of sample values.
	 */
	virtual valarray<uint64_t>& getULongData() const throw (bad_cast) = 0;

	/**
	 * Returns the container of samples values. If the samples are not
	 * of type {@code float} a {@code bad_cast} exception is thrown.
	 * @return the container of sample values.
	 */
	virtual valarray<float>& getFloatData() const throw (bad_cast) = 0;

	/**
	 * Returns the container of samples values. If the samples are not
	 * of type {@code double} a {@code bad_cast} exception is thrown.
	 * @return the container of sample values.
	 */
	virtual valarray<double>& getDoubleData() const throw (bad_cast) = 0;

	virtual valarray<double> getDoubles() const = 0;

	virtual valarray<float> getFloats() const = 0;

	/**
	 * Returns an untyped pointer to the container of sample values.
	 * @return an untyped pointer to the container of sample values.
	 */
	virtual void* getUntypedData() const = 0;

	virtual double getScaleFactor() const = 0;

	virtual double getAddOffset() const = 0;

	virtual int getType() const = 0;

	virtual bool isFillValue(size_t i) const throw (out_of_range) = 0;

	virtual void setFillValue(size_t i) throw (out_of_range) = 0;

	virtual void shift(long n, long strideK, long strideL) = 0;
};

#endif	/* ACCESSOR_H */
