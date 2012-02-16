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

#include "../../../../src/main/cpp/core/ExitCode.h"
#include "../../../../src/main/cpp/modules/Aer.h"
#include "../../../../src/main/cpp/modules/Aei.h"
#include "../../../../src/main/cpp/modules/Ave.h"
#include "../../../../src/main/cpp/modules/Aco.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Pcl.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"
#include "../../../../src/main/cpp/util/BasicTask.h"
#include "../../../../src/main/cpp/writer/SynL2Writer.h"
#include "../../../../src/main/cpp/writer/ManifestWriter.h"

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
	BasicTask task("SY_INT_SYL2_2");

	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
	shared_ptr<Module> ave = shared_ptr<Module>(new Ave());
	shared_ptr<Module> aer = shared_ptr<Module>(new Aer());
	shared_ptr<Module> aei = shared_ptr<Module>(new Aei());
	shared_ptr<Module> aco = shared_ptr<Module>(new Aco());
	shared_ptr<Module> writer = shared_ptr<Module>(new SynL2Writer());
	shared_ptr<Module> manifestWriter = shared_ptr<Module>(new ManifestWriter(Constants::PRODUCT_SY2));

	task.getContext().addModule(reader);
	task.getContext().addModule(col);
	task.getContext().addModule(pcl);
	task.getContext().addModule(ave);
	task.getContext().addModule(aer);
	task.getContext().addModule(aei);
	task.getContext().addModule(aco);
	task.getContext().addModule(writer);
	task.getContext().addModule(manifestWriter);

	const int exitCode = task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_INT_SYL2_2.xml");

	CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}
