/*
 * SyL2WriterTest.cpp
 *
 *  Created on: Sep 2, 2011
 *      Author: ralf
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/writer/SynL2Writer.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"

#include "SynL2SegmentProvider.h"
#include "SynL2WriterTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(SynL2WriterTest);

SynL2WriterTest::SynL2WriterTest() {
}

SynL2WriterTest::~SynL2WriterTest() {
}

void SynL2WriterTest::setUp() {
	XPathInitializer init;

	prepareContext();
}


void SynL2WriterTest::prepareContext() {
    context = shared_ptr<Context>(new Context());
    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
    shared_ptr<Module> module = getSegmentProvider();
    shared_ptr<Module> writer = shared_ptr<Module>(new SynL2Writer());
    context->addModule(module);
    context->addModule(writer);

    shared_ptr<JobOrderParser> jobOrderParser = shared_ptr<JobOrderParser>(new JobOrderParser());
    shared_ptr<JobOrder> jobOrder = jobOrderParser->parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_SWR.xml");
    context->setJobOrder(jobOrder);

    const bool breakpointEnabled = jobOrder->getIpfConfiguration().isBreakpointEnable();
    shared_ptr<DictionaryParser> dictionaryParser = shared_ptr<DictionaryParser>(new DictionaryParser(breakpointEnabled));
    shared_ptr<Dictionary> dictionary = dictionaryParser->parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");
    context->setDictionary(dictionary);

    shared_ptr<Logging> logging = jobOrderParser->createLogging("LOG.SY_UNT_SWR");
    context->setLogging(logging);
}

void SynL2WriterTest::tearDown() {
}

void SynL2WriterTest::testWriter() {
	Processor processor;
	processor.process(*context);
}

shared_ptr<Module> SynL2WriterTest::getSegmentProvider() {
    return shared_ptr<Module>(new SynL2SegmentProvider());
}
