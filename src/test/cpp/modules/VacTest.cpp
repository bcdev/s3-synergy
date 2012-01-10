/* 
 * File:   VacTest.cpp
 * Author: thomasstorm
 * 
 * Created on Jan 09, 2012
 */

#include <cstdlib>

#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Pcl.h"
#include "../../../../src/main/cpp/modules/Vbm.h"
#include "../../../../src/main/cpp/modules/Vac.h"
#include "../../../../src/main/cpp/util/BasicTask.h"
#include "../../../../src/main/cpp/writer/SegmentWriter.h"

#include "VacTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(VacTest);

VacTest::VacTest() {
}

VacTest::~VacTest() {
}

void VacTest::setUp() {
}

void VacTest::tearDown() {
}

void VacTest::testVac() {
	BasicTask task("SY_UNT_VAC");

	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
	shared_ptr<Module> vbm = shared_ptr<Module>(new Vbm());
	shared_ptr<Module> vac = shared_ptr<Module>(new Vac());
	shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

	task.getContext().addModule(reader);
	task.getContext().addModule(col);
	task.getContext().addModule(pcl);
	task.getContext().addModule(vbm);
	task.getContext().addModule(vac);
	task.getContext().addModule(writer);

	task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_VAC.xml");
}

