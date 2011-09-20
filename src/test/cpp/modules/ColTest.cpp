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
#include "../../../../src/main/cpp/writer/SynL2Writer.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"
#include "../../../../src/main/cpp/util/DefaultLogging.h"


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

    shared_ptr<DefaultLogging> logging = shared_ptr<DefaultLogging>(new DefaultLogging("LOG.SY_UNT_COL"));
    logging->setOutLogLevel(Logging::LOG_LEVEL_INFO);
    logging->setErrLogLevel(Logging::LOG_LEVEL_INFO);
    context->setLogging(logging);

    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");
    shared_ptr<JobOrder> jobOrder = JobOrderParser().parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.0.xml");

    context->setDictionary(dictionary);
    context->setJobOrder(jobOrder);
}

void ColTest::tearDown() {
}

void ColTest::testAddSlstrVariables() {
    context->addSegment(Constants::SEGMENT_OLC, 10, 10, 5, 0, 9);
    col->addTargetSegment(*context);
    Segment& collocatedSegment = context->getSegment(Constants::SEGMENT_SYN_COLLOCATED);

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
    col->addTargetSegment(*context);
    Segment& collocatedSegment = context->getSegment(Constants::SEGMENT_SYN_COLLOCATED);
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

void ColTest::testRetrieveDeltaVariableName() {
    Col col;

    string varName = "OLC_flags";
    string variableName = col.retrieveDeltaVariableName(varName, Col::AXIS_COL);
    CPPUNIT_ASSERT(variableName.compare("delta_x_1") == 0);

    variableName = col.retrieveDeltaVariableName(varName, Col::AXIS_ROW);
    CPPUNIT_ASSERT(variableName.compare("delta_y_1") == 0);

    varName = "L_18";
    variableName = col.retrieveDeltaVariableName(varName, Col::AXIS_COL);
    CPPUNIT_ASSERT(variableName.compare("delta_x_18") == 0);

    varName = "L_1";
    variableName = col.retrieveDeltaVariableName(varName, Col::AXIS_ROW);
    CPPUNIT_ASSERT(variableName.compare("delta_y_1") == 0);

    varName = "L_18_er";
    variableName = col.retrieveDeltaVariableName(varName, Col::AXIS_COL);
    CPPUNIT_ASSERT(variableName.compare("delta_x_18") == 0);

    varName = "longitude";
    variableName = col.retrieveDeltaVariableName(varName, Col::AXIS_COL);
    CPPUNIT_ASSERT(variableName.compare("delta_x_1") == 0);

    varName = "latitude";
    variableName = col.retrieveDeltaVariableName(varName, Col::AXIS_COL);
    CPPUNIT_ASSERT(variableName.compare("delta_x_1") == 0);

    varName = "altitude";
    variableName = col.retrieveDeltaVariableName(varName, Col::AXIS_COL);
    CPPUNIT_ASSERT(variableName.compare("delta_x_1") == 0);

    varName = "L_1_er";
    variableName = col.retrieveDeltaVariableName(varName, Col::AXIS_ROW);
    CPPUNIT_ASSERT(variableName.compare("delta_y_1") == 0);

    varName = "L_19";
    CPPUNIT_ASSERT_THROW(col.retrieveDeltaVariableName(varName, Col::AXIS_COL), logic_error);

    varName = "L_0";
    CPPUNIT_ASSERT_THROW(col.retrieveDeltaVariableName(varName, Col::AXIS_COL), logic_error);
}

void ColTest::testRetrievePositionVariableName() {
    Col col;

    string varName = "SLN_confidence";
    string variableName = col.retrievePositionVariableName(varName, Col::AXIS_COL);
    CPPUNIT_ASSERT(variableName.compare("x_corr_1") == 0);

    varName = "L_19";
    variableName = col.retrievePositionVariableName(varName, Col::AXIS_COL);
    CPPUNIT_ASSERT(variableName.compare("x_corr_1") == 0);

    varName = "L_20";
    variableName = col.retrievePositionVariableName(varName, Col::AXIS_COL);
    CPPUNIT_ASSERT(variableName.compare("x_corr_2") == 0);

    varName = "L_19";
    variableName = col.retrievePositionVariableName(varName, Col::AXIS_ROW);
    CPPUNIT_ASSERT(variableName.compare("y_corr_1") == 0);

    varName = "L_20";
    variableName = col.retrievePositionVariableName(varName, Col::AXIS_ROW);
    CPPUNIT_ASSERT(variableName.compare("y_corr_2") == 0);

    varName = "L_24";
    variableName = col.retrievePositionVariableName(varName, Col::AXIS_COL);
    CPPUNIT_ASSERT(variableName.compare("x_corr_6") == 0);

    varName = "L_24";
    variableName = col.retrievePositionVariableName(varName, Col::AXIS_ROW);
    CPPUNIT_ASSERT(variableName.compare("y_corr_6") == 0);

    varName = "L_19_er";
    variableName = col.retrievePositionVariableName(varName, Col::AXIS_COL);
    CPPUNIT_ASSERT(variableName.compare("x_corr_1") == 0);

    varName = "L_24_er";
    variableName = col.retrievePositionVariableName(varName, Col::AXIS_ROW);
    CPPUNIT_ASSERT(variableName.compare("y_corr_6") == 0);

    varName = "L_25";
    variableName = col.retrievePositionVariableName(varName, Col::AXIS_COL);
    CPPUNIT_ASSERT(variableName.compare("x_corr_o") == 0);

    varName = "L_25";
    variableName = col.retrievePositionVariableName(varName, Col::AXIS_ROW);
    CPPUNIT_ASSERT(variableName.compare("y_corr_o") == 0);

    varName = "L_25_er";
    variableName = col.retrievePositionVariableName(varName, Col::AXIS_COL);
    CPPUNIT_ASSERT(variableName.compare("x_corr_o") == 0);

    varName = "L_25_er";
    variableName = col.retrievePositionVariableName(varName, Col::AXIS_ROW);
    CPPUNIT_ASSERT(variableName.compare("y_corr_o") == 0);

    varName = "L_30";
    variableName = col.retrievePositionVariableName(varName, Col::AXIS_COL);
    CPPUNIT_ASSERT(variableName.compare("x_corr_o") == 0);

    varName = "L_30_er";
    variableName = col.retrievePositionVariableName(varName, Col::AXIS_ROW);
    CPPUNIT_ASSERT(variableName.compare("y_corr_o") == 0);

    varName = "L_18";
    CPPUNIT_ASSERT_THROW(col.retrievePositionVariableName(varName, Col::AXIS_COL), logic_error);

    varName = "L_31";
    CPPUNIT_ASSERT_THROW(col.retrievePositionVariableName(varName, Col::AXIS_COL), logic_error);
}



void ColTest::testCol() {
    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> writer = shared_ptr<Module>(new SynL2Writer());

    context->addModule(reader);
    context->addModule(col);
    context->addModule(writer);

	Processor processor;
	processor.process(*context);
}
