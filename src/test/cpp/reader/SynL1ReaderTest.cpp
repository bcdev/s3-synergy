/* 
 * File:   L1cReaderTest.cpp
 * Author: ralf
 * 
 * Created on August 16, 2011, 1:36 PM
 */

#include <cstdlib>


#include "../../../main/cpp/core/ExitCode.h"
#include "../../../main/cpp/reader/SynL1Reader.h"
#include "../../../main/cpp/util/BasicTask.h"
#include "../../../main/cpp/writer/SegmentWriter.h"

#include "SynL1ReaderTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(SynL1ReaderTest);

SynL1ReaderTest::SynL1ReaderTest() {
}

SynL1ReaderTest::~SynL1ReaderTest() {
}

void SynL1ReaderTest::setUp() {
}

void SynL1ReaderTest::tearDown() {
}

void SynL1ReaderTest::testReader() {
    BasicTask task("SY_UNT_SRE");
    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());
    task.getContext().addModule(reader);
    task.getContext().addModule(writer);

    const int exitCode = task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_SRE.xml");
    CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}
