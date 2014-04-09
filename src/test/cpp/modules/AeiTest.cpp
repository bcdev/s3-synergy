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

#include <algorithm>
#include <cstdlib>

#include "../../../main/cpp/core/ExitCode.h"
#include "../../../main/cpp/reader/SynL1Reader.h"
#include "../../../main/cpp/modules/Aei.h"
#include "../../../main/cpp/modules/Aer.h"
#include "../../../main/cpp/modules/Ave.h"
#include "../../../main/cpp/modules/Col.h"
#include "../../../main/cpp/modules/Pcl.h"
#include "../../../main/cpp/writer/SegmentWriter.h"
#include "../../../main/cpp/util/BasicTask.h"

#include "AeiTest.h"

using std::copy;
using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(AeiTest);

AeiTest::AeiTest() {
}

AeiTest::~AeiTest() {
}

void AeiTest::setUp() {
}


void AeiTest::tearDown() {
}

void AeiTest::testAei() {
    BasicTask task("SY_UNT_AEI");

    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> col = shared_ptr<Module>(new Col());
    shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
    shared_ptr<Module> ave = shared_ptr<Module>(new Ave());
    shared_ptr<Module> aer = shared_ptr<Module>(new Aer());
    shared_ptr<Module> aei = shared_ptr<Aei>(new Aei());
    shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

    task.getContext().addModule(reader);
    task.getContext().addModule(col);
    task.getContext().addModule(pcl);
    task.getContext().addModule(ave);
    task.getContext().addModule(aer);
    task.getContext().addModule(aei);
    task.getContext().addModule(writer);

    const int exitCode = task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_AEI.xml");
    CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}

void AeiTest::testAei2() {
    BasicTask task("SY_UNT_AEI_02");

    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> col = shared_ptr<Module>(new Col());
    shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
    shared_ptr<Module> ave = shared_ptr<Module>(new Ave());
    shared_ptr<Module> aer = shared_ptr<Module>(new Aer());
    shared_ptr<Module> aei = shared_ptr<Aei>(new Aei());
    shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

    task.getContext().addModule(reader);
    task.getContext().addModule(col);
    task.getContext().addModule(pcl);
    task.getContext().addModule(ave);
    task.getContext().addModule(aer);
    task.getContext().addModule(aei);
    task.getContext().addModule(writer);

    const int exitCode = task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_AEI_02.xml");
    CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}
