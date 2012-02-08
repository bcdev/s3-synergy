/* 
 * File:   AcoTest.cpp
 * Author: ralf
 * 
 * Created on August 16, 2011, 1:36 PM
 */

#include "../../../../src/main/cpp/core/ExitCode.h"
#include "../../../../src/main/cpp/modules/Aer.h"
#include "../../../../src/main/cpp/modules/Aei.h"
#include "../../../../src/main/cpp/modules/Ave.h"
#include "../../../../src/main/cpp/modules/Aco.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Pcl.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/util/BasicTask.h"
#include "../../../../src/main/cpp/writer/SegmentWriter.h"

#include "AcoTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(AcoTest);

AcoTest::AcoTest() {
}

AcoTest::~AcoTest() {
}

void AcoTest::setUp() {
}

void AcoTest::tearDown() {
}

void AcoTest::testAco() {
    BasicTask task("SY_UNT_ACO");

	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
	shared_ptr<Module> ave = shared_ptr<Module>(new Ave());
	shared_ptr<Module> aer = shared_ptr<Module>(new Aer());
	shared_ptr<Module> aei = shared_ptr<Module>(new Aei());
	shared_ptr<Module> aco = shared_ptr<Module>(new Aco());
	shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

	task.getContext().addModule(reader);
	task.getContext().addModule(col);
	task.getContext().addModule(pcl);
	task.getContext().addModule(ave);
	task.getContext().addModule(aer);
	task.getContext().addModule(aei);
	task.getContext().addModule(aco);
	task.getContext().addModule(writer);

	const int exitCode = task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_ACO.xml");
	CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}
