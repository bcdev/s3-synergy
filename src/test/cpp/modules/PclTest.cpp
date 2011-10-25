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
#include "../../../../src/main/cpp/writer/SegmentWriter.h"
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

    context = shared_ptr<Context>(new Context());
    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    shared_ptr<JobOrderParser> jobOrderParser = shared_ptr<JobOrderParser>(new JobOrderParser());
    shared_ptr<JobOrder> jobOrder = jobOrderParser->parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_PCL.xml");
    context->setJobOrder(jobOrder);

    shared_ptr<Logging> logging = jobOrderParser->createLogging("LOG.SY_UNT_PCL");
    context->setLogging(logging);


    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
    shared_ptr<Module> col = shared_ptr<Module>(new Col());
    shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

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
	Pcl pcl;
	Segment& segment = context->addSegment(Constants::SEGMENT_SYN_COLLOCATED, 20, 20, 20, 20, 120);
	segment.addVariable("OLC_flags", Constants::TYPE_BYTE);
	segment.addVariable("SLN_confidence", Constants::TYPE_BYTE);
	segment.addVariable("SLO_confidence", Constants::TYPE_BYTE);
	for (size_t b = 19; b <= 30; b++) {
		segment.addVariable("L_" + lexical_cast<string>(b), Constants::TYPE_DOUBLE);
	}
	CPPUNIT_ASSERT(!segment.hasVariable("SYN_flags"));
	pcl.start(*context);
	CPPUNIT_ASSERT(segment.hasVariable("SYN_flags"));
}

void PclTest::testGetValue() {
	long olcFlags = Pcl::SY1_OLCI_LAND_FLAG;
	short slnFlags = Pcl::SY1_SLSTR_CLOUD_FLAG;
	short sloFlags = Pcl::SY1_SLSTR_CLOUD_FLAG;

	uint16_t landValue = Pcl::SY2_LAND_FLAG;
	uint16_t cloudValue = Pcl::SY2_CLOUD_FLAG;
	uint16_t value = Pcl::computeFlagValue(olcFlags, slnFlags, sloFlags);

	CPPUNIT_ASSERT((value & landValue) == landValue);
	CPPUNIT_ASSERT((value & cloudValue) == cloudValue);

	olcFlags = 8;
	value = Pcl::computeFlagValue(olcFlags, slnFlags, sloFlags);

	CPPUNIT_ASSERT((value & landValue) != landValue);
	CPPUNIT_ASSERT((value & cloudValue) == cloudValue);

	olcFlags = Pcl::SY1_OLCI_LAND_FLAG;
	sloFlags = 8;
	value = Pcl::computeFlagValue(olcFlags, slnFlags, sloFlags);

	CPPUNIT_ASSERT((value & landValue) == landValue);
	CPPUNIT_ASSERT((value & cloudValue) == cloudValue);
}

void PclTest::testPcl() {
	Processor processor;
	processor.process(*context);
}
