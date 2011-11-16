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

void PclTest::tearDown() {
}

void PclTest::prepareContext() {
    context = shared_ptr<Context>(new Context());
    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");

    JobOrderParser jobOrderParser;
    shared_ptr<JobOrder> jobOrder = jobOrderParser.parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_PCL.xml");
    context->setJobOrder(jobOrder);

    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");
    context->setDictionary(dictionary);

    shared_ptr<Logging> logging = jobOrderParser.createLogging("LOG.SY_UNT_PCL");
    context->setLogging(logging);
}

void PclTest::testComputeFlagValue() {
	long olcFlags = Constants::SY1_OLCI_LAND_FLAG;
	short slnFlags = Constants::SY1_SLSTR_CLOUD_FLAG;
	short sloFlags = Constants::SY1_SLSTR_CLOUD_FLAG;

	uint16_t landValue = Constants::SY2_LAND_FLAG;
	uint16_t cloudValue = Constants::SY2_CLOUD_FLAG;
	uint16_t value = Pcl::computeFlagValue(olcFlags, slnFlags, sloFlags);

	CPPUNIT_ASSERT((value & landValue) == landValue);
	CPPUNIT_ASSERT((value & cloudValue) == cloudValue);

	olcFlags = 8;
	value = Pcl::computeFlagValue(olcFlags, slnFlags, sloFlags);

	CPPUNIT_ASSERT((value & landValue) != landValue);
	CPPUNIT_ASSERT((value & cloudValue) == cloudValue);

	olcFlags = Constants::SY1_OLCI_LAND_FLAG;
	sloFlags = 8;
	value = Pcl::computeFlagValue(olcFlags, slnFlags, sloFlags);

	CPPUNIT_ASSERT((value & landValue) == landValue);
	CPPUNIT_ASSERT((value & cloudValue) == cloudValue);
}

void PclTest::testPcl() {
    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
    shared_ptr<Module> col = shared_ptr<Module>(new Col());
    shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

    context->addModule(reader);
    context->addModule(col);
    context->addModule(pcl);
    context->addModule(writer);

    Processor processor;
	processor.process(*context);
}
