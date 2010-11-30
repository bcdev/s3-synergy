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
#include "StringUtils.h"

using std::make_pair;

SegmentImpl::SegmentImpl(const string& segmentId, 
        size_t minL, 
        size_t maxL, 
        size_t minK, 
        size_t maxK, 
        size_t minM, 
        size_t maxM) : id(segmentId) {
    this->minL = minL;
    this->maxL = maxL;
    this->minK = minK;
    this->maxK = maxK;
    this->minM = minM;
    this->maxM = maxM;
}

SegmentImpl::~SegmentImpl() {
}

void SegmentImpl::addIntVariable(const string& varName) {
    // TODO - allocate and initialize array
    int* values = 0;
    dataMap.insert(make_pair(&varName, values));
}

size_t SegmentImpl::computePosition(size_t k, size_t l, size_t m) const {
    return (m - minM) + ((l - minL) + (k - minK) * (maxL - minL + 1)) * (maxM - minM + 1);
}

const string& SegmentImpl::getId() const {
    return id;
}

size_t SegmentImpl::getMaxK() const {
    return maxK;
}

size_t SegmentImpl::getMinK() const {
    return minK;
}

size_t SegmentImpl::getMaxL() const {
    return maxL;
}

size_t SegmentImpl::getMinL() const {
    return minL;
}

size_t SegmentImpl::getMaxM() const {
    return maxM;
}

size_t SegmentImpl::getMinM() const {
    return minM;
}

string SegmentImpl::toString() const {
    string result;
    result.append( "id: " );
    result.append( id );
    result.append( ", " );
    result.append( "minL: " );
    result.append( StringUtils::intToString(minL) );
    result.append( ", " );
    result.append( "maxL: " );
    result.append( StringUtils::intToString(maxL ) );
    result.append( ", " );
    result.append( "minK: " );
    result.append( StringUtils::intToString(minK ) );
    result.append( ", " );
    result.append( "maxK: " );
    result.append( StringUtils::intToString(maxK ) );
    result.append( ", " );
    result.append( "minM: " );
    result.append( StringUtils::intToString(minM ) );
    result.append( ", " );
    result.append( "maxM: " );
    result.append( StringUtils::intToString(maxM ) );
    return result;
}

int SegmentImpl::getSampleInt(const string& varName, size_t position) const {
    // TODO - check type and get sample value
    return 0;
}

void SegmentImpl::setSampleInt(const string& varName, size_t position, int value) {
    // TODO - check type and set sample value
}

size_t SegmentImpl::getValueCount() const {
    return (maxL - minL + 1) * (maxK - minK + 1) * (maxM - minM + 1);
}

void SegmentImpl::setSamplesInt(const string& varName, int* values) {
    dataMap[&varName] = values;
}
void SegmentImpl::setMaxL(size_t maxL) {
    this->maxL = maxL;
}
void SegmentImpl::setMinL(size_t minL) {
    this->minL = minL;
}