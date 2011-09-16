/* 
 * File:   PclTest.cpp
 * Author: thomasstorm
 * 
 * Created on Sep 14, 2011, 1:36 PM
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
	XPathInitializer init;

	const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
	shared_ptr<Dictionary> dictionary = DictionaryParser().parse(
			S3_SYNERGY_HOME + "/src/main/resources/dictionary");
	shared_ptr<JobOrder> jobOrder = JobOrderParser().parse(
			S3_SYNERGY_HOME
					+ "/src/test/resources/jobs/JobOrder.SY_UNT_ACO.xml");
	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> writer = shared_ptr<Module>(new SynL2Writer());

	context->setDictionary(dictionary);
	context->setJobOrder(jobOrder);
	context->addModule(reader);
	context->addModule(col);
	context->addModule(pcl);
	context->addModule(writer);
}

PclTest::~PclTest() {
}

void PclTest::setUp() {
}

void PclTest::tearDown() {
	foreach(string id, context->getSegmentIds()) {
		context->removeSegment(id);
	}
}

void PclTest::testStart() {
	shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
	Segment& segment = context->addSegment(Constants::SEGMENT_SYN_COLLOCATED, 20, 20, 20, 20, 120);
	segment.addVariable("OLC_flags", Constants::TYPE_BYTE);
	segment.addVariable("SLN_confidence", Constants::TYPE_BYTE);
	segment.addVariable("SLO_confidence", Constants::TYPE_BYTE);
	CPPUNIT_ASSERT(!segment.hasVariable("SYN_flags"));
	pcl->start(*context);
	CPPUNIT_ASSERT(segment.hasVariable("SYN_flags"));
}

void PclTest::testGetAccessor() {
	Segment& segment = context->addSegment(Constants::SEGMENT_SYN_COLLOCATED, 20, 30, 5, 0, 19);
	segment.addVariable("OLC_flags", Constants::TYPE_LONG);
	segment.addVariable("SLN_confidence", Constants::TYPE_LONG);
	segment.addVariable("SLO_confidence", Constants::TYPE_LONG);
	Pcl pclToTest;
    CPPUNIT_ASSERT_NO_THROW(pclToTest.getSourceAccessor(*context, "SLO_confidence"));
    CPPUNIT_ASSERT_NO_THROW(pclToTest.getSourceAccessor(*context, "SLN_confidence"));
    CPPUNIT_ASSERT_NO_THROW(pclToTest.getSourceAccessor(*context, "OLC_flags"));
}

void PclTest::testGetIndex() {
	Segment& segment = context->addSegment(Constants::SEGMENT_SYN_COLLOCATED, 20, 30, 5, 0, 19);
	segment.addVariable("OLC_flags", Constants::TYPE_BYTE);
	segment.addVariable("SLN_confidence", Constants::TYPE_BYTE);
	segment.addVariable("SLO_confidence", Constants::TYPE_BYTE);
	Pcl pclToTest;
    pclToTest.start(*context);
	size_t index = pclToTest.getIndex(0, 0, 0);
	CPPUNIT_ASSERT(index == 0);

	index = pclToTest.getIndex(0, 0, 1);
	CPPUNIT_ASSERT(index == 1);

	index = pclToTest.getIndex(0, 0, 10);
	CPPUNIT_ASSERT(index == 10);

	index = pclToTest.getIndex(0, 1, 0);
	CPPUNIT_ASSERT(index == 30);
}

void PclTest::testGetValue() {
	Pcl pclToTest;
	long olcFlags = 0b10000000000000000000000000000000;
	short slnFlags = 0b100000000001000;
	short sloFlags = 0b100000000001000;

	size_t landValue = 0b100000;
	size_t cloudValue = 0b1;
	uint16_t value = pclToTest.getValue(0, olcFlags, slnFlags, sloFlags);

	CPPUNIT_ASSERT((value & landValue) == landValue);
	CPPUNIT_ASSERT((value & cloudValue) == cloudValue);

	olcFlags = 0b1000;
	slnFlags = 0b100000000001000;
	sloFlags = 0b100000000001000;
	value = pclToTest.getValue(0, olcFlags, slnFlags, sloFlags);

	CPPUNIT_ASSERT((value & landValue) != landValue);
	CPPUNIT_ASSERT((value & cloudValue) == cloudValue);

	olcFlags = 0b10000000000000000000000000000000;
	slnFlags = 0b100000000001000;
	sloFlags = 0b1000;
	value = pclToTest.getValue(0, olcFlags, slnFlags, sloFlags);

	CPPUNIT_ASSERT((value & landValue) == landValue);
	CPPUNIT_ASSERT((value & cloudValue) != cloudValue);
}

void PclTest::testPcl() {
	Processor processor;
	processor.process(*context);
}
