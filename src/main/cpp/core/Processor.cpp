/*
 * File:   Processor.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 5:14 PM
 */

#include <iostream>
#include <vector>

#include "Processor.h"

using std::vector;

Processor::Processor() {
}

Processor::~Processor() {
}

void Processor::process(Context& context) {
    try {
        vector<Module*> modules = context.getModules();

        foreach(Module* module, modules) {
            module->start(context);
        }
        do {

            foreach(Module* module, modules) {
                module->process(context);
            }
            context.moveSegmentsForward();
        } while (!context.isCompleted());

        reverse_foreach(Module* module, modules) {
            module->stop(context);
        }
    } catch (exception& e) {
        context.handleError(e);
    }
}
