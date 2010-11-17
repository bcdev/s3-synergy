/* 
 * File:   Processor.cpp
 * Author: thomass
 * 
 * Created on November 17, 2010, 5:14 PM
 */

#include "Processor.h"

Processor::Processor() : modules() {
}

Processor::~Processor() {
}

void Processor::process(Segment& segment) {
    for (size_t i = 0; i < modules.size(); i++) {
        modules[i]->processSegment(segment, segment);
    }
}

void Processor::addModule(Module& module) {
    modules.push_back(&module);
}