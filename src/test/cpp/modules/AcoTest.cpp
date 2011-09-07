/* 
 * File:   AcoTest.cpp
 * Author: ralf
 * 
 * Created on August 16, 2011, 1:36 PM
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/reader/L1cReader.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"

#include "AcoTest.h"

extern shared_ptr<Context> context;

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(AcoTest);

const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");

AcoTest::AcoTest() {
}

AcoTest::~AcoTest() {
}

void AcoTest::setUp() {
	XPathInitializer init;

	shared_ptr<Dictionary> dictionary = DictionaryParser().parse(
			S3_SYNERGY_HOME + "/src/main/resources/dictionary");
	shared_ptr<JobOrder> jobOrder = JobOrderParser().parse(
			S3_SYNERGY_HOME
					+ "/src/test/resources/jobs/JobOrder.SY_UNT_SRE.xml");
	shared_ptr<Module> reader = shared_ptr<Module>(new L1cReader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());

	context->setDictionary(dictionary);
	context->setJobOrder(jobOrder);
	context->addModule(reader);
	context->addModule(col);
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
