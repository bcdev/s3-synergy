/* 
 * File:   VgtP_1_Test.cpp
 * Author: thomasstorm
 * 
 * Created on January 24, 2012
 */

#include "../../../../src/main/cpp/core/ExitCode.h"
#include "../../../../src/main/cpp/util/BasicTask.h"

#include "VgtP_1_Test.h"

CPPUNIT_TEST_SUITE_REGISTRATION(VgtP_1_Test);

VgtP_1_Test::VgtP_1_Test() {
}

VgtP_1_Test::~VgtP_1_Test() {
}

void VgtP_1_Test::setUp() {
}

void VgtP_1_Test::tearDown() {
}

void VgtP_1_Test::testParseJobOrder_01() {
	BasicTask task("SY_INT_VGTP_1_01");

    const string jobOrderPath = Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_INT_VGTP_1_01.xml";

	const int exitCode = task.execute(jobOrderPath);

	CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}

void VgtP_1_Test::testParseJobOrder_02() {
	BasicTask task("SY_INT_VGTP_1_02");

    const string jobOrderPath = Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_INT_VGTP_1_02.xml";

	const int exitCode = task.execute(jobOrderPath);

	CPPUNIT_ASSERT(exitCode == ExitCode::FAILURE);
}
