/* 
 * File:   AerTest.cpp
 * Author: thomasstorm
 * 
 * Created on Oct 04, 2011
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/modules/Aer.h"
#include "../../../../src/main/cpp/modules/Ave.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Pcl.h"
#include "../../../../src/main/cpp/writer/SynL2Writer.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"


#include "AerTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(AerTest);

AerTest::AerTest() {
}

AerTest::~AerTest() {
}

void AerTest::setUp() {
    XPathInitializer init;
    prepareContext();
    aer = shared_ptr<Aer>(new Aer());
}

void AerTest::prepareContext() {
    context = shared_ptr<Context>(new Context());
    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
    shared_ptr<JobOrderParser> jobOrderParser = shared_ptr<JobOrderParser>(new JobOrderParser());
    shared_ptr<JobOrder> jobOrder = jobOrderParser->parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_AVE.xml");
    context->setJobOrder(jobOrder);

    shared_ptr<Logging> logging = jobOrderParser->createLogging("LOG.SY_UNT_AVE");
    context->setLogging(logging);


    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");

    context->setDictionary(dictionary);
    context->setJobOrder(jobOrder);
}

void AerTest::tearDown() {
}

void AerTest::testNdv() {
    Segment& segment = context->addSegment(Constants::SEGMENT_SYN_AVERAGED, 100, 10, 2, 0, 583);
    segment.addVariable("L_9", Constants::TYPE_DOUBLE, 1.0, 0.0);
    segment.addVariable("L_17", Constants::TYPE_DOUBLE, 1.0, 0.0);
    AerPixelDerived p(segment, 0, 0, 0);
    p.solarIrradiances[9] = 8;
    p.solarIrradiances[17] = 12;
    p.solarIrradianceFillValues[9] = -1;
    p.solarIrradianceFillValues[17] = -1;
    double ndv = aer->ndv(p);
    CPPUNIT_ASSERT(std::abs(ndv - 0.1428571) < 1.e-5);

    p.solarIrradianceFillValues[9] = 8;
    ndv = aer->ndv(p);
    CPPUNIT_ASSERT(ndv == 0.5);
}

void AerTest::testAer() {
    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> col = shared_ptr<Module>(new Col());
    shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
    shared_ptr<Module> ave = shared_ptr<Module>(new Ave());
    shared_ptr<Module> writer = shared_ptr<Module>(new SynL2Writer());

    context->addModule(reader);
    context->addModule(col);
    context->addModule(pcl);
    context->addModule(ave);
    context->addModule(aer);
    context->addModule(writer);

	Processor processor;
	processor.process(*context);
}
