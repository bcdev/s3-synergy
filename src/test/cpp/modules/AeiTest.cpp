/* 
 * File:   AeiTest.cpp
 * Author: thomasstorm
 * 
 * Created on Nov 01, 2011
 */

#include <algorithm>
#include <cstdlib>

#include "../../../main/cpp/core/ExitCode.h"
#include "../../../main/cpp/reader/SynL1Reader.h"
#include "../../../main/cpp/modules/Aei.h"
#include "../../../main/cpp/modules/Aer.h"
#include "../../../main/cpp/modules/Ave.h"
#include "../../../main/cpp/modules/Col.h"
#include "../../../main/cpp/modules/Pcl.h"
#include "../../../main/cpp/writer/SegmentWriter.h"
#include "../../../main/cpp/util/BasicTask.h"

#include "AeiTest.h"

using std::copy;
using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(AeiTest);

AeiTest::AeiTest() {
}

AeiTest::~AeiTest() {
}

void AeiTest::setUp() {
}


void AeiTest::tearDown() {
}

void AeiTest::testAei() {
    BasicTask task("SY_UNT_AEI");

    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> col = shared_ptr<Module>(new Col());
    shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
    shared_ptr<Module> ave = shared_ptr<Module>(new Ave());
    shared_ptr<Module> aer = shared_ptr<Module>(new Aer());
    shared_ptr<Module> aei = shared_ptr<Aei>(new Aei());
    shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

    task.getContext().addModule(reader);
    task.getContext().addModule(col);
    task.getContext().addModule(pcl);
    task.getContext().addModule(ave);
    task.getContext().addModule(aer);
    task.getContext().addModule(aei);
    task.getContext().addModule(writer);

    const int exitCode = task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_AEI.xml");
    CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}
