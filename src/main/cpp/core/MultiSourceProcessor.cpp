/*
 * File:   MultiSourceProcessor.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 5:14 PM
 */

#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>

#include "ExitCode.h"
#include "MultiSourceProcessor.h"

using std::vector;

MultiSourceProcessor::MultiSourceProcessor() {
}

MultiSourceProcessor::~MultiSourceProcessor() {
}

void MultiSourceProcessor::process(Context& context) {
	Timer timer;

	context.getLogging().progress("Main processing started.", "MultiSourceProcessor");
	try {
	    const vector<Input>& inputList = context.getJobOrder().getIpfProcessors()[0].getInputList();
        timer.start();
        context.setStartTime(timer.getStartTime());
        vector<shared_ptr<Module> > modules = context.getModules();
        foreach (Input input, inputList) {
            context.setSourcePath(input.getFileNames()[0]);
            foreach (shared_ptr<Module> module, modules) {
                try {
                    module->start(context);
                } catch (boost::exception& e) {
                    addErrorInfo(e, module->getId(), "start()", ExitCode::FAILURE);
                    throw;
                } catch (std::exception& e) {
                    addErrorInfo(e, module->getId(), "start()", ExitCode::FAILURE);
                    BOOST_THROW_EXCEPTION(e);
                }
            }
            do {
                foreach (shared_ptr<Module> module, modules) {
                    try {
                        module->process(context);
                    } catch (boost::exception& e) {
                        addErrorInfo(e, module->getId(), "process()", ExitCode::FAILURE);
                            throw;
                    } catch (std::exception& e) {
                        addErrorInfo(e, module->getId(), "process()", ExitCode::FAILURE);
                        BOOST_THROW_EXCEPTION(e);
                    }
                }
                context.moveSegmentsForward();
            } while (!context.isCompleted());
            reverse_foreach(shared_ptr<Module> module, modules) {
                try {
                    module->stop(context);
                } catch (boost::exception& e) {
                    addErrorInfo(e, module->getId(), "stop()", ExitCode::INCOMPLETE);
                    throw;
                    } catch (std::exception& e) {
                        addErrorInfo(e, module->getId(), "stop()", ExitCode::INCOMPLETE);
                        BOOST_THROW_EXCEPTION(e);
                    }
            }
        }
        timer.stop();
    } catch (std::exception& e) {
        context.handleError(e);
    }
    context.getLogging().progress("Main processing completed in " + timer.getDiffTime() + " (HH:MM:SS.mm)", "MultiSourceProcessor");
}

MultiSourceProcessor::Timer::Timer() {
}

MultiSourceProcessor::Timer::~Timer() {
}

void MultiSourceProcessor::Timer::start() {
	std::time(&startTime);
}

void MultiSourceProcessor::Timer::stop() {
	std::time(&stopTime);
}

string MultiSourceProcessor::Timer::getDiffTime() const {
	const double seconds = std::difftime(stopTime, startTime);
    const int hours = seconds / 3600.0;
    const int minutes = (seconds - hours * 3600.0) / 60.0;
    const double secondsFraction = seconds - hours * 3600.0 - minutes * 60.0;

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hours << ":" << std::setw(2) << minutes << ":" << std::fixed << std::setw(5) << std::setprecision(2) << secondsFraction;
    return oss.str();
}

time_t MultiSourceProcessor::Timer::getStartTime() const {
    return startTime;
}
