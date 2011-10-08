/*
 * Pixel.h
 *
 *  Created on: 07.10.2011
 *      Author: thomasstorm
 */

#ifndef PIXEL_H_
#define PIXEL_H_

#include <map>
#include <string>
#include <valarray>

#include "../core/Boost.h"
#include "../core/Segment.h"

using std::map;
using std::string;
using std::valarray;

class Pixel {

public:

    Pixel(Segment& segment, long k, long l, long m);

    Pixel(const Pixel& p);

    bool operator<(const Pixel& other) const {
        return segment.getGrid().getIndex(k, l, m) < other.segment.getGrid().getIndex(k, l, m);
    }

    const long k;
    const long l;
    const long m;

    const long index;

protected:
    /**
     * Returns the value of the sample.
     * @return the value of the sample.
     */
    int8_t getByte(const string& varName) const throw (bad_cast, out_of_range);

    /**
     * Returns the value of the sample.
     * @return the value of the sample.
     */
    uint8_t getUByte(const string& varName) const throw (bad_cast, out_of_range);

    /**
     * Returns the value of the sample.
     * @return the value of the sample.
     */
    int16_t getShort(const string& varName) const throw (bad_cast, out_of_range);

    /**
     * Returns the value of the sample.
     * @return the value of the sample.
     */
    uint16_t getUShort(const string& varName) const throw (bad_cast, out_of_range);

    /**
     * Returns the value of the sample.
     * @return the value of the sample.
     */
    int32_t getInt(const string& varName) const throw (bad_cast, out_of_range);

    /**
     * Returns the value of the sample.
     * @return the value of the sample.
     */
    uint32_t getUInt(const string& varName) const throw (bad_cast, out_of_range);

    /**
     * Returns the value of the sample.
     * @return the value of the sample.
     */
    int64_t getLong(const string& varName) const throw (bad_cast, out_of_range);

    /**
     * Returns the value of the sample.
     * @return the value of the sample.
     */
    uint64_t getULong(const string& varName) const throw (bad_cast, bad_cast, out_of_range);

    /**
     * Returns the value of the sample.
     * @return the value of the sample.
     */
    float getFloat(const string& varName) const throw (bad_cast, out_of_range);

    /**
     * Returns the value of the sample.
     * @param i The index.
     * @return the value of the sample.
     */
    double getDouble(const string& varName) const throw (bad_cast, out_of_range);

    /**
     * Sets the value of the sample to a given value.
     * @param value The value.
     */
    void setByte(const string& varName, int8_t value) throw (bad_cast, out_of_range);

    /**
     * Sets the value of the sample to a given value.
     * @param value The value.
     */
    void setUByte(const string& varName, uint8_t value) throw (bad_cast, out_of_range);

    /**
     * Sets the value of the sample to a given value.
     * @param value The value.
     */
    void setShort(const string& varName, int16_t value) throw (bad_cast, out_of_range);

    /**
     * Sets the value of the sample to a given value.
     * @param value The value.
     */
    void setUShort(const string& varName, uint16_t value) throw (bad_cast, out_of_range);

    /**
     * Sets the value of the sample to a given value.
     * @param value The value.
     */
    void setInt(const string& varName, int32_t value) throw (bad_cast, out_of_range);

    /**
     * Sets the value of the sample to a given value.
     * @param value The value.
     */
    void setUInt(const string& varName, uint32_t value) throw (bad_cast, out_of_range);

    /**
     * Sets the value of the sample to a given value.
     * @param value The value.
     */
    void setLong(const string& varName, int64_t value) throw (bad_cast, out_of_range);

    /**
     * Sets the value of the sample to a given value.
     * @param value The value.
     */
    void setULong(const string& varName, uint64_t value) throw (bad_cast, out_of_range);

    /**
     * Sets the value of the sample to a given value.
     * @param i The index.
     * @param value The value.
     */
    void setFloat(const string& varName, float value) throw (bad_cast, out_of_range);

    /**
     * Sets the value of the sample to a given value.
     * @param value The value.
     */
    void setDouble(const string& varName, double value) throw (bad_cast, out_of_range);

private:
    Segment& segment;
};

#endif /* PIXEL_H_ */
