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
#include "SegmentImpl.h"

TestModule::TestModule() : AbstractModule("TestModule") {
}

TestModule::~TestModule() {
}

Segment* TestModule::processSegment(ProcessorContext& context) {
    Segment& segment = context.getSegment("SYN_COLLOCATED");
    if (segment.getIntVariable("SDR_1") == 0 ) {
        segment.addIntVariable(createVariable("SDR_1"));
    }
    Logger::get()->progress("Starting to process segment [" + segment.toString() + "]", getModuleId(), getVersion());
    for (size_t l = getMinLineNotComputed(segment, context); l <= segment.getRowCount() - overlap; l++) {
        for (size_t k = segment.getMinK(); k <= segment.getCamCount(); k++) {
            for (size_t m = segment.getM(); m <= segment.getColCount(); m++) {
                segment.setSampleInt("SDR_1", segment.computePosition(k, l, m), 666);
            }
        }
    }
    if (segment.getRowCount() != context.getMaxLine(segment)) {
        context.setMaxLineComputed(segment, *this, segment.getRowCount() - overlap);
    } else {
        context.setMaxLineComputed(segment, *this, segment.getRowCount());
    }

    return &segment;
}

size_t TestModule::getMinLineRequired(size_t line) const {
    return line - overlap;
}