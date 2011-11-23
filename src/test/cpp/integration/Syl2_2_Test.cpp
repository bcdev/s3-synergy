/* 
 * File:   AcoTest.cpp
 * Author: ralf
 * 
 * Created on August 16, 2011, 1:36 PM
 */

#include "../../../../src/main/cpp/modules/Aer.h"
#include "../../../../src/main/cpp/modules/Aei.h"
#include "../../../../src/main/cpp/modules/Ave.h"
#include "../../../../src/main/cpp/modules/Aco.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Pcl.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"
#include "../../../../src/main/cpp/writer/SynL2Writer.h"

#include "Syl2_2_Test.h"

CPPUNIT_TEST_SUITE_REGISTRATION(Syl2_2_Test);

Syl2_2_Test::Syl2_2_Test() {
}

Syl2_2_Test::~Syl2_2_Test() {
}

void Syl2_2_Test::setUp() {
}

void Syl2_2_Test::tearDown() {
}

void Syl2_2_Test::testIntegrationOfSynL2Modules() {
	BasicTask task("TEST");

	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
	shared_ptr<Module> ave = shared_ptr<Module>(new Ave());
	shared_ptr<Module> aer = shared_ptr<Module>(new Aer());
	shared_ptr<Module> aei = shared_ptr<Module>(new Aei());
	shared_ptr<Module> aco = shared_ptr<Module>(new Aco());
	shared_ptr<Module> writer = shared_ptr<Module>(new SynL2Writer());

	task.getContext().addModule(reader);
	task.getContext().addModule(col);
	task.getContext().addModule(pcl);
	task.getContext().addModule(ave);
	task.getContext().addModule(aer);
	task.getContext().addModule(aei);
	task.getContext().addModule(aco);
	task.getContext().addModule(writer);

    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
	task.execute(S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_INT_SYL2_2.xml");
}
