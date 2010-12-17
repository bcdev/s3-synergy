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

#include <cstdint>
#include <stdexcept>
#include <valarray>

#include "Grid.h"

using std::out_of_range;
using std::valarray;

/**
 * Represents an accessor to the samples of a variable. An accessor provides
 * accessor and mutator methods for each value type. 
 * 
 * The samples of a variable are addressed by the indexes {@code k}, {@code l}
 * and {@code m}, which denote the OLCI camera, row, and column indexes,
 * respectively.
 */
class Accessor {
public:

    /**
     * Destructor.
     */
    virtual ~Accessor() {
    }

    /**
     * Returns the value of the sample at grid coordinates (k, l, m).
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @return the value of the sample at grid coordinates (k, l, m).
     */
    virtual int8_t getByte(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) = 0;

    /**
     * Returns the value of the sample at grid coordinates (k, l, m).
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @return the value of the sample at grid coordinates (k, l, m).
     */
    virtual uint8_t getUByte(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) = 0;

    /**
     * Returns the value of the sample at grid coordinates (k, l, m).
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @return the value of the sample at grid coordinates (k, l, m).
     */
    virtual int16_t getShort(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) = 0;

    /**
     * Returns the value of the sample at grid coordinates (k, l, m).
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @return the value of the sample at grid coordinates (k, l, m).
     */
    virtual uint16_t getUShort(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) = 0;

    /**
     * Returns the value of the sample at grid coordinates (k, l, m).
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @return the value of the sample at grid coordinates (k, l, m).
     */
    virtual int32_t getInt(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) = 0;

    /**
     * Returns the value of the sample at grid coordinates (k, l, m).
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @return the value of the sample at grid coordinates (k, l, m).
     */
    virtual uint32_t getUInt(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) = 0;

    /**
     * Returns the value of the sample at grid coordinates (k, l, m).
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @return the value of the sample at grid coordinates (k, l, m).
     */
    virtual int64_t getLong(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) = 0;

    /**
     * Returns the value of the sample at grid coordinates (k, l, m).
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @return the value of the sample at grid coordinates (k, l, m).
     */
    virtual uint64_t getULong(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) = 0;

    /**
     * Returns the value of the sample at grid coordinates (k, l, m).
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @return the value of the sample at grid coordinates (k, l, m).
     */
    virtual float getFloat(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) = 0;

    /**
     * Returns the value of the sample at grid coordinates (k, l, m).
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @return the value of the sample at grid coordinates (k, l, m).
     */
    virtual double getDouble(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) = 0;

    /**
     * Sets the value of the sample at grid coordinates (k, l, m) to a given
     * value.
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @param value The value.
     */
    virtual void setByte(uint8_t k, uint16_t l, uint16_t m, int8_t value) throw (out_of_range) = 0;

    /**
     * Sets the value of the sample at grid coordinates (k, l, m) to a given
     * value.
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @param value The value.
     */
    virtual void setUByte(uint8_t k, uint16_t l, uint16_t m, uint8_t value) throw (out_of_range) = 0;

    /**
     * Sets the value of the sample at grid coordinates (k, l, m) to a given
     * value.
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @param value The value.
     */
    virtual void setShort(uint8_t k, uint16_t l, uint16_t m, int16_t value) throw (out_of_range) = 0;

    /**
     * Sets the value of the sample at grid coordinates (k, l, m) to a given
     * value.
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @param value The value.
     */
    virtual void setUShort(uint8_t k, uint16_t l, uint16_t m, uint16_t value) throw (out_of_range) = 0;

    /**
     * Sets the value of the sample at grid coordinates (k, l, m) to a given
     * value.
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @param value The value.
     */
    virtual void setInt(uint8_t k, uint16_t l, uint16_t m, int32_t value) throw (out_of_range) = 0;

    /**
     * Sets the value of the sample at grid coordinates (k, l, m) to a given
     * value.
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @param value The value.
     */
    virtual void setUInt(uint8_t k, uint16_t l, uint16_t m, uint32_t value) throw (out_of_range) = 0;

    /**
     * Sets the value of the sample at grid coordinates (k, l, m) to a given
     * value.
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @param value The value.
     */
    virtual void setLong(uint8_t k, uint16_t l, uint16_t m, int64_t value) throw (out_of_range) = 0;

    /**
     * Sets the value of the sample at grid coordinates (k, l, m) to a given
     * value.
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @param value The value.
     */
    virtual void setULong(uint8_t k, uint16_t l, uint16_t m, uint64_t value) throw (out_of_range) = 0;

    /**
     * Sets the value of the sample at grid coordinates (k, l, m) to a given
     * value.
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @param value The value.
     */
    virtual void setFloat(uint8_t k, uint16_t l, uint16_t m, float value) throw (out_of_range) = 0;

    /**
     * Sets the value of the sample at grid coordinates (k, l, m) to a given
     * value.
     * @param k The camera index.
     * @param l The line index.
     * @param m The column index.
     * @param value The value.
     */
    virtual void setDouble(uint8_t k, uint16_t l, uint16_t m, double value) throw (out_of_range) = 0;

    /**
     * Returns the container of samples values. If the actual container is not
     * of of type {@code int8_t} a {@code bad_cast} exception is thrown.
     * @return the container of sample values.
     */
    virtual valarray<int8_t>& getByteData() const = 0;

    /**
     * Returns the container of samples values. If the actual container is not
     * of of type {@code uint8_t} a {@code bad_cast} exception is thrown.
     * @return the container of sample values.
     */
    virtual valarray<uint8_t>& getUByteData() const = 0;

    /**
     * Returns the container of samples values. If the actual container is not
     * of of type {@code int16_t} a {@code bad_cast} exception is thrown.
     * @return the container of sample values.
     */
    virtual valarray<int16_t>& getShortData() const = 0;

    /**
     * Returns the container of samples values. If the actual container is not
     * of of type {@code uint16_t} a {@code bad_cast} exception is thrown.
     * @return the container of sample values.
     */
    virtual valarray<uint16_t>& getUShortData() const = 0;

    /**
     * Returns the container of samples values. If the actual container is not
     * of of type {@code int32_t} a {@code bad_cast} exception is thrown.
     * @return the container of sample values.
     */
    virtual valarray<int32_t>& getIntData() const = 0;

    /**
     * Returns the container of samples values. If the actual container is not
     * of of type {@code uint32_t} a {@code bad_cast} exception is thrown.
     * @return the container of sample values.
     */
    virtual valarray<uint32_t>& getUIntData() const = 0;

    /**
     * Returns the container of samples values. If the actual container is not
     * of of type {@code int64_t} a {@code bad_cast} exception is thrown.
     * @return the container of sample values.
     */
    virtual valarray<int64_t>& getLongData() const = 0;

    /**
     * Returns the container of samples values. If the actual container is not
     * of of type {@code uint64_t} a {@code bad_cast} exception is thrown.
     * @return the container of sample values.
     */
    virtual valarray<uint64_t>& getULongData() const = 0;

    /**
     * Returns the container of samples values. If the actual container is not
     * of of type {@code float} a {@code bad_cast} exception is thrown.
     * @return the container of sample values.
     */
    virtual valarray<float>& getFloatData() const = 0;

    /**
     * Returns the container of samples values. If the actual container is not
     * of of type {@code double} a {@code bad_cast} exception is thrown.
     * @return the container of sample values.
     */
    virtual valarray<double>& getDoubleData() const = 0;

    /**
     * Returns the grid of the parent segment.
     * @return the grid of the parent segment.
     */
    virtual const Grid& getGrid() const = 0;
};

#endif	/* ACCESSOR_H */
