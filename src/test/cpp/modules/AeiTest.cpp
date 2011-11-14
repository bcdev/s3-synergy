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
#include "../../../main/cpp/modules/Aei.h"
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

void AeiTest::testAei() {
	using std::max;
	using std::min;

    const long sourceL0 = min(max(0L, (2000L - 8 / 2) / 8), 833L - 1L);
    CPPUNIT_ASSERT(sourceL0 == 249);

    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> col = shared_ptr<Module>(new Col());
    shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
    shared_ptr<Module> ave = shared_ptr<Module>(new Ave());
    shared_ptr<Module> aer = shared_ptr<Module>(new Aer());
    shared_ptr<Module> aei = shared_ptr<Aei>(new Aei());
    shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

    context->addModule(reader);
    context->addModule(col);
    context->addModule(pcl);
    context->addModule(ave);
    context->addModule(aer);
    context->addModule(aei);
    context->addModule(writer);

    Processor processor;
    processor.process(*context);
}
