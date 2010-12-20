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
 * File:   TestModule.cpp
 * Author: thomass
 * 
 * Created on December 1, 2010, 11:30 AM
 */

#include <iostream>

#include "ProcessorContext.h"
#include "TestModule.h"
#include "../core/Segment.h"

TestModule::TestModule() : AbstractModule("TestModule") {
}

TestModule::~TestModule() {
}

Segment* TestModule::processSegment(ProcessorContext& context) {
    Segment& segment = context.getSegment("SYN_COLLOCATED");
    if (!segment.hasVariable("SDR_1")) {
        segment.addVariableInt("SDR_1");
    }
    Grid& grid = segment.getGrid();
    Logger::get()->progress("Starting to process segment [" + segment.toString() + "]", getModuleId(), getVersion());
    for (size_t line = getMinLineNotComputed(segment, context); line <= segment.getGrid().getSizeL() - overlap; line++) {
        for (size_t cam = grid.getStartK(); cam <= grid.getSizeK(); cam++) {
            for (size_t col = grid.getStartM(); col <= grid.getSizeM(); col++) {
                segment.getAccessor("SDR_1").setInt( grid.getIndex(cam, line, col), 101);
            }
        }
    }
    size_t rowCount = grid.getSizeL();
    if (rowCount != context.getMaxLine(segment)) {
        context.setMaxLineComputed(segment, *this, rowCount - overlap);
    } else {
        context.setMaxLineComputed(segment, *this, rowCount);
    }

    return &segment;
}

size_t TestModule::getMinLineRequired(size_t line) const {
    return line - overlap;
}