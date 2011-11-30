/* 
 * File:   VflTest.cpp
 * Author: thomasstorm
 * 
 * Created on Nov 17, 2011
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"
#include "../../../../src/main/cpp/core/Pixel.h"
#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/writer/SegmentWriter.h"


#include "VflTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(VflTest);

VflTest::VflTest() {
}

VflTest::~VflTest() {
}

void VflTest::setUp() {
    XPathInitializer init;
    prepareContext();
    vbm = shared_ptr<Vfl>(new Vfl());
}

void VflTest::prepareContext() {
    context = shared_ptr<Context>(new Context());
    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
    shared_ptr<JobOrderParser> jobOrderParser = shared_ptr<JobOrderParser>(new JobOrderParser());
    shared_ptr<JobOrder> jobOrder = jobOrderParser->parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_VFL.xml");
    context->setJobOrder(jobOrder);

    shared_ptr<Logging> logging = jobOrderParser->createLogging("LOG.SY_UNT_VBM");
    context->setLogging(logging);


    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");

    context->setDictionary(dictionary);
    context->setJobOrder(jobOrder);
}

void VflTest::tearDown() {
}