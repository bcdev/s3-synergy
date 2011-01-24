/*
 * File:   Processor.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 5:14 PM
 */

#include <iostream>
#include <vector>

#include "../util/ModuleException.h"
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
            try {
                module->start(context);
            } catch (ModuleException& e) {
                BOOST_THROW_EXCEPTION(e);
            } catch (exception& e) {
                wrapException(e, module->getId(), "start");
            }
        }
        do {
            foreach(Module* module, modules) {
                try {
                    module->process(context);
                } catch (ModuleException& e) {
                    throw e;
                } catch (exception& e) {
                    wrapException(e, module->getId(), "process");
                }
            }
            context.moveSegmentsForward();
        } while (!context.isCompleted());
        reverse_foreach(Module* module, modules) {
            try {
                module->stop(context);
            } catch (ModuleException& e) {
                throw e;
            } catch (exception& e) {
                wrapException(e, module->getId(), "stop");
            }
        }
    } catch (exception& e) {
        context.handleError(e);
    }
}

void Processor::wrapException(exception& e, string moduleName, string sourceMethod) {
    ModuleException me;
    string message = "Module '" + moduleName + "' has thrown an exception in method '" + sourceMethod + "()'. Exception content:\n";
    message.append(e.what());
    me.setMessage(message);
    BOOST_THROW_EXCEPTION(me);
}