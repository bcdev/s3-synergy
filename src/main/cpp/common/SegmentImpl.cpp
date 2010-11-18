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
: K(k), L(l), M(m), valueCount(k * l * m) {
    previous = next = 0;
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
    for (size_t i = 0; i < valueCount; i++) {
        // todo define better no data value
        values[i] = NO_DATA_VALUE;
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
    return m + l * M + k * L * M;
}

Segment* SegmentImpl::getPrevious() {
    return previous;
}

void SegmentImpl::setPrevious(Segment* previous) {
    this->previous = previous;
}

Segment* SegmentImpl::getNext() {
    if (next == 0) {
        next = reader->readNextSegment();
    }

    return next;
}

void SegmentImpl::setNext(Segment* next) {
    this->next = next;
}