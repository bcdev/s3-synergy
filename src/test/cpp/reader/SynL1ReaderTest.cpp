/* 
 * File:   L1cReaderTest.cpp
 * Author: ralf
 * 
 * Created on August 16, 2011, 1:36 PM
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/writer/SegmentWriter.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"

#include "SynL1ReaderTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(SynL1ReaderTest);

SynL1ReaderTest::SynL1ReaderTest() {
}

SynL1ReaderTest::~SynL1ReaderTest() {
}

void SynL1ReaderTest::setUp() {
	XPathInitializer init;

	prepareContext();
}

void SynL1ReaderTest::prepareContext() {
    context = shared_ptr<Context>(new Context());
    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");
    context->setDictionary(dictionary);

    JobOrderParser jobOrderParser;
    shared_ptr<JobOrder> jobOrder = jobOrderParser.parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_SRE.xml");
    context->setJobOrder(jobOrder);

    shared_ptr<Logging> logging = jobOrderParser.createLogging("LOG.SY_UNT_ACO");
    context->setLogging(logging);
}

void SynL1ReaderTest::tearDown() {
}

void SynL1ReaderTest::testReader() {
    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());
	context->addModule(reader);
	context->addModule(writer);

	Processor processor;
	processor.process(*context);
}
