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
#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/writer/SegmentWriter.h"


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
    double t550 = Vbm::aerosolOpticalThickness(lat);
    CPPUNIT_ASSERT(std::abs(t550 - 0.05000026692696688018) < 0.0001);
}

void VbmTest::testSurfaceReflectance() {
    const double surfRefl = Vbm::surfaceReflectance(0.4, 85.9, 66.34, 0.34, 0.68, 0.21, 0.2, 0.3, 0.3, 0.3);
    CPPUNIT_ASSERT(std::abs(surfRefl - 73.81938710116) < 0.0001);
}

void VbmTest::testHyperspectralUpscale() {
    double M = 0.5 * (1 / std::cos(64.3) + 1 / (std::cos(68.2)));
    const double result = vbm->toaReflectance(0.3, M, 0.5, 0.2, 0.1, 0.4, 0.8, 0.9);
    CPPUNIT_ASSERT(std::abs(result - 0.85244299269730513062) < 0.0001);
}

void VbmTest::testVbmIntegration() {
    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> col = shared_ptr<Col>(new Col());
    shared_ptr<Module> vbm = shared_ptr<Vbm>(new Vbm());
    shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

    context->addModule(reader);
    context->addModule(col);
    context->addModule(vbm);
    context->addModule(writer);

    Processor processor;
    processor.process(*context);
}
