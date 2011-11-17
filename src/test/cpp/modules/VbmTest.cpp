/* 
 * File:   VbmTest.cpp
 * Author: thomasstorm
 * 
 * Created on Nov 17, 2011
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"
#include "../../../../src/main/cpp/core/Pixel.h"


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

void VbmTest::testComputeT550() {
    double lat = 53.2;
    double t550 = Vbm::computeT550(lat);
    CPPUNIT_ASSERT(std::abs(0.28031252522321136148 - t550) < 0.0001);
}

void VbmTest::testDownscaling() {
    Pixel p;
    valarray<double> result(24);
    uint16_t aerosolModelIndex = 22;
    p.waterVapour = 0.2;
    p.airPressure = 0.5;
    p.sza = 5.0;
    p.saa = 10.0;
    p.vaaOlc = 15.0;
    p.vzaOlc = 20.0;
    p.vaaSln = 25.0;
    p.vzaSln = 30.0;
    for(size_t i = 0; i < 18; i++) {
        p.radiances[i] = 10.0;
        p.solarIrradiances[i] = 20.0;
    }

    vbm->downscale(p, aerosolModelIndex, 0.5, result);
    CPPUNIT_ASSERT(result.size() == 24);
}

