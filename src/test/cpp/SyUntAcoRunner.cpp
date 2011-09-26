/*
 * SyUntSwrRunner.cpp
 *
 *  Created on: Sep 1, 2011
 *      Author: ralf
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
#include "../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../src/main/cpp/util/JobOrderParser.h"

using std::cout;
using std::endl;
using std::getenv;

shared_ptr<Context> context = shared_ptr<Context>(new Context());

static void prepareContext() {
	XPathInitializer init;

	shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
	context->setErrorHandler(errorHandler);

	shared_ptr<DefaultLogging> logging = shared_ptr<DefaultLogging>(new DefaultLogging("LOG.SY_UNT_ACO"));
	context->setLogging(logging);

	const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
	shared_ptr<JobOrder> jobOrder = JobOrderParser().parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_ACO.xml");
	context->setJobOrder(jobOrder);
	logging->setOutLogLevel(jobOrder->getIpfConfiguration().getStandardLogLevel());

	shared_ptr<Dictionary> dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");
	context->setDictionary(dictionary);
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
