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

#include "../../../../src/main/cpp/util/BasicTask.h"
#include "../../../../src/main/cpp/writer/SegmentWriter.h"

#include "MapSegmentProvider.h"
#include "SegmentWriterTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(SegmentWriterTest);

SegmentWriterTest::SegmentWriterTest() {
}

SegmentWriterTest::~SegmentWriterTest() {
}

void SegmentWriterTest::setUp() {
}

void SegmentWriterTest::tearDown() {
}

void SegmentWriterTest::testWriter() {
	BasicTask task("SY_UNT_GEN");

	shared_ptr<Module> reader = shared_ptr<Module>(new MapSegmentProvider());
	shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

	task.getContext().addModule(reader);
	task.getContext().addModule(writer);

	task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_GEN.xml");
}
