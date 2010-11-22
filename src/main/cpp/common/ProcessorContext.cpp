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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 *
 * File:   ProcessorContext.cpp
 * Author: ralf
 * 
 * Created on November 22, 2010, 10:58 AM
 */

#include <stdexcept>

#include "Module.h"
#include "ProcessorContext.h"

ProcessorContext::ProcessorContext() {
}

ProcessorContext::~ProcessorContext() {
}

Segment& ProcessorContext::getSegment(string id) const {
}

Module& ProcessorContext::getModule(string moduleId) const {
    for( size_t i = 0; i < modules.size(); i++ ) {
        Module* currentModule = modules.at(i);
        if( currentModule->getId().compare( moduleId ) == 0 ) {
            return *currentModule;
        }
    }
    throw std::invalid_argument("no module for moduleId " + moduleId);
}

const vector<Module*>& ProcessorContext::getModules() const {
    return modules;
}

void ProcessorContext::addModule(Module& module) {
    modules.push_back(&module);
}

void ProcessorContext::setMaxComputedLine(Segment& segment, Module& module, size_t line) {

}

size_t ProcessorContext::getMaxComputedLine(Segment& segment, Module& module) const {

}

size_t ProcessorContext::getOverlap(Segment& segment) const {

}

void ProcessorContext::setOverlap(Segment& segment, size_t overlap) {

}