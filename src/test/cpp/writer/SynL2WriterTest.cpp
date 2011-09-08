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

extern shared_ptr<Context> context;

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(SynL2WriterTest);

SynL2WriterTest::SynL2WriterTest() {
}

SynL2WriterTest::~SynL2WriterTest() {
}

void SynL2WriterTest::setUp() {
	XPathInitializer init;

	const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
	shared_ptr<Dictionary> dictionary = DictionaryParser().parse(
			S3_SYNERGY_HOME + "/src/main/resources/dictionary");
	shared_ptr<JobOrder> jobOrder = JobOrderParser().parse(
			S3_SYNERGY_HOME
					+ "/src/test/resources/jobs/JobOrder.SY_UNT_SWR.xml");
	shared_ptr<Module> module = shared_ptr<Module>(new SynL2SegmentProvider());
	shared_ptr<Module> writer = shared_ptr<Module>(new SynL2Writer());

	context->setDictionary(dictionary);
	context->setJobOrder(jobOrder);
	context->addModule(module);
	context->addModule(writer);
}

void SynL2WriterTest::tearDown() {
	foreach (shared_ptr<Module> m, context->getModules())
			{
				context->removeModule(m);
			}
	context->setJobOrder(shared_ptr<JobOrder>());
	context->setDictionary(shared_ptr<Dictionary>());
}

void SynL2WriterTest::testWriter() {
	Processor processor;
	processor.process(*context);
}
