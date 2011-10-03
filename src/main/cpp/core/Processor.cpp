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

#include "ExitCode.h"
#include "Processor.h"

using std::vector;

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
					} catch (boost::exception& e) {
						addErrorInfo(e, module->getId(), "start()",
								ExitCode::FAILURE);
						throw;
					} catch (std::exception& e) {
						addErrorInfo(e, module->getId(), "start()",
								ExitCode::FAILURE);
						BOOST_THROW_EXCEPTION(e);
					}
				}
		do {
			foreach(shared_ptr<Module> module, modules)
					{
						try {
							module->process(context);
						} catch (boost::exception& e) {
							addErrorInfo(e, module->getId(), "process()",
									ExitCode::FAILURE);
							throw;
						} catch (std::exception& e) {
							addErrorInfo(e, module->getId(), "process()",
									ExitCode::FAILURE);
							BOOST_THROW_EXCEPTION(e);
						}
					}
			context.moveSegmentsForward();
		} while (!context.isCompleted());
		reverse_foreach(shared_ptr<Module> module, modules)
				{
					try {
						module->stop(context);
					} catch (boost::exception& e) {
						addErrorInfo(e, module->getId(), "stop()",
								ExitCode::INCOMPLETE);
						throw;
					} catch (std::exception& e) {
						addErrorInfo(e, module->getId(), "stop()",
								ExitCode::INCOMPLETE);
						BOOST_THROW_EXCEPTION(e);
					}
				}
		timer.stop();
	} catch (std::exception& e) {
		context.handleError(e);
	}
	context.getLogging()->info(
			"Main processing completed in " + timer.getTime()
					+ " (HH:MM:SS.mm)", "Processor");
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

string Processor::Timer::getTime() const {
	const double secs = std::difftime(stopTime, startTime);
	const int h = secs / 3600.0;
	const int m = (secs - h * 3600.0) / 60.0;
	const double s = secs - h * 3600.0 - m * 60.0;

	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(2) << h << ":" << std::setw(2) << m
			<< ":" << std::fixed << std::setw(5) << std::setprecision(2) << s;
	return oss.str();
}
