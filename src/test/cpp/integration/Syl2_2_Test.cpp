/* 
 * File:   AcoTest.cpp
 * Author: ralf
 * 
 * Created on August 16, 2011, 1:36 PM
 */

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/modules/Aer.h"
#include "../../../../src/main/cpp/modules/Aei.h"
#include "../../../../src/main/cpp/modules/Ave.h"
#include "../../../../src/main/cpp/modules/Aco.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Pcl.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"
#include "../../../../src/main/cpp/writer/SynL2Writer.h"

#include "Syl2_2_Test.h"

CPPUNIT_TEST_SUITE_REGISTRATION(Syl2_2_Test);

Syl2_2_Test::Syl2_2_Test() {
}

Syl2_2_Test::~Syl2_2_Test() {
}

void Syl2_2_Test::setUp() {
    prepareContext();
}

void Syl2_2_Test::prepareContext() {
    XPathInitializer init;

    context = shared_ptr<Context>(new Context());

    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
    shared_ptr<JobOrderParser> jobOrderParser = shared_ptr<JobOrderParser>(new JobOrderParser());
    shared_ptr<JobOrder> jobOrder = jobOrderParser->parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_INT_SYL2_2.xml");
    context->setJobOrder(jobOrder);

    shared_ptr<Logging> logging = jobOrderParser->createLogging("LOG.SY_INT_SYL_2");
    context->setLogging(logging);

    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");
    context->setDictionary(dictionary);
}

void Syl2_2_Test::tearDown() {
}

void Syl2_2_Test::testIntegrationOfSynL2Modules() {
	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
	shared_ptr<Module> ave = shared_ptr<Module>(new Ave());
	shared_ptr<Module> aer = shared_ptr<Module>(new Aer());
	shared_ptr<Module> aei = shared_ptr<Module>(new Aei());
	shared_ptr<Module> aco = shared_ptr<Module>(new Aco());
	shared_ptr<Module> writer = shared_ptr<Module>(new SynL2Writer());

	context->addModule(reader);
	context->addModule(col);
	context->addModule(pcl);
	context->addModule(ave);
	context->addModule(aer);
	context->addModule(aei);
	context->addModule(aco);
	context->addModule(writer);

	Processor processor;
	processor.process(*context);
}
