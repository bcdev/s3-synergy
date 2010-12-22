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
#include <typeinfo>

#include "Accessor.h"

using std::bad_cast;

template<class T>
class AbstractAccessor : public virtual Accessor {
public:

    int8_t getByte(size_t i) const throw (out_of_range) {
        return data[at(i)];
    }

    void setByte(size_t i, int8_t value) throw (out_of_range) {
        data[at(i)] = value;
    }

    double getDouble(size_t i) const throw (out_of_range) {
        return data[at(i)];
    }

    void setDouble(size_t i, double value) throw (out_of_range) {
        data[at(i)] = value;
    }

    float getFloat(size_t i) const throw (out_of_range) {
        return data[at(i)];
    }

    void setFloat(size_t i, float value) throw (out_of_range) {
        data[at(i)] = value;
    }

    int32_t getInt(size_t i) const throw (out_of_range) {
        return data[at(i)];
    }

    void setInt(size_t i, int32_t value) throw (out_of_range) {
        data[at(i)] = value;
    }

    int64_t getLong(size_t i) const throw (out_of_range) {
        return data[at(i)];
    }

    void setLong(size_t i, int64_t value) throw (out_of_range) {
        data[at(i)] = value;
    }

    int16_t getShort(size_t i) const throw (out_of_range) {
        return data[at(i)];
    }

    void setShort(size_t i, int16_t value) throw (out_of_range) {
        data[at(i)] = value;
    }

    uint8_t getUByte(size_t i) const throw (out_of_range) {
        return data[at(i)];
    }

    void setUByte(size_t i, uint8_t value) throw (out_of_range) {
        data[at(i)] = value;
    }

    uint32_t getUInt(size_t i) const throw (out_of_range) {
        return data[at(i)];
    }

    void setUInt(size_t i, uint32_t value) throw (out_of_range) {
        data[at(i)] = value;
    }

    uint64_t getULong(size_t i) const throw (out_of_range) {
        return data[at(i)];
    }

    void setULong(size_t i, uint64_t value) throw (out_of_range) {
        data[at(i)] = value;
    }

    uint16_t getUShort(size_t i) const throw (out_of_range) {
        return data[at(i)];
    }

    void setUShort(size_t i, uint16_t value) throw (out_of_range) {
        data[at(i)] = value;
    }

    const size_t getSampleCount() const {
        return data.size();
    }

    virtual valarray<int8_t>& getByteData() const throw (bad_cast) {
        throw bad_cast();
    }

    virtual valarray<double>& getDoubleData() const throw (bad_cast) {
        throw bad_cast();
    }

    virtual valarray<float>& getFloatData() const throw (bad_cast) {
        throw bad_cast();
    }

    virtual valarray<int32_t>& getIntData() const throw (bad_cast) {
        throw bad_cast();
    }

    virtual valarray<int64_t>& getLongData() const throw (bad_cast) {
        throw bad_cast();
    }

    virtual valarray<int16_t>& getShortData() const throw (bad_cast) {
        throw bad_cast();
    }

    virtual valarray<uint8_t>& getUByteData() const throw (bad_cast) {
        throw bad_cast();
    }

    virtual valarray<uint32_t>& getUIntData() const throw (bad_cast) {
        throw bad_cast();
    }

    virtual valarray<uint64_t>& getULongData() const throw (bad_cast) {
        throw bad_cast();
    }

    virtual valarray<uint16_t>& getUShortData() const throw (bad_cast) {
        throw bad_cast();
    }

    void shift(size_t n, size_t strideK, size_t strideL) {
        for (size_t i = 0; i < data.size(); i += strideK) {
            std::copy(&data[i + n * strideL], &data[i + strideK], &data[i]);
            std::fill(&data[i + strideK - n * strideL], &data[i + strideK], T(0));
        }
    }

protected:

    AbstractAccessor(size_t n) : Accessor(), data(n) {
    }

    virtual ~AbstractAccessor() {
    }

    valarray<T>& getData() const {
        return const_cast<valarray<T>&> (data);
    }

private:

    size_t at(size_t i) const {
        if (i < data.size()) {
            return i;
        }
        throw out_of_range("index i is out of range.");
    }

    valarray<T> data;
};

#endif	/* ABSTRACTACCESSOR_H */
