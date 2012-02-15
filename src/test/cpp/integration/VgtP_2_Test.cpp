/* 
 * File:   VgtP_2_Test.cpp
 * Author: thomasstorm
 * 
 * Created on January 24, 2012
 */

#include "../../../../src/main/cpp/core/ExitCode.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Pcl.h"
#include "../../../../src/main/cpp/modules/Vbm.h"
#include "../../../../src/main/cpp/modules/Vfl.h"
#include "../../../../src/main/cpp/modules/Vpr.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"
#include "../../../../src/main/cpp/util/BasicTask.h"
#include "../../../../src/main/cpp/writer/VgtWriter.h"
#include "../../../../src/main/cpp/writer/ManifestWriter.h"

#include "VgtP_2_Test.h"

CPPUNIT_TEST_SUITE_REGISTRATION(VgtP_2_Test);

VgtP_2_Test::VgtP_2_Test() {
}

VgtP_2_Test::~VgtP_2_Test() {
}

void VgtP_2_Test::setUp() {
}

void VgtP_2_Test::tearDown() {
}

void VgtP_2_Test::testIntegrationOfVgtPModules() {
	BasicTask task("SY_INT_VGTP_2");

	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
	shared_ptr<Module> vbm = shared_ptr<Module>(new Vbm());
	shared_ptr<Module> vfl = shared_ptr<Module>(new Vfl());
	shared_ptr<Module> vpr = shared_ptr<Module>(new Vpr());
	shared_ptr<Module> writer = shared_ptr<Module>(new VgtWriter());
	shared_ptr<Module> manifestWriter = shared_ptr<Module>(new ManifestWriter(Constants::PRODUCT_VGP));

	task.getContext().addModule(reader);
	task.getContext().addModule(col);
	task.getContext().addModule(pcl);
	task.getContext().addModule(vbm);
	task.getContext().addModule(vfl);
	task.getContext().addModule(vpr);
	task.getContext().addModule(writer);
	task.getContext().addModule(manifestWriter);

	const int exitCode = task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_INT_VGTP_2.xml");

	CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}
