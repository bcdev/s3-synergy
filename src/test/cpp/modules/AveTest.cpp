/* 
 * File:   AveTest.cpp
 * Author: thomasstorm
 * 
 * Created on Sep 22, 2011
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/modules/Ave.h"
#include "../../../../src/main/cpp/writer/SynL2Writer.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"


#include "AveTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(AveTest);

AveTest::AveTest() {
}

AveTest::~AveTest() {
}

void AveTest::setUp() {
    XPathInitializer init;
    prepareContext();
    ave = shared_ptr<Ave>(new Ave());
}

void AveTest::prepareContext() {
    context = shared_ptr<Context>(new Context());
    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
    shared_ptr<JobOrderParser> jobOrderParser = shared_ptr<JobOrderParser>(new JobOrderParser());
    shared_ptr<JobOrder> jobOrder = jobOrderParser->parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_AVE.xml");
    context->setJobOrder(jobOrder);

    shared_ptr<Logging> logging = jobOrderParser->createLogging("LOG.SY_UNT_AVE");
    context->setLogging(logging);


    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");

    context->setDictionary(dictionary);
    context->setJobOrder(jobOrder);
}

void AveTest::tearDown() {
}

void AveTest::testAve() {
    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> writer = shared_ptr<Module>(new SynL2Writer());

    context->addModule(reader);
    context->addModule(ave);
    context->addModule(writer);

	Processor processor;
	processor.process(*context);
}
