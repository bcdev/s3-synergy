/* 
 * File:   AerTest.cpp
 * Author: thomasstorm
 * 
 * Created on Oct 04, 2011
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


#include "AerTest.h"

using std::copy;
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
	context.reset();
}

void AerTest::testAer_s() {
    aer->readAuxdata(*context);

    Pixel p;
    p.aot = 0.5;
    p.flags = 0;
    p.lat = 57.10812;
    p.lon = -3.3295166;
    p.radiances[0] = 72.2334;
    p.radiances[1] = 96.2851;
    p.radiances[2] = 69.1;
    p.radiances[3] = 59.4;
    p.radiances[4] = 56.5;
    p.radiances[5] = 82.0;
    p.radiances[6] = 36.2;
    p.radiances[7] = 28.6;
    p.radiances[8] = 29.8;
    p.radiances[9] = 31.7;
    p.radiances[10] = 60.0;
    p.radiances[11] = 166.7;
    p.radiances[12] = 58.6;
    p.radiances[13] = 160.5;
    p.radiances[14] = 134.6;
    p.radiances[15] = 129.5;
    p.radiances[16] = 76.8;
    p.radiances[17] = 72.0;
    p.radiances[18] = 52.0;
    p.radiances[19] = 22.1;
    p.radiances[20] = 115.6;
    p.radiances[21] = 0;
    p.radiances[22] = 0;
    p.radiances[23] = 0;
    p.radiances[24] = Constants::FILL_VALUE_DOUBLE;
    p.radiances[25] = Constants::FILL_VALUE_DOUBLE;
    p.radiances[26] = Constants::FILL_VALUE_DOUBLE;
    p.radiances[27] = Constants::FILL_VALUE_DOUBLE;
    p.radiances[28] = Constants::FILL_VALUE_DOUBLE;
    p.radiances[29] = Constants::FILL_VALUE_DOUBLE;

    for(size_t i = 0; i < 18; i++) {
        p.solarIrradiances[i] = 1530.923;
    }
    for(size_t i = 18; i < 24; i++) {
        p.solarIrradiances[i] = 1802.648;
    }
    for(size_t i = 24; i < 30; i++) {
        p.solarIrradiances[i] = 1490.223;
    }

    p.sza = 44.426440;
    p.saa = 134.502528;
    p.vzaOlc = 56.402192;
    p.vaaOlc = 93.820264;
    p.vzaSln = 0;
    p.vaaSln = Constants::FILL_VALUE_DOUBLE;
    p.vzaSln = 0;
    p.vaaSlo = Constants::FILL_VALUE_DOUBLE;

    p.ozone = 0.006393726;
    p.airPressure = 1018.949;
    p.waterVapour = 0.2;

    p.aot = Constants::FILL_VALUE_DOUBLE;
    p.aotError = Constants::FILL_VALUE_DOUBLE;
    p.angstromExponent = Constants::FILL_VALUE_DOUBLE;
    p.aerosolModel = numeric_limits<short>::min();
    p.errorMetric = numeric_limits<double>::max();

	ErrorMetric em(*context);

	Pixel q;
    aer->retrieveAerosolProperties(p, q, em);
}

void AerTest::testAer() {
    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> col = shared_ptr<Module>(new Col());
    shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
    shared_ptr<Module> ave = shared_ptr<Module>(new Ave());
    shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

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
    const valarray<int16_t>& amins = *(aer->amins);
    CPPUNIT_ASSERT(amins.size() == 3);
    CPPUNIT_ASSERT(amins[0] == 1);
    CPPUNIT_ASSERT(amins[1] == 2);
    CPPUNIT_ASSERT(amins[2] == 3);

    double alpha550 = (*aer->aerosolAngstromExponents)[0];
    CPPUNIT_ASSERT(alpha550 == 1.25f);

    alpha550 = (*aer->aerosolAngstromExponents)[10];
    CPPUNIT_ASSERT(alpha550 == 1.25f);

    alpha550 = (*aer->aerosolAngstromExponents)[30];
    CPPUNIT_ASSERT(alpha550 == 1.25f);
}
