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
    // todo - extract methods start, process, and stop (nf-20110419)
    try {
        vector<shared_ptr<Module> > modules = context.getModules();
        foreach(shared_ptr<Module> module, modules) {
            try {
                module->start(context);
            } catch (ModuleException& e) {
                BOOST_THROW_EXCEPTION(e);
            } catch (exception& e) {
            	BOOST_THROW_EXCEPTION(wrapException(e, module->getId(), "start"));
            }
        }
        do {
            foreach(shared_ptr<Module> module, modules) {
                try {
                    module->process(context);
                } catch (ModuleException& e) {
                    BOOST_THROW_EXCEPTION(e);
                } catch (exception& e) {
                	BOOST_THROW_EXCEPTION(wrapException(e, module->getId(), "process"));
                }
            }
            context.moveSegmentsForward();
        } while (!context.isCompleted());
        reverse_foreach(shared_ptr<Module> module, modules) {
            try {
                module->stop(context);
            } catch (ModuleException& e) {
                BOOST_THROW_EXCEPTION(e);
            } catch (exception& e) {
            	BOOST_THROW_EXCEPTION(wrapException(e, module->getId(), "stop"));
            }
        }
    } catch (exception& e) {
        context.handleError(e);
    }
}

ModuleException Processor::wrapException(exception& e, const string& moduleName, const string& sourceMethod) const {
    string message = "Module '" + moduleName + "' has thrown an exception in method '" + sourceMethod + "()':\n";
    message.append(e.what());

    ModuleException me;
    me.setMessage(message);

    return me;
}
