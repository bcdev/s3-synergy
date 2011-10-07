/*
 * Pixel.cpp
 *
 *  Created on: 07.10.2011
 *      Author: thomasstorm
 */

#include <algorithm>

#include "Pixel.h"

using std::copy;

Pixel::Pixel(Segment& segment, long k, long l, long m) :
        k(k), l(l), m(m), index(segment.getGrid().getIndex(k, l, m)), segment(segment) {
}

Pixel::Pixel(const Pixel& p) :
        k(p.k), l(p.l), m(p.m), index(p.segment.getGrid().getIndex(p.k, p.l, p.m)), segment(p.segment) {
}

int8_t Pixel::getByte(const string& varName) const throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).getByte(index);
}

uint8_t Pixel::getUByte(const string& varName) const throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).getUByte(index);
}

int16_t Pixel::getShort(const string& varName) const throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).getShort(index);
}

uint16_t Pixel::getUShort(const string& varName) const throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).getUShort(index);
}

int32_t Pixel::getInt(const string& varName) const throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).getInt(index);
}

uint32_t Pixel::getUInt(const string& varName) const throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).getUInt(index);
}

int64_t Pixel::getLong(const string& varName) const throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).getLong(index);
}

uint64_t Pixel::getULong(const string& varName) const throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).getULong(index);
}

float Pixel::getFloat(const string& varName) const throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).getFloat(index);
}

double Pixel::getDouble(const string& varName) const throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).getDouble(index);
}

void Pixel::setByte(const string& varName, int8_t value) throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).setByte(index, value);
}

void Pixel::setUByte(const string& varName, uint8_t value) throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).setUByte(index, value);
}

void Pixel::setShort(const string& varName, int16_t value) throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).setShort(index, value);
}

void Pixel::setUShort(const string& varName, uint16_t value) throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).setUShort(index, value);
}

void Pixel::setInt(const string& varName, int32_t value) throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).setInt(index, value);
}

void Pixel::setUInt(const string& varName, uint32_t value) throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).setUInt(index, value);
}

void Pixel::setLong(const string& varName, int64_t value) throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).setLong(index, value);
}

void Pixel::setULong(const string& varName, uint64_t value) throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).setULong(index, value);
}

void Pixel::setFloat(const string& varName, float value) throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).setFloat(index, value);
}

void Pixel::setDouble(const string& varName, double value) throw (bad_cast, out_of_range) {
    return segment.getAccessor(varName).setDouble(index, value);
}

