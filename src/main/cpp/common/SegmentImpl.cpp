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

#include <iostream>
#include <typeinfo>

#include "SegmentImpl.h"

using std::make_pair;

SegmentImpl::SegmentImpl(size_t k, size_t l, size_t m) : K(k), L(l), M(m), valueCount(k * l * m) {
}

SegmentImpl::~SegmentImpl() {
}

size_t SegmentImpl::getK() {
    return K;
}

size_t SegmentImpl::getL() {
    return L;
}

size_t SegmentImpl::getM() {
    return M;
}

void SegmentImpl::addIntVariable(const string& var) {
    int values[valueCount];
    for( size_t i = 0; i < valueCount; i++ ) {
        // todo define better no data value
        values[i] = NO_DATA_VALUE;
    }
    dataBufferMap.insert(make_pair(var, values));
}

void SegmentImpl::remove(const string& varName) {
    dataBufferMap.erase(varName);
}

Pixel* SegmentImpl::getPixel(int k, int l, int m, Pixel* pixel) {
    int arrayPosition = computeArrayPosition(k, l, m);
    pixel = new PixelImpl(*this, k, l, m, arrayPosition);
    return pixel;
}

int SegmentImpl::getSampleInt(const string& varName, int position) {
    int* values = (int*) dataBufferMap.at(varName);
    return values[position];
}

void SegmentImpl::setSampleInt(const string& varName, int position, int value) {
    int* values = (int*) dataBufferMap.at(varName);
    values[position] = value;
    dataBufferMap.insert( make_pair(varName, values) );
}

void SegmentImpl::getSamplesInt(const string& varName, vector<int>& samples) {
    int* values = (int*)dataBufferMap.at(varName);
    for( size_t i = 0; i < valueCount; i++ ) {
        int value = values[i];
        samples.push_back( value );
    }
}

void SegmentImpl::setSamplesInt(const string& varName, vector<int>& samples) {
    int* values = (int*) dataBufferMap.at(varName);
    for( size_t i = 0; i < valueCount; i++ ) {
        if( i < samples.size() ) {
            values[i] = samples.at( i );
        }
    }
    dataBufferMap.insert(make_pair( varName, &samples ));
}

int SegmentImpl::computeArrayPosition(int k, int l, int m) {
    return m + l * M + k * L * M;
}