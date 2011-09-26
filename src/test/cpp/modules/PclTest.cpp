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

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(PclTest);

PclTest::PclTest() {
}

PclTest::~PclTest() {
}

void PclTest::setUp() {
    XPathInitializer init;

    prepareContext();
}


void PclTest::prepareContext() {
    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");
    shared_ptr<JobOrder> jobOrder = JobOrderParser().parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_PCL.xml");

    context = shared_ptr<Context>(new Context());
    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    shared_ptr<Logging> logging = jobOrder->createLogging("LOG.SY_UNT_PCL");
    context->setLogging(logging);

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
    CPPUNIT_ASSERT_NO_THROW(pclToTest.getSourceAccessor(*context, "SLO_confidence", Constants::SEGMENT_SLO));
    CPPUNIT_ASSERT_NO_THROW(pclToTest.getSourceAccessor(*context, "SLN_confidence", Constants::SEGMENT_SLN));
    CPPUNIT_ASSERT_NO_THROW(pclToTest.getSourceAccessor(*context, "OLC_flags", Constants::SEGMENT_OLC));
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
	long olcFlags = 2147483648;
	short slnFlags = 16392;
	short sloFlags = 16392;

	size_t landValue = 32;
	size_t cloudValue = 1;
	uint16_t value = pclToTest.getValue(0, olcFlags, slnFlags, sloFlags);

	CPPUNIT_ASSERT((value & landValue) == landValue);
	CPPUNIT_ASSERT((value & cloudValue) == cloudValue);

	olcFlags = 8;
	slnFlags = 16392;
	sloFlags = 16392;
	value = pclToTest.getValue(0, olcFlags, slnFlags, sloFlags);

	CPPUNIT_ASSERT((value & landValue) != landValue);
	CPPUNIT_ASSERT((value & cloudValue) == cloudValue);

	olcFlags = 2147483648;
	slnFlags = 16392;
	sloFlags = 8;
	value = pclToTest.getValue(0, olcFlags, slnFlags, sloFlags);

	CPPUNIT_ASSERT((value & landValue) == landValue);
	CPPUNIT_ASSERT((value & cloudValue) != cloudValue);
}

void PclTest::testPcl() {
	Processor processor;
	processor.process(*context);
}
