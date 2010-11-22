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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * File:   SegmentImpl.cpp
 * Author: thomass
 * 
 * Created on November 11, 2010, 10:04 AM
 */

#include "SegmentImpl.h"
#include "Reader.h"

using std::make_pair;

SegmentImpl::SegmentImpl(size_t k, size_t l, size_t m)
: maxK(k), maxL(l), maxM(m), valueCount(k * l * m) {
}

SegmentImpl::~SegmentImpl() {
}

size_t SegmentImpl::getMaxK() {
    return maxK;
}

size_t SegmentImpl::getMinK() {
    return 0;
}

size_t SegmentImpl::getMaxL() {
    return maxL;
}

size_t SegmentImpl::getMinL() {
    return 0;
}

size_t SegmentImpl::getMaxM() {
    return maxM;
}

size_t SegmentImpl::getMinM() {
    return 0;
}

void SegmentImpl::addIntVariable(const string& var) {
    // todo - won't work
    int values[valueCount];
    for (size_t i = 0; i < valueCount; i++) {
        // todo define better no data value
        values[i] = 0;
    }
    dataMap.insert(make_pair(var, values));
}

int SegmentImpl::getSampleInt(const string& varName, int position) {
    int* values = (int*) dataMap.at(varName);
    return values[position];
}

void SegmentImpl::setSampleInt(const string& varName, int position, int value) {
    int* values = (int*) dataMap.at(varName);
    values[position] = value;
    dataMap.insert(make_pair(varName, values));
}

int SegmentImpl::computeArrayPosition(int k, int l, int m) {
    return m + l * maxM + k * maxL * maxM;
}
