/*
 * File:   Processor.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 5:14 PM
 */

#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>

#include "Processor.h"

using std::vector;

Processor::Processor() {
}

Processor::~Processor() {
}

void Processor::process(Context& context) {
	Timer timer;

	context.getLogging()->info("Main processing started.", "MAIN");
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
			"Main processing completed in " + timer.time() + " (HH:MM:SS.mm)",
			"MAIN");
}

Processor::Timer::Timer() {
}

Processor::Timer::~Timer() {
}

void Processor::Timer::start() {
	std::time(&startTime);
}

void Processor::Timer::stop() {
	std::time(&stopTime);
}

string Processor::Timer::time() const {
	const double secs = std::difftime(stopTime, startTime);
	const int h = secs / 3600.0;
	const int m = (secs - h * 3600.0) / 60.0;
	const double s = secs - h * 3600.0 - m * 60.0;

	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(2) << h << ":" << std::setw(2) << m
			<< ":" << std::fixed << std::setw(5) << std::setprecision(2) << s;
	return oss.str();
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
