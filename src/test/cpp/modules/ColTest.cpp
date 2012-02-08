/* 
 * File:   ColTest.cpp
 * Author: thomasstorm
 * 
 * Created on Sep 15, 2011, 3:30 PM
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/ExitCode.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/writer/SegmentWriter.h"
#include "../../../../src/main/cpp/util/BasicTask.h"


#include "ColTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(ColTest);

ColTest::ColTest() {
}

ColTest::~ColTest() {
}

void ColTest::setUp() {
}

void ColTest::tearDown() {
}

void ColTest::testCol() {
    BasicTask task("SY_UNT_COL");

    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());
    shared_ptr<Module> col = shared_ptr<Col>(new Col());

    task.getContext().addModule(reader);
    task.getContext().addModule(col);
    task.getContext().addModule(writer);

    const int exitCode = task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_COL.xml");
    CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}
