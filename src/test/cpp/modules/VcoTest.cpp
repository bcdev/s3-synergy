/* 
 * File:   VcoTest.cpp
 * Author: thomasstorm
 * 
 * Created on Dec 01, 2011
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/SwathSegment.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Pcl.h"
#include "../../../../src/main/cpp/modules/Vbm.h"
#include "../../../../src/main/cpp/modules/Vfl.h"
#include "../../../../src/main/cpp/modules/Vac.h"
#include "../../../../src/main/cpp/modules/Vco.h"
#include "../../../../src/main/cpp/util/BasicTask.h"
#include "../../../../src/main/cpp/writer/SegmentWriter.h"

#include "VcoTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(VcoTest);

VcoTest::VcoTest() {
}

VcoTest::~VcoTest() {
}

void VcoTest::setUp() {
}

void VcoTest::tearDown() {
}

void VcoTest::testVco() {
	BasicTask task("SY_UNT_VCO");

	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
	shared_ptr<Module> vbm = shared_ptr<Module>(new Vbm());
	shared_ptr<Module> vfl = shared_ptr<Module>(new Vfl());
	shared_ptr<Module> vac = shared_ptr<Module>(new Vac());
	shared_ptr<Module> vco = shared_ptr<Module>(new Vco());
	shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

	task.getContext().addModule(reader);
	task.getContext().addModule(col);
	task.getContext().addModule(pcl);
	task.getContext().addModule(vbm);
	task.getContext().addModule(vfl);
	task.getContext().addModule(vac);
	task.getContext().addModule(vco);
	task.getContext().addModule(writer);

	task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_VCO.xml");
}

void VcoTest::testTimeConversion() {

}
