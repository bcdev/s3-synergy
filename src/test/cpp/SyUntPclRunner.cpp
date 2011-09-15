/*
 * SyUntSwrRunner.cpp
 *
 *  Created on: Sep 14, 2011
 *      Author: thomasstorm
 */

// CppUnit site http://sourceforge.net/projects/cppunit/files
#include <cstdlib>
#include <iostream>

#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>

#include "../../../src/main/cpp/core/Context.h"
#include "../../../src/main/cpp/util/DefaultLogging.h"

using std::cout;
using std::endl;
using std::getenv;

shared_ptr<Context> context = shared_ptr<Context>(new Context());

static void prepareContext() {
	shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
	context->setErrorHandler(errorHandler);

	shared_ptr<DefaultLogging> logging = shared_ptr<DefaultLogging>(new DefaultLogging("LOG.SY_UNT_ACO"));
	logging->setOutLogLevel(Logging::LOG_LEVEL_INFO);
	logging->setErrLogLevel(Logging::LOG_LEVEL_INFO);
	context->setLogging(logging);
}

int main() {
	if (getenv("S3_SYNERGY_HOME") == NULL) {
		cout << "The test runner cannot be executed because the" << endl;
		cout << "'S3_SYNERGY_HOME' environment variable has not" << endl;
		cout << "been set." << endl;
		return 1;
	}
	prepareContext();

	// Create the event manager and test controller
	CPPUNIT_NS::TestResult controller;

	// Add a listener that collects test result
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener(&result);

	// Add a listener that print dots as test run.
	CPPUNIT_NS::BriefTestProgressListener progress;
	controller.addListener(&progress);

	// Add the top suite to the test runner
	CPPUNIT_NS::TestRunner runner;
	runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
	runner.run(controller);

	// Print test in a compiler compatible format.
	CPPUNIT_NS::CompilerOutputter outputter(&result, CPPUNIT_NS::stdCOut());
	outputter.write();

	return result.wasSuccessful() ? 0 : 1;
}
