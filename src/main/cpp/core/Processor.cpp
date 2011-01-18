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
        for (size_t i = 0; i < modules.size(); i++) {
            modules[i]->start(context);
        }
        while (!context.isCompleted()) {
            for (size_t i = 0; i < modules.size(); i++) {
                modules[i]->process(context);
            }
            context.shift();
        }
        for (size_t i = 0; i < modules.size(); i++) {
            modules[i]->stop(context);
        }
    } catch (SynException& e) {
        context.handleError(e);
    }
}
