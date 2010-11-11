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
 */

#include <vector>
#include <string>
#include "PixelImpl.h"

double PixelImpl::getDouble(const std::string& name) const {
//    const std::valarray<double> samples = std::valarray<double>();
//    segment.getSamplesDouble(name, samples );
//    return samples.operator [](position);
}

bool PixelImpl::isRaised(const std::string& name, int flagMask) const {
    std::vector<int> samples = std::vector<int>();
    segment.getSamplesInt(name, samples);
    int mask = samples.at(position);
    return (mask & flagMask ) == flagMask;
}

void PixelImpl::setDouble(const std::string& name, double value) {

}

void PixelImpl::raise(const std::string& name, int flagMask) {

}

void PixelImpl::clear(const std::string& name, int flagMask) {
    
}

Segment& PixelImpl::getSegment() const {
    return segment;
}

int PixelImpl::computePosition() {
    //    int position = l * WIDTH_OF_INPUT_PRODUCT + m;
    return l * m;
}