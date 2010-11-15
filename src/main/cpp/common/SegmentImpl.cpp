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

#include <vector>
#include <iostream>
#include <typeinfo>
#include "SegmentImpl.h"

//SegmentImpl::SegmentImpl() : Segment() {
//
//}

SegmentImpl::~SegmentImpl() {
}

void SegmentImpl::addIntVariable(const std::string& var) {
    int values[numValues];
    for( int i = 0; i < numValues; i++ ) {
        // todo define better no data value
        values[i] = NO_DATA_VALUE;
    }
    dataBufferMap.insert(std::make_pair(var, values));
}

void SegmentImpl::remove(const std::string& varName) {
    dataBufferMap.erase(varName);
}

Pixel* SegmentImpl::getPixel(int k, int l, int m, Pixel* pixel) {
    int arrayPosition = computeArrayPosition(k, l, m);
    pixel = new PixelImpl(*this, k, l, m, arrayPosition);
    return pixel;
}

int SegmentImpl::getSampleInt(const std::string& varName, int position) {
    int* values = (int*) dataBufferMap.at(varName);
    return values[position];
}

void SegmentImpl::setSampleInt(const std::string& varName, int position, int value) {
    int* values = (int*) dataBufferMap.at(varName);
    values[position] = value;
    dataBufferMap.insert( std::make_pair(varName, values) );
}

void SegmentImpl::getSamplesInt(const std::string& varName, std::vector<int>& samples) {
    int* values = (int*)dataBufferMap.at(varName);
    for( int i = 0; i < numValues; i++ ) {
        int value = values[i];
        samples.push_back( value );
    }
}

void SegmentImpl::setSamplesInt(const std::string& varName, std::vector<int>& samples) {
    int* values = (int*) dataBufferMap.at(varName);
    for( int i = 0; i < numValues; i++ ) {
        if( i < samples.size() ) {
            values[i] = samples.at( i );
        }
    }
    dataBufferMap.insert(std::make_pair( varName, &samples ));
}

int SegmentImpl::computeArrayPosition(int k, int l, int m) {
    // todo use something similar to:
    // int position = k * l * WIDTH_OF_INPUT_PRODUCT + m;
    return k * l * m;
}