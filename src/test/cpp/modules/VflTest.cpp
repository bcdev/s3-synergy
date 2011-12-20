/* 
 * File:   VflTest.cpp
 * Author: thomasstorm
 * 
 * Created on Nov 17, 2011
 */

#include <cstdlib>

#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Pcl.h"
#include "../../../../src/main/cpp/modules/Vbm.h"
#include "../../../../src/main/cpp/modules/Vfl.h"
#include "../../../../src/main/cpp/util/BasicTask.h"
#include "../../../../src/main/cpp/writer/SegmentWriter.h"


#include "VflTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(VflTest);

VflTest::VflTest() {
}

VflTest::~VflTest() {
}

void VflTest::setUp() {
}

void VflTest::tearDown() {
}

void VflTest::testVfl() {
	BasicTask task("SY_UNT_VFL");

	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
	shared_ptr<Module> vbm = shared_ptr<Module>(new Vbm());
	shared_ptr<Module> vfl = shared_ptr<Module>(new Vfl());
	shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

	task.getContext().addModule(reader);
	task.getContext().addModule(col);
	task.getContext().addModule(pcl);
	task.getContext().addModule(vbm);
	task.getContext().addModule(vfl);
	task.getContext().addModule(writer);

    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
	task.execute(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_VFL.xml");
}
