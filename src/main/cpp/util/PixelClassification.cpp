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
#include "../core/Context.h"

PixelClassification::PixelClassification() : DefaultModule("PCL") {
}

PixelClassification::~PixelClassification() {
}

void PixelClassification::process(Context& context) {
    Segment& segment = context.getSegment("SYN_COLLOCATED");
    if (!segment.hasVariable("SYN_flags")) {
        segment.addVariableInt("SYN_flags");
    }
    context.getLogging()->progress("Starting to process segment [" + segment.toString() + "]", getId(), getVersion());
    Grid& grid = segment.getGrid();

    size_t startLine = getStartL(context, segment);
    size_t endLine = getDefaultEndL(startLine, grid);

    for (size_t cam = grid.getStartK(); cam < grid.getSizeK(); cam++) {
        for (size_t line = startLine; line <= endLine; line++) {
            for (size_t col = grid.getStartM(); col < grid.getSizeM(); col++) {
                const size_t p = grid.getIndex(cam, line, col);
                const int olcFlags = segment.getAccessor("F_OLC").getInt(p);
                const int slnFlags = segment.getAccessor("F_SLN").getInt(p);
                const int sloFlags = segment.getAccessor("F_SLO").getInt(p);
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
                segment.getAccessor("SYN_flags").setInt(p, synFlags);
            }
        }
    }
    context.setMaxLComputed(segment, *this, endLine);
}