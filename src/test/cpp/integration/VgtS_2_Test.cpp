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
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Pcl.h"
#include "../../../../src/main/cpp/modules/Vbm.h"
#include "../../../../src/main/cpp/modules/Vfl.h"
#include "../../../../src/main/cpp/modules/Vac.h"
#include "../../../../src/main/cpp/modules/Vco.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"
#include "../../../../src/main/cpp/util/BasicTask.h"
#include "../../../../src/main/cpp/writer/VgtWriter.h"
#include "../../../../src/main/cpp/writer/ManifestWriter.h"

#include "VgtS_2_Test.h"

CPPUNIT_TEST_SUITE_REGISTRATION(VgtS_2_Test);

VgtS_2_Test::VgtS_2_Test() {
}

VgtS_2_Test::~VgtS_2_Test() {
}

void VgtS_2_Test::setUp() {
}

void VgtS_2_Test::tearDown() {
}

void VgtS_2_Test::testIntegrationOfVgtSModules() {
	BasicTask task("SY_INT_VGTS_2");

	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
	shared_ptr<Module> vbm = shared_ptr<Module>(new Vbm());
	shared_ptr<Module> vfl = shared_ptr<Module>(new Vfl());
	shared_ptr<Module> vac = shared_ptr<Module>(new Vac());
	shared_ptr<Module> vco = shared_ptr<Module>(new Vco());
	shared_ptr<Module> writer = shared_ptr<Module>(new VgtWriter(Constants::PRODUCT_VGS));
	shared_ptr<Module> manifestWriter = shared_ptr<Module>(new ManifestWriter(Constants::PRODUCT_VGS));

	task.getContext().addModule(reader);
	task.getContext().addModule(col);
	task.getContext().addModule(pcl);
	task.getContext().addModule(vbm);
	task.getContext().addModule(vfl);
	task.getContext().addModule(vac);
	task.getContext().addModule(vco);
	task.getContext().addModule(writer);
	task.getContext().addModule(manifestWriter);

	const int exitCode = task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_INT_VGTS_2.xml");

	CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}
