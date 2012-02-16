/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
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

    const string jobOrderPath = Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_INT_SYL2_1_01.xml";

	const int exitCode = task.execute(jobOrderPath);

	CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}

void Syl2_1_Test::testParseJobOrder_02() {
	BasicTask task("SY_INT_SYL2_2");

    const string jobOrderPath = Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_INT_SYL2_1_02.xml";

	const int exitCode = task.execute(jobOrderPath);

	CPPUNIT_ASSERT(exitCode == ExitCode::FAILURE);
}
