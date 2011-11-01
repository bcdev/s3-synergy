/* 
 * File:   AeiTest.cpp
 * Author: thomasstorm
 * 
 * Created on Nov 01, 2011
 */

#include <algorithm>
#include <cstdlib>

#include "../../../main/cpp/core/Processor.h"
#include "../../../main/cpp/core/Context.h"
#include "../../../main/cpp/reader/SynL1Reader.h"
#include "../../../main/cpp/modules/Aer.h"
#include "../../../main/cpp/modules/Ave.h"
#include "../../../main/cpp/modules/Col.h"
#include "../../../main/cpp/modules/Pcl.h"
#include "../../../main/cpp/writer/SegmentWriter.h"
#include "../../../main/cpp/util/DictionaryParser.h"
#include "../../../main/cpp/util/JobOrderParser.h"
#include "../../../main/cpp/core/Pixel.h"
#include "../../../main/cpp/util/ErrorMetric.h"


#include "AeiTest.h"

using std::copy;
using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(AeiTest);

AeiTest::AeiTest() {
}

AeiTest::~AeiTest() {
}

void AeiTest::setUp() {
    XPathInitializer init;
    prepareContext();
    aei = shared_ptr<Aei>(new Aei());
}

void AeiTest::prepareContext() {
    context = shared_ptr<Context>(new Context());
    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
    shared_ptr<JobOrderParser> jobOrderParser = shared_ptr<JobOrderParser>(new JobOrderParser());
    shared_ptr<JobOrder> jobOrder = jobOrderParser->parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_AEI.xml");
    context->setJobOrder(jobOrder);

    shared_ptr<Logging> logging = jobOrderParser->createLogging("LOG.SY_UNT_AEI");
    context->setLogging(logging);

    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");

    context->setDictionary(dictionary);
    context->setJobOrder(jobOrder);
}

void AeiTest::tearDown() {
	context.reset();
}

void AeiTest::testComputeWeights() {
    valarray<double> weights(4);
    aei->averagingFactor = 8;
    aei->computeWeights(10, 5, 8, 20, 10, 16, weights);
    CPPUNIT_ASSERT(weights[0] == 0.90234375f);
    CPPUNIT_ASSERT(weights[1] == -0.38671875f);
    CPPUNIT_ASSERT(weights[2] == -0.41015625f);
    CPPUNIT_ASSERT(weights[3] == 0.17578125f);


}
