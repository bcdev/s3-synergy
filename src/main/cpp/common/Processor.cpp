/*
 * File:   Processor.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 5:14 PM
 */

#include <vector>

#include "Processor.h"
#include "ProcessorContext.h"

using std::vector;

Processor::Processor() {
}

Processor::~Processor() {
}

void Processor::addModule(Module& module) {
    modules.push_back(&module);
}

bool Processor::isCompleted() const {
    return completed;
}

void Processor::process(ProcessorContext& context) {
    while (!isCompleted()) {
        bool processingCompleted = true;
        for (size_t i = 0; i < modules.size(); i++) {
            Segment* target = modules[i]->processSegment(context);
            if (target != 0 && !context.containsSegment(*target)) {
                context.addSegment(*target);
            }
            bool maxLinesEqual = context.getMaxLineComputed(*target, *modules[i]) == context.getMaxLine(*target);
            processingCompleted = processingCompleted && maxLinesEqual;
        }
        setCompleted(processingCompleted);
    }
}

void Processor::setCompleted(bool completed) {
    this->completed = completed;
}