/* 
 * File:   L1cReaderTest.cpp
 * Author: ralf
 * 
 * Created on August 16, 2011, 1:36 PM
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
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
    shared_ptr<JobOrder> jobOrder = JobOrderParser().parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_SRE.xml");
    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());

    context->setDictionary(dictionary);
    context->setJobOrder(jobOrder);
    context->addModule(reader);

    shared_ptr<Logging> logging = jobOrder->createLogging("LOG.SY_UNT_SRE");
    context->setLogging(logging);
}

void SynL1ReaderTest::tearDown() {
}

void SynL1ReaderTest::testReader() {
	Processor processor;
	processor.process(*context);
}
