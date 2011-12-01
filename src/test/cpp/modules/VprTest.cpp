/* 
 * File:   VprTest.cpp
 * Author: thomasstorm
 * 
 * Created on Dec 01, 2011
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"
#include "../../../../src/main/cpp/core/Pixel.h"
#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Vbm.h"
#include "../../../../src/main/cpp/writer/SegmentWriter.h"


#include "VprTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(VprTest);

VprTest::VprTest() {
}

VprTest::~VprTest() {
}

void VprTest::setUp() {
    XPathInitializer init;
    prepareContext();
}

void VprTest::prepareContext() {
    context = shared_ptr<Context>(new Context());
    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
    shared_ptr<JobOrderParser> jobOrderParser = shared_ptr<JobOrderParser>(new JobOrderParser());
    shared_ptr<JobOrder> jobOrder = jobOrderParser->parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_VPR.xml");
    context->setJobOrder(jobOrder);

    shared_ptr<Logging> logging = jobOrderParser->createLogging("LOG.SY_UNT_VPR");
    context->setLogging(logging);


    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");

    context->setDictionary(dictionary);
    context->setJobOrder(jobOrder);
}

void VprTest::tearDown() {
}

void VprTest::testGetLatitude() {
    CPPUNIT_ASSERT(std::abs(90 - Vpr::getLatitude(0)) < 0.001);
    CPPUNIT_ASSERT(std::abs(89.1071 - Vpr::getLatitude(100)) < 0.001);
    CPPUNIT_ASSERT(std::abs(0 - Vpr::getLatitude(10080)) < 0.001);
    CPPUNIT_ASSERT(std::abs(-89.991 - Vpr::getLatitude(20159)) < 0.001);

    CPPUNIT_ASSERT_THROW(Vpr::getLatitude(20160), std::invalid_argument);
    CPPUNIT_ASSERT_THROW(Vpr::getLatitude(-1), std::invalid_argument);
}

void VprTest::testGetLongitude() {
    CPPUNIT_ASSERT(std::abs(-180 - Vpr::getLongitude(0)) < 0.001);
    CPPUNIT_ASSERT(std::abs(-179.1071 - Vpr::getLongitude(100)) < 0.001);
    CPPUNIT_ASSERT(std::abs(0 - Vpr::getLongitude(20160)) < 0.001);
    CPPUNIT_ASSERT(std::abs(179.9911 - Vpr::getLongitude(40319)) < 0.001);

    CPPUNIT_ASSERT_THROW(Vpr::getLongitude(40320), std::invalid_argument);
    CPPUNIT_ASSERT_THROW(Vpr::getLongitude(-1), std::invalid_argument);
}
