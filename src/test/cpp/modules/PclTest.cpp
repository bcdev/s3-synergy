/* 
 * File:   PclTest.cpp
 * Author: thomass
 * 
 * Created on August 16, 2011, 1:36 PM
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Pcl.h"
#include "../../../../src/main/cpp/writer/SynL2Writer.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"

#include "PclTest.h"

extern shared_ptr<Context> context;

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(PclTest);

PclTest::PclTest() {
}

PclTest::~PclTest() {
}

void PclTest::setUp() {
	XPathInitializer init;

	const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
	shared_ptr<Dictionary> dictionary = DictionaryParser().parse(
			S3_SYNERGY_HOME + "/src/main/resources/dictionary");
	shared_ptr<JobOrder> jobOrder = JobOrderParser().parse(
			S3_SYNERGY_HOME
					+ "/src/test/resources/jobs/JobOrder.SY_UNT_ACO.xml");
	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
	shared_ptr<Module> writer = shared_ptr<Module>(new SynL2Writer());

	context->setDictionary(dictionary);
	context->setJobOrder(jobOrder);
	context->addModule(reader);
	context->addModule(col);
	context->addModule(pcl);
}

void PclTest::tearDown() {
	foreach (shared_ptr<Module> m, context->getModules()) {
		context->removeModule(m);
    }
}

void PclTest::testStart() {
	shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
	Segment& segment = context->addSegment("SYN_COLLOCATED", 20, 20, 20, 20, 120);
	CPPUNIT_ASSERT(!segment.hasVariable("SYN_flags"));
	pcl->start(*context);
	CPPUNIT_ASSERT(segment.hasVariable("SYN_flags"));
}

void PclTest::testPcl() {
//	Processor processor;
//	processor.process(*context);
}
