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

const double AerTest::EPSILON = 1.0E-6;

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
    shared_ptr<JobOrder> jobOrder = jobOrderParser->parse(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_AER.xml");
    context->setJobOrder(jobOrder);

    shared_ptr<Logging> logging = jobOrderParser->createLogging("LOG.SY_UNT_AER");
    context->setLogging(logging);


    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");

    context->setDictionary(dictionary);
    context->setJobOrder(jobOrder);
}

void AerTest::tearDown() {
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

void AerTest::testReadAuxdata() {
    aer->readAuxdata(*context);
    const valarray<int16_t> amins = aer->amins;
    CPPUNIT_ASSERT(amins[0] == 1);
    CPPUNIT_ASSERT(amins[20] == 21);
    CPPUNIT_ASSERT(amins[39] == 40);

    const float initialTau550 = aer->initialTau550;
    CPPUNIT_ASSERT(initialTau550 == 0.1f);

    const valarray<double> initialNus = aer->initialNu;
    CPPUNIT_ASSERT(initialNus.size() == 2);
    CPPUNIT_ASSERT(std::abs(initialNus[0] - 0.5) < EPSILON);
    CPPUNIT_ASSERT(std::abs(initialNus[1] - 0.3) < EPSILON);

    const valarray<double> initialOmegas = aer->initialOmega;
    CPPUNIT_ASSERT(initialOmegas.size() == 6);
    for(size_t i = 0; i < initialOmegas.size(); i++) {
        CPPUNIT_ASSERT(std::abs(initialOmegas[i] - 0.1) < EPSILON);
    }

    double alpha550 = aer->aerosolAngstromExponents[0];
    CPPUNIT_ASSERT(std::abs(alpha550 - 1.25) < EPSILON);

    alpha550 = aer->aerosolAngstromExponents[10];
    CPPUNIT_ASSERT(std::abs(alpha550 - 1.25) < EPSILON);

    alpha550 = aer->aerosolAngstromExponents[30];
    CPPUNIT_ASSERT(std::abs(alpha550 - 1.25) < EPSILON);

//    matrix<double> weights = aer->angularWeights;
//    CPPUNIT_ASSERT(weights.at_element(0,0) == 1.5);
//    CPPUNIT_ASSERT(weights.at_element(0,1) == 1.0);
//    CPPUNIT_ASSERT(weights.at_element(1,1) == 1.0);
//    CPPUNIT_ASSERT(weights.at_element(0,2) == 0.5);
//    CPPUNIT_ASSERT(weights.at_element(1,5) == 1.0);
}
