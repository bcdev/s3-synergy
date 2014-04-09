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

#include <cstdlib>

#include "../../../../src/main/cpp/core/ExitCode.h"
#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/modules/Ave.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Pcl.h"
#include "../../../../src/main/cpp/writer/SegmentWriter.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"
#include "../../../../src/main/cpp/util/BasicTask.h"

#include "AveTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(AveTest);

AveTest::AveTest() {
}

AveTest::~AveTest() {
}

void AveTest::setUp() {
}

void AveTest::tearDown() {
}

void AveTest::testIsRadianceName() {
    CPPUNIT_ASSERT(Ave::isRadianceName("L_1"));
    CPPUNIT_ASSERT(Ave::isRadianceName("L_1_er"));
    CPPUNIT_ASSERT(Ave::isRadianceName("L_10"));
    CPPUNIT_ASSERT(Ave::isRadianceName("L_10_er"));
    CPPUNIT_ASSERT(Ave::isRadianceName("L_18_er"));
    CPPUNIT_ASSERT(!Ave::isRadianceName("L_19_exception"));
    CPPUNIT_ASSERT(!Ave::isRadianceName("L_30_exception"));
    CPPUNIT_ASSERT(Ave::isRadianceName("L_30"));
    CPPUNIT_ASSERT(!Ave::isRadianceName("SYN_flags"));
}

void AveTest::testAve() {
    BasicTask task("SY_UNT_AVE");

    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> col = shared_ptr<Module>(new Col());
    shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
    shared_ptr<Module> ave = shared_ptr<Module>(new Ave());
    shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

    task.getContext().addModule(reader);
    task.getContext().addModule(col);
    task.getContext().addModule(pcl);
    task.getContext().addModule(ave);
    task.getContext().addModule(writer);

    const int exitCode = task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_AVE.xml");
    CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}

void AveTest::testAve2() {
    BasicTask task("SY_UNT_AVE_02");

    shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
    shared_ptr<Module> col = shared_ptr<Module>(new Col());
    shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
    shared_ptr<Module> ave = shared_ptr<Module>(new Ave());
    shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

    task.getContext().addModule(reader);
    task.getContext().addModule(col);
    task.getContext().addModule(pcl);
    task.getContext().addModule(ave);
    task.getContext().addModule(writer);

    const int exitCode = task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_AVE_02.xml");
    CPPUNIT_ASSERT(exitCode == ExitCode::OK);
}
