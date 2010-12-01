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
#include "VariableImpl.h"

TestModule::TestModule() : AbstractModule("TestModule") {
}

TestModule::~TestModule() {
}

Segment* TestModule::processSegment(ProcessorContext& context) {
    Segment& segment = context.getSegment("SYN_COLLOCATED");
    segment.addIntVariable(createTestVariable());
    Logger::get()->progress("Starting to process segment " + segment.toString(), getId(), getVersion());
    for (size_t l = getMinLineNotComputed(segment, context); l <= segment.getMaxL() - overlap; l++) {
        for (size_t k = segment.getMinK(); k <= segment.getMaxK(); k++) {
            for (size_t m = segment.getMinM(); m <= segment.getMaxM(); m++) {
                segment.setSampleInt("SDR_1", segment.computePosition(k, l, m), 666);
            }
        }
    }
    if (segment.getMaxL() != context.getMaxLine(segment)) {
        context.setMaxLineComputed(segment, *this, segment.getMaxL() - overlap);
    } else {
        context.setMaxLineComputed(segment, *this, segment.getMaxL());
    }

    return &segment;
}

size_t TestModule::getMinLineRequired(size_t line) const {
    return line - overlap;
}

Variable* TestModule::createTestVariable() {
    Variable* var = new VariableImpl("SDR_1");
    var->addAttribute(createStringAttribute("standard_name", "surface_directional_reflectance"));
    var->addAttribute(createStringAttribute("long_name", "Surface directional reflectance for SYN channel 1"));
    var->addAttribute(createFloatAttribute("_FillValue", -10.000));
    var->addAttribute(createFloatAttribute("scale_factor", 0.0001));
    var->addAttribute(createShortAttribute("valid_min", 0));
    var->addAttribute(createShortAttribute("valid_max", 10000));
    var->addAttribute(createStringAttribute("ancillary_variables", "SDR_1_er"));
    var->addAttribute(createShortAttribute("channel", 1));
    var->addAttribute(createFloatAttribute("central_wavelength", 400));
    var->addAttribute(createFloatAttribute("min_wavelength", 100));
    var->addAttribute(createFloatAttribute("max_wavelength", 700));
    return var;
}

Attribute<void*>* TestModule::createIntAttribute(string key, int value) {
    Attribute<void*>* attribute = new Attribute<void*>(key);
    int valueArray[1];
    valueArray[0] = value;
    attribute->setValue(valueArray);
    return attribute;
}

Attribute<void*>* TestModule::createShortAttribute(string key, int value) {
    Attribute<void*>* attribute = new Attribute<void*>(key);
    short valueArray[1];
    valueArray[0] = value;
    attribute->setValue(valueArray);
    return attribute;
}

Attribute<void*>* TestModule::createFloatAttribute(string key, float value) {
    Attribute<void*>* attribute = new Attribute<void*>(key);
    float valueArray[1];
    valueArray[0] = value;
    attribute->setValue(valueArray);
    return attribute;
}

Attribute<void*>* TestModule::createStringAttribute(string key, string value) {
    Attribute<void*>* attribute = new Attribute<void*>(key);
    string valueArray[1];
    valueArray[0] = value;
    attribute->setValue(valueArray);
    return attribute;
}