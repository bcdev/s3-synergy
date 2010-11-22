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

void Processor::process(ProcessorContext& context) {
    for (size_t i = 0; i < modules.size(); i++) {
        //Segment* target = modules[i]->processSegment(context);
        //if (!context.contains(target)) {
           //context.add(modules[i].getTargetId(), target);
        //}
    }
}

void Processor::addModule(Module& module) {
    modules.push_back(&module);
}