/* 
 * File:   AveTest.cpp
 * Author: thomasstorm
 * 
 * Created on Sep 22, 2011
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/ExitCode.h"
#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/modules/Ave.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Pcl.h"
#include "../../../../src/main/cpp/writer/SegmentWriter.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"
#include "../../../../src/main/cpp/util/BasicTask.h"

#include "AveTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(AveTest);

AveTest::AveTest() {
}

AveTest::~AveTest() {
}

void AveTest::setUp() {
}

void AveTest::tearDown() {
}

void AveTest::testIsRadianceName() {
    CPPUNIT_ASSERT(Ave::isRadianceName("L_1"));
    CPPUNIT_ASSERT(Ave::isRadianceName("L_1_er"));
    CPPUNIT_ASSERT(Ave::isRadianceName("L_10"));
    CPPUNIT_ASSERT(Ave::isRadianceName("L_10_er"));
    CPPUNIT_ASSERT(Ave::isRadianceName("L_18_er"));
    CPPUNIT_ASSERT(!Ave::isRadianceName("L_19_exception"));
    CPPUNIT_ASSERT(!Ave::isRadianceName("L_30_exception"));
    CPPUNIT_ASSERT(Ave::isRadianceName("L_30"));
    CPPUNIT_ASSERT(!Ave::isRadianceName("SYN_flags"));
}

void AveTest::testAve() {
    BasicTask task("SY_UNT_AVE");

    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> col = shared_ptr<Module>(new Col());
    shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
    shared_ptr<Module> ave = shared_ptr<Module>(new Ave());
    shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

    task.getContext().addModule(reader);
    task.getContext().addModule(col);
    task.getContext().addModule(pcl);
    task.getContext().addModule(ave);
    task.getContext().addModule(writer);

    const int exitCode = task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_AVE.xml");
    CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}
