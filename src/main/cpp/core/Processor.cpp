/*
 * File:   Processor.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 5:14 PM
 */

#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "Processor.h"

using std::vector;

class Timer {
public:
	Timer() {
	}

	~Timer() {
	}

	void start() {
		std::time(&startTime);
	}

	void stop() {
		std::time(&stopTime);
	}

	string duration() const {
		const double secs = std::difftime(stopTime, startTime);
		const int h = secs / 3600.0;
		const int m = (secs - h * 3600.0) / 60.0;
		const double s = secs - h * 3600.0 - m * 60.0;

		std::stringstream ss;
		ss << std::setfill('0') << std::setw(2) << h << ":" << std::setw(2) << m
				<< ":" << std::fixed << std::setw(5) << std::setprecision(2) << s;
		return ss.str();
	}

private:
	time_t startTime;
	time_t stopTime;
};

Processor::Processor() {
}

Processor::~Processor() {
}

void Processor::process(Context& context) {
	Timer timer;

	context.getLogging()->info("Main processing started.", "Processor");
	try {
		timer.start();
		vector<shared_ptr<Module> > modules = context.getModules();
		foreach(shared_ptr<Module> module, modules)
				{
					try {
						module->start(context);
					} catch (ModuleException& e) {
						BOOST_THROW_EXCEPTION(e);
					} catch (exception& e) {
						BOOST_THROW_EXCEPTION(
								wrapException(e, module->getId(), "start"));
					}
				}
		do {
			foreach(shared_ptr<Module> module, modules)
					{
						try {
							module->process(context);
						} catch (ModuleException& e) {
							BOOST_THROW_EXCEPTION(e);
						} catch (exception& e) {
							BOOST_THROW_EXCEPTION(
									wrapException(e, module->getId(), "process"));
						}
					}
			context.moveSegmentsForward();
		} while (!context.isCompleted());
		reverse_foreach(shared_ptr<Module> module, modules)
				{
					try {
						module->stop(context);
					} catch (ModuleException& e) {
						BOOST_THROW_EXCEPTION(e);
					} catch (exception& e) {
						BOOST_THROW_EXCEPTION(
								wrapException(e, module->getId(), "stop"));
					}
				}
		timer.stop();
	} catch (exception& e) {
		context.handleError(e);
	}
	context.getLogging()->info(
			"Main processing completed in " + timer.duration() + " (HH:MM:SS.mm)",
			"Processor");
}

ModuleException Processor::wrapException(exception& e, const string& moduleName,
		const string& sourceMethod) const {
	string message = "Module '" + moduleName
			+ "' has thrown an exception in method '" + sourceMethod + "()':\n";
	message.append(e.what());

	ModuleException me;
	me.setMessage(message);

	return me;
}
