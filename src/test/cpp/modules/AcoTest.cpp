/* 
 * File:   AcoTest.cpp
 * Author: ralf
 * 
 * Created on August 16, 2011, 1:36 PM
 */

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/modules/Aco.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/util/DefaultLogging.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"
#include "../../../../src/main/cpp/writer/SynL2Writer.h"

#include "AcoTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(AcoTest);

AcoTest::AcoTest() {
}

AcoTest::~AcoTest() {
}

void AcoTest::setUp() {
    prepareContext();
	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> aco = shared_ptr<Module>(new Aco());
	shared_ptr<Module> writer = shared_ptr<Module>(new SynL2Writer());

	context->addModule(reader);
	context->addModule(col);
	context->addModule(aco);
	context->addModule(writer);
}

void AcoTest::prepareContext() {
    XPathInitializer init;

    context = shared_ptr<Context>(new Context());

    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    shared_ptr<DefaultLogging> logging = shared_ptr<DefaultLogging>(new DefaultLogging("LOG.SY_UNT_ACO"));
    context->setLogging(logging);

    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
    shared_ptr<JobOrder> jobOrder = JobOrderParser().parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_ACO.xml");
    context->setJobOrder(jobOrder);
    logging->setOutLogLevel(jobOrder->getIpfConfiguration().getStandardLogLevel());
    logging->setErrLogLevel(jobOrder->getIpfConfiguration().getErrorLogLevel());

    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");
    context->setDictionary(dictionary);
}

void AcoTest::tearDown() {
	foreach (shared_ptr<Module> m, context->getModules())
			{
				context->removeModule(m);
			}
}

void AcoTest::testAco() {
	Processor processor;
	processor.process(*context);
}
