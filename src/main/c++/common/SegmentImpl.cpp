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
#include "SegmentImpl.h"

SegmentImpl::SegmentImpl() : Segment() {

}

SegmentImpl::~SegmentImpl() {
}

void SegmentImpl::add(const std::string& var) {
    std::vector<int> values = std::vector<int>();
    values.push_back(0x0010);
//    dataBufferMap.insert(std::make_pair(var, values));
}

void SegmentImpl::remove(const std::string& varName) {

}

Pixel& SegmentImpl::getPixel(int k, int l, int m, Pixel& pixel) {
    return pixel;
}

PixelImpl SegmentImpl::createPixel(const std::string varName, int k, int l, int m) {
    PixelImpl pixel(*this, k, l, m);
    return pixel;
}

void SegmentImpl::getSamplesInt(const std::string& varName, std::vector<int>& samples) {
//    std::vector<int> values = (std::vector<int>)
//    dataBufferMap.at(varName);
//    samples.push_back( values.front() );
}

//void SegmentImpl::setSamplesInt(const std::string& varName, std::valarray<int>& samples) {
//}

