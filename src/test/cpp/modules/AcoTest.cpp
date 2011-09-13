/* 
 * File:   AcoTest.cpp
 * Author: ralf
 * 
 * Created on August 16, 2011, 1:36 PM
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Aco.h"
#include "../../../../src/main/cpp/writer/SynL2Writer.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"

#include "AcoTest.h"

extern shared_ptr<Context> context;

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(AcoTest);

AcoTest::AcoTest() {
}

AcoTest::~AcoTest() {
}

void AcoTest::setUp() {
	XPathInitializer init;

	const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
	shared_ptr<Dictionary> dictionary = DictionaryParser().parse(
			S3_SYNERGY_HOME + "/src/main/resources/dictionary");
	shared_ptr<JobOrder> jobOrder = JobOrderParser().parse(
			S3_SYNERGY_HOME
					+ "/src/test/resources/jobs/JobOrder.SY_UNT_ACO.xml");
	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> aco = shared_ptr<Module>(new Aco());
	shared_ptr<Module> writer = shared_ptr<Module>(new SynL2Writer());

	context->setDictionary(dictionary);
	context->setJobOrder(jobOrder);
	context->addModule(reader);
	context->addModule(col);
	context->addModule(aco);
	context->addModule(writer);
}

void AcoTest::tearDown() {
	foreach (shared_ptr<Module> m, context->getModules())
			{
				context->removeModule(m);
			}
	context->setJobOrder(shared_ptr<JobOrder>());
	context->setDictionary(shared_ptr<Dictionary>());
}

void AcoTest::testAco() {
	Processor processor;
	processor.process(*context);
}
