/* 
 * File:   L1cReaderTest.cpp
 * Author: ralf
 * 
 * Created on August 16, 2011, 1:36 PM
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/reader/L1cReader.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"

#include "L1cReaderTest.h"

extern shared_ptr<Context> context;

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(L1cReaderTest);

const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");

L1cReaderTest::L1cReaderTest() {
}

L1cReaderTest::~L1cReaderTest() {
}

void L1cReaderTest::setUp() {
    XPathInitializer init;

    dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");
    jobOrder = JobOrderParser().parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_SRE.xml");
    reader = shared_ptr<Module>(new L1cReader());

    context->setDictionary(dictionary);
    context->setJobOrder(jobOrder);
    context->addModule(*reader);
}

void L1cReaderTest::tearDown() {
	context->removeModule(*reader);
	context->setJobOrder(shared_ptr<JobOrder>());
	context->setDictionary(shared_ptr<Dictionary>());
}

void L1cReaderTest::testReader() {
	Processor processor;
	processor.process(*context);
}
