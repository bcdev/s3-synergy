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
#include "../../../main/cpp/util/BasicTask.h"
#include "../../../main/cpp/writer/ManifestWriter.h"
#include "../../../main/cpp/writer/SynL2Writer.h"

#include "SynL2SegmentProvider.h"
#include "SynL2WriterTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(SynL2WriterTest);

SynL2WriterTest::SynL2WriterTest() {
}

SynL2WriterTest::~SynL2WriterTest() {
}

void SynL2WriterTest::setUp() {
}

void SynL2WriterTest::tearDown() {
}

void SynL2WriterTest::testWriter() {
    BasicTask task("SY_UNT_SWR");
    shared_ptr<Module> module = shared_ptr<Module>(new SynL2SegmentProvider());
    shared_ptr<Module> writer = shared_ptr<Module>(new SynL2Writer());
    shared_ptr<Module> manifestWriter = shared_ptr<Module>(new ManifestWriter(Constants::PRODUCT_SY2));
    task.getContext().addModule(module);
    task.getContext().addModule(writer);
    task.getContext().addModule(manifestWriter);

    const int exitCode = task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_SWR.xml");
    CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}
