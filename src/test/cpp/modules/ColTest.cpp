/* 
 * File:   ColTest.cpp
 * Author: thomasstorm
 * 
 * Created on Sep 15, 2011, 3:30 PM
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/SegmentImpl.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/writer/UniversalWriter.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"


#include "ColTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(ColTest);

ColTest::ColTest() {
}

ColTest::~ColTest() {
}

void ColTest::setUp() {
    XPathInitializer init;
    prepareContext();
    col = shared_ptr<Col>(new Col());
}

void ColTest::prepareContext() {
    context = shared_ptr<Context>(new Context());
    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");
    shared_ptr<JobOrder> jobOrder = JobOrderParser().parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_COL.xml");

    shared_ptr<Logging> logging = jobOrder->createLogging("LOG.SY_UNT_COL");
    context->setLogging(logging);

    context->setDictionary(dictionary);
    context->setJobOrder(jobOrder);
}

void ColTest::tearDown() {
}

void ColTest::testAddSlstrVariables() {
    context->addSegment(Constants::SEGMENT_OLC, 10, 10, 5, 0, 9);
    context->addSegment(Constants::SEGMENT_SLN, 10, 10, 5, 0, 9);
    context->addSegment(Constants::SEGMENT_SLO, 10, 10, 5, 0, 9);
    Segment& collocatedSegment = context->addSegment(Constants::SEGMENT_SYN_COLLOCATED, 10, 10, 5, 0, 9);

	// setting dummy type; this is done by reader normally, but not in test
    ProductDescriptor& pd = context->getDictionary()->getProductDescriptor("SY1");
    foreach(SegmentDescriptor* sd, pd.getSegmentDescriptors()) {
        foreach(VariableDescriptor* vd, sd->getVariableDescriptors()) {
            vd->setType(7);
        }
    }

    CPPUNIT_ASSERT(!collocatedSegment.hasVariable("L_1"));
    CPPUNIT_ASSERT(!collocatedSegment.hasVariable("L_18"));
    CPPUNIT_ASSERT(!collocatedSegment.hasVariable("L_19"));
    CPPUNIT_ASSERT(!collocatedSegment.hasVariable("L_30"));
    CPPUNIT_ASSERT(!collocatedSegment.hasVariable("L_25_exception"));
    CPPUNIT_ASSERT(!collocatedSegment.hasVariable("L_30_exception"));

	col->addSlstrVariables(*context);
	CPPUNIT_ASSERT(!collocatedSegment.hasVariable("L_1"));
	CPPUNIT_ASSERT(!collocatedSegment.hasVariable("L_18"));
	CPPUNIT_ASSERT(collocatedSegment.hasVariable("L_19"));
	CPPUNIT_ASSERT(collocatedSegment.hasVariable("L_30"));
	CPPUNIT_ASSERT(collocatedSegment.hasVariable("L_25_exception"));
	CPPUNIT_ASSERT(collocatedSegment.hasVariable("L_30_exception"));
}

void ColTest::testAddOlciVariables() {
    Segment& olciSegment = context->addSegment(Constants::SEGMENT_OLC, 10, 10, 5, 0, 9);
    Segment& collocatedSegment = context->addSegment(Constants::SEGMENT_SYN_COLLOCATED, 10, 10, 5, 0, 9);
    // setting dummy type; this is done by reader normally, but not in test
    ProductDescriptor& pd = context->getDictionary()->getProductDescriptor("SY1");
    foreach(SegmentDescriptor* sd, pd.getSegmentDescriptors()) {
        foreach(VariableDescriptor* vd, sd->getVariableDescriptors()) {
            vd->setType(7);
        }
    }
	olciSegment.addVariable("L_17", Constants::TYPE_BYTE);
	olciSegment.addVariable("L_17_er", Constants::TYPE_BYTE);
	olciSegment.addVariable("longitude", Constants::TYPE_BYTE);
	olciSegment.addVariable("latitude", Constants::TYPE_BYTE);
	olciSegment.addVariable("altitude", Constants::TYPE_BYTE);

    CPPUNIT_ASSERT(!collocatedSegment.hasVariable("L_1"));
    CPPUNIT_ASSERT(!collocatedSegment.hasVariable("L_18"));
    CPPUNIT_ASSERT(!collocatedSegment.hasVariable("L_1_er"));
    CPPUNIT_ASSERT(!collocatedSegment.hasVariable("L_18_er"));

	col->addOlciVariables(*context);
	CPPUNIT_ASSERT(collocatedSegment.hasVariable("L_1"));
	CPPUNIT_ASSERT(collocatedSegment.hasVariable("L_18"));
	CPPUNIT_ASSERT(collocatedSegment.hasVariable("L_1_er"));
	CPPUNIT_ASSERT(collocatedSegment.hasVariable("L_18_er"));
}

void ColTest::testCol() {
    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> writer = shared_ptr<Module>(new UniversalWriter());

    context->addModule(reader);
    context->addModule(col);
    context->addModule(writer);

	Processor processor;
	processor.process(*context);
}
