/*
 * SegmentWriterTest.cpp
 *
 *  Created on: 23.01.2012
 *      Author: thomasstorm
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
