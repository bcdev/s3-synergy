/* 
 * File:   VbmTest.cpp
 * Author: thomasstorm
 * 
 * Created on Nov 17, 2011
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/modules/Ave.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Pcl.h"
#include "../../../../src/main/cpp/writer/SegmentWriter.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"


#include "VbmTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(VbmTest);

VbmTest::VbmTest() {
}

VbmTest::~VbmTest() {
}

void VbmTest::setUp() {
    XPathInitializer init;
    prepareContext();
    vbm = shared_ptr<Vbm>(new Vbm());
}

void VbmTest::prepareContext() {
    context = shared_ptr<Context>(new Context());
    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
    shared_ptr<JobOrderParser> jobOrderParser = shared_ptr<JobOrderParser>(new JobOrderParser());
    shared_ptr<JobOrder> jobOrder = jobOrderParser->parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_VBM.xml");
    context->setJobOrder(jobOrder);

    shared_ptr<Logging> logging = jobOrderParser->createLogging("LOG.SY_UNT_VBM");
    context->setLogging(logging);


    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");

    context->setDictionary(dictionary);
    context->setJobOrder(jobOrder);
}

void VbmTest::tearDown() {
}

void VbmTest::testDownscaling() {

}
