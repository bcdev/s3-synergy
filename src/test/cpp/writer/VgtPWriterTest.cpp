/*
 * VgtPWriterTest.cpp
 *
 *  Created on: Jan 16, 2012
 *      Author: thomasstorm
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/writer/VgtPWriter.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"

#include "VgtSegmentProvider.h"
#include "VgtPWriterTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(VgtPWriterTest);

VgtPWriterTest::VgtPWriterTest() {
}

VgtPWriterTest::~VgtPWriterTest() {
}

void VgtPWriterTest::setUp() {
	XPathInitializer init;

	prepareContext();
}


void VgtPWriterTest::prepareContext() {
    context = shared_ptr<Context>(new Context());
    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    JobOrderParser jobOrderParser;
    shared_ptr<JobOrder> jobOrder = jobOrderParser.parse(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_VWR.xml");
    context->setJobOrder(jobOrder);

    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(Constants::S3_SYNERGY_HOME + "/src/main/resources/dictionary");
    context->setDictionary(dictionary);

    shared_ptr<Logging> logging = jobOrderParser.createLogging("LOG.SY_UNT_VWR");
    context->setLogging(logging);
}

void VgtPWriterTest::tearDown() {
}

void VgtPWriterTest::testWriter() {
    shared_ptr<Module> module = shared_ptr<Module>(new VgtSegmentProvider());
    shared_ptr<Module> writer = shared_ptr<Module>(new VgtPWriter());
    context->addModule(module);
    context->addModule(writer);

    Processor processor;
	processor.process(*context);
}
