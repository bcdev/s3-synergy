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

#include <typeinfo>

#include "Accessor.h"
#include "GridImpl.h"

using std::bad_cast;

template<class T>
class AbstractAccessor : public virtual Accessor {
public:

    int8_t getByte(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) {
        return data[index(k, l, m)];
    }

    void setByte(uint8_t k, uint16_t l, uint16_t m, int8_t value) throw (out_of_range) {
        data[index(k, l, m)] = value;
    }

    double getDouble(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) {
        return data[index(k, l, m)];
    }

    void setDouble(uint8_t k, uint16_t l, uint16_t m, double value) throw (out_of_range) {
        data[index(k, l, m)] = value;
    }

    float getFloat(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) {
        return data[index(k, l, m)];
    }

    void setFloat(uint8_t k, uint16_t l, uint16_t m, float value) throw (out_of_range) {
        data[index(k, l, m)] = value;
    }

    int32_t getInt(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) {
        return data[index(k, l, m)];
    }

    void setInt(uint8_t k, uint16_t l, uint16_t m, int32_t value) throw (out_of_range) {
        data[index(k, l, m)] = value;
    }

    int64_t getLong(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) {
        return data[index(k, l, m)];
    }

    void setLong(uint8_t k, uint16_t l, uint16_t m, int64_t value) throw (out_of_range) {
        data[index(k, l, m)] = value;
    }

    int16_t getShort(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) {
        return data[index(k, l, m)];
    }

    void setShort(uint8_t k, uint16_t l, uint16_t m, int16_t value) throw (out_of_range) {
        data[index(k, l, m)] = value;
    }

    uint8_t getUByte(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) {
        return data[index(k, l, m)];
    }

    void setUByte(uint8_t k, uint16_t l, uint16_t m, uint8_t value) throw (out_of_range) {
        data[index(k, l, m)] = value;
    }

    uint32_t getUInt(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) {
        return data[index(k, l, m)];
    }

    void setUInt(uint8_t k, uint16_t l, uint16_t m, uint32_t value) throw (out_of_range) {
        data[index(k, l, m)] = value;
    }

    uint64_t getULong(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) {
        return data[index(k, l, m)];
    }

    void setULong(uint8_t k, uint16_t l, uint16_t m, uint64_t value) throw (out_of_range) {
        data[index(k, l, m)] = value;
    }

    uint16_t getUShort(uint8_t k, uint16_t l, uint16_t m) const throw (out_of_range) {
        return data[index(k, l, m)];
    }

    void setUShort(uint8_t k, uint16_t l, uint16_t m, uint16_t value) throw (out_of_range) {
        data[index(k, l, m)] = value;
    }

    virtual valarray<int8_t>& getByteData() const {
        throw bad_cast();
    }

    virtual valarray<double>& getDoubleData() const {
        throw bad_cast();
    }

    virtual valarray<float>& getFloatData() const {
        throw bad_cast();
    }

    virtual valarray<int32_t>& getIntData() const {
        throw bad_cast();
    }

    virtual valarray<int64_t>& getLongData() const {
        throw bad_cast();
    }

    virtual valarray<int16_t>& getShortData() const {
        throw bad_cast();
    }

    virtual valarray<uint8_t>& getUByteData() const {
        throw bad_cast();
    }

    virtual valarray<uint32_t>& getUIntData() const {
        throw bad_cast();
    }

    virtual valarray<uint64_t>& getULongData() const {
        throw bad_cast();
    }

    virtual valarray<uint16_t>& getUShortData() const {
        throw bad_cast();
    }

    const Grid& getGrid() const {
        return grid;
    }

protected:

    AbstractAccessor(const Grid& g) : Accessor(), grid(g), data(g.getSizeK() * g.getSizeL() * g.getSizeM()) {
    }

    virtual ~AbstractAccessor() {
    }

    valarray<T>& getData() const {
        return const_cast<valarray<T>&> (data);
    }

private:

    size_t index(uint8_t k, uint16_t l, uint16_t m) const {
        const size_t minK = grid.getK();
        const size_t minL = grid.getL();
        const size_t minM = grid.getM();
        const size_t sizeK = grid.getSizeK();
        const size_t sizeL = grid.getSizeL();
        const size_t sizeM = grid.getSizeM();

        if (k < minK || k > minK + sizeK - 1) {
            throw out_of_range("camera index is out of range.");
        }
        if (l < minL || l > minL + sizeL - 1) {
            throw out_of_range("row index is out of range.");
        }
        if (m < minM || m > minM + sizeM - 1) {
            throw out_of_range("column index is out of range.");
        }
        return m * grid.getStrideM() + l * grid.getStrideL() + k * grid.getStrideK();
    }

    const GridImpl& grid;
    valarray<T> data;
};

#endif	/* ABSTRACTACCESSOR_H */
