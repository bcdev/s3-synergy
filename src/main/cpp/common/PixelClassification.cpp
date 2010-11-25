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
 * File:   PixelClassification.cpp
 * Author: thomass
 * 
 * Created on November 10, 2010, 4:26 PM
 */

#include "PixelClassification.h"
#include "ProcessorContext.h"

PixelClassification::PixelClassification() : AbstractModule("PCL") {
}

PixelClassification::~PixelClassification() {
}

Segment* PixelClassification::processSegment(ProcessorContext& context) {
    string segmentId = "SYN_COLLOCATED";
    Segment& source = context.getSegment(segmentId);

    context.getLogger()->logProgress( "Starting to process segment " + source.toString(), getId(), getVersion() );
    // TODO - parallelize

    for (size_t l = source.getMinL(); l <= source.getMaxL(); l++) {
        for (size_t k = source.getMinK(); k <= source.getMaxK(); k++) {
            for (size_t m = source.getMinM(); m <= source.getMaxM(); m++) {
                const size_t p = source.computePosition(k, l, m);
                const int olcFlags = source.getSampleInt("F_OLC", p);
                const int slnFlags = source.getSampleInt("F_SLN", p);
                const int sloFlags = source.getSampleInt("F_SLO", p);
                const bool olcLand = (olcFlags & 0x1000) != 0;
                const bool slnLand = (slnFlags & 0x0800) != 0;
                const bool sloLand = (sloFlags & 0x0800) != 0;
                const bool slnCloud = (slnFlags & 0x00400000) != 0;
                const bool sloCloud = (sloFlags & 0x00400000) != 0;

                // TODO - OLCI L2 Pixel classification

                int synFlags = 0;
                if (olcLand && slnLand && sloLand) {
                    synFlags |= 0x0020;
                }
                if (slnCloud || sloCloud) {
                    synFlags |= 0x0001;
                }
                source.setSampleInt("F_SYN", p, synFlags);
            }
        }
    }
    context.setMaxLineComputed(source, *this, source.getMaxL());
    context.setMinLineRequired(source, source.getMaxL() + 1);
    
    return &source;
}
