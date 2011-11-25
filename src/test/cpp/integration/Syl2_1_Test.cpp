/* 
 * File:   AcoTest.cpp
 * Author: ralf
 * 
 * Created on August 16, 2011, 1:36 PM
 */

#include "../../../../src/main/cpp/core/ExitCode.h"

#include "../../../../src/main/cpp/util/BasicTask.h"

#include "Syl2_1_Test.h"

CPPUNIT_TEST_SUITE_REGISTRATION(Syl2_1_Test);

Syl2_1_Test::Syl2_1_Test() {
}

Syl2_1_Test::~Syl2_1_Test() {
}

void Syl2_1_Test::setUp() {
}

void Syl2_1_Test::tearDown() {
}

void Syl2_1_Test::testParseJobOrder_01() {
	BasicTask task("SY_INT_SYL2_2");

    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
    const string jobOrderPath = S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_INT_SYL2_1_01.xml";

	const int exitCode = task.execute(jobOrderPath);

	CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}

void Syl2_1_Test::testParseJobOrder_02() {
	BasicTask task("SY_INT_SYL2_2");

    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
    const string jobOrderPath = S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_INT_SYL2_1_02.xml";

	const int exitCode = task.execute(jobOrderPath);

	CPPUNIT_ASSERT(exitCode == ExitCode::FAILURE);
}
