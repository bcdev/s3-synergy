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

#include <string>

#include "PixelImpl.h"

using std::string;

bool PixelImpl::isFlagRaised(const string& name, int flagMask) const {
    int mask = segment.getSampleInt(name, position);
    return (mask & flagMask ) == flagMask;
}

void PixelImpl::raiseFlag(const string& name, int flagMask) {
    int mask = segment.getSampleInt(name, position);
    mask = mask | flagMask;
    segment.setSampleInt(name, position, mask);
}

void PixelImpl::clearFlag(const string& name, int flagMask) {
    
}

Segment& PixelImpl::getSegment() const {
    return segment;
}