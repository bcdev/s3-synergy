/*
 * SegmentWriterTest.cpp
 *
 *  Created on: 23.01.2012
 *      Author: thomasstorm
 */

#include "SegmentWriterTest.h"

#include <cstdlib>

#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/writer/SegmentWriter.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"
#include "../../../../src/main/cpp/core/NullLogging.h"

#include "MapSegmentProvider.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(SegmentWriterTest);

SegmentWriterTest::SegmentWriterTest() {
}

SegmentWriterTest::~SegmentWriterTest() {
}

void SegmentWriterTest::setUp() {
    XPathInitializer init;

    prepareContext();
}


void SegmentWriterTest::prepareContext() {
    context = shared_ptr<Context>(new Context());
    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    JobOrderParser jobOrderParser;
    shared_ptr<JobOrder> jobOrder = jobOrderParser.parse(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_GEN.xml");
    context->setJobOrder(jobOrder);

    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(Constants::S3_SYNERGY_HOME + "/src/main/resources/dictionary");
    context->setDictionary(dictionary);

    shared_ptr<Logging> logging = shared_ptr<Logging>(new NullLogging());
    context->setLogging(logging);
}

void SegmentWriterTest::tearDown() {
}

void SegmentWriterTest::testWriter() {
    shared_ptr<Module> module = shared_ptr<Module>(new MapSegmentProvider());
    shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());
    context->addModule(module);
    context->addModule(writer);

    Processor processor;
    processor.process(*context);
}
