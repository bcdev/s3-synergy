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
