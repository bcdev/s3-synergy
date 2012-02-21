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
#include "../../../main/cpp/writer/VgtWriter.h"

#include "VgtSegmentProvider.h"
#include "VgtWriterTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(VgtWriterTest);

VgtWriterTest::VgtWriterTest() {
}

VgtWriterTest::~VgtWriterTest() {
}

void VgtWriterTest::setUp() {
}

void VgtWriterTest::tearDown() {
}

void VgtWriterTest::testWriter() {
    BasicTask task("SY_UNT_VWR");
    shared_ptr<Module> module = shared_ptr<Module>(new VgtSegmentProvider());
    shared_ptr<Module> writer = shared_ptr<Module>(new VgtWriter(Constants::PRODUCT_VGP));
    shared_ptr<Module> manifestWriter = shared_ptr<Module>(new ManifestWriter(Constants::PRODUCT_VGP));
    task.getContext().addModule(module);
    task.getContext().addModule(writer);
    task.getContext().addModule(manifestWriter);

    const int exitCode = task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_VWR.xml");
    CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}
