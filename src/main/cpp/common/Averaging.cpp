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
 * File:   Averaging.cpp
 * Author: thomass
 * 
 * Created on November 18, 2010, 5:17 PM
 */

#include "Averaging.h"

Averaging::Averaging() {
}

Averaging::Averaging(const Averaging& orig) {
}

Averaging::~Averaging() {
}

Segment* Averaging::processSegment(Segment* segment) {
    Segment* target = new SegmentImpl(segment.getK(), segment.getL() / 8, segment.getM() / 8);

    // mach ...

    return target;
}
