/* 
 * File:   PclTest.cpp
 * Author: thomasstorm
 * 
 * Created on Sep 14, 2011, 1:36 PM
 */

#include <cstdlib>

#include "../../../main/cpp/core/ExitCode.h"
#include "../../../main/cpp/reader/SynL1Reader.h"
#include "../../../main/cpp/modules/Col.h"
#include "../../../main/cpp/modules/Pcl.h"
#include "../../../main/cpp/writer/SegmentWriter.h"
#include "../../../main/cpp/util/BasicTask.h"

#include "PclTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(PclTest);

PclTest::PclTest() {
}

PclTest::~PclTest() {
}

void PclTest::setUp() {
}

void PclTest::tearDown() {
}

void PclTest::testPcl() {
    BasicTask task("SY_UNT_PCL");

    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
    shared_ptr<Module> col = shared_ptr<Module>(new Col());
    shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

    task.getContext().addModule(reader);
    task.getContext().addModule(col);
    task.getContext().addModule(pcl);
    task.getContext().addModule(writer);

    const int exitCode = task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_PCL.xml");
    CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}
