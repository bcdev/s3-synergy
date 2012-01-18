/* 
 * File:   VprTest.cpp
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
#include "../../../../src/main/cpp/util/BasicTask.h"
#include "../../../../src/main/cpp/writer/SegmentWriter.h"

#include "VprTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(VprTest);

VprTest::VprTest() {
}

VprTest::~VprTest() {
}

void VprTest::setUp() {
}

void VprTest::tearDown() {
}

void VprTest::testMinMaxSynLat() {
    Vpr vpr;
    const shared_ptr<Segment> geoSegment = shared_ptr<Segment>(new SwathSegment(Constants::SEGMENT_GEO, 3, 2, 2, 0, 2));
    Accessor& latAccessor = geoSegment->addVariable("latitude", Constants::TYPE_DOUBLE);
    Accessor& lonAccessor = geoSegment->addVariable("longitude", Constants::TYPE_DOUBLE);
    vpr.latAccessor = &latAccessor;
    vpr.lonAccessor = &lonAccessor;

    const Grid& geoGrid = geoSegment->getGrid();
    //vpr.geoGrid = &geoGrid;

    latAccessor.setDouble(geoGrid.getIndex(0, 0, 0), 20);
    latAccessor.setDouble(geoGrid.getIndex(0, 0, 1), 21);
    latAccessor.setDouble(geoGrid.getIndex(0, 1, 0), 22);
    latAccessor.setDouble(geoGrid.getIndex(0, 1, 1), 23);
    latAccessor.setDouble(geoGrid.getIndex(0, 2, 0), 24);
    latAccessor.setDouble(geoGrid.getIndex(0, 2, 1), 25);
    latAccessor.setDouble(geoGrid.getIndex(1, 0, 0), 40);
    latAccessor.setDouble(geoGrid.getIndex(1, 0, 1), 41);
    latAccessor.setDouble(geoGrid.getIndex(1, 1, 0), 42);
    latAccessor.setDouble(geoGrid.getIndex(1, 1, 1), 43);
    latAccessor.setDouble(geoGrid.getIndex(1, 2, 0), 44);
    latAccessor.setDouble(geoGrid.getIndex(1, 2, 1), 45);

    double minLat = 91;
    double maxLat = -91;
    vpr.getMinMaxSourceLat(minLat, maxLat);
    CPPUNIT_ASSERT(std::abs(maxLat - 45) < 0.001);
    CPPUNIT_ASSERT(std::abs(minLat - 20) < 0.001);
}

void VprTest::testMinMaxSynLon() {
    Vpr vpr;
    const shared_ptr<Segment> geoSegment = shared_ptr<Segment>(new SwathSegment(Constants::SEGMENT_GEO, 3, 2, 2, 0, 2));
    Accessor& latAccessor = geoSegment->addVariable("latitude", Constants::TYPE_DOUBLE);
    Accessor& lonAccessor = geoSegment->addVariable("longitude", Constants::TYPE_DOUBLE);
    vpr.latAccessor = &latAccessor;
    vpr.lonAccessor = &lonAccessor;

    const Grid& geoGrid = geoSegment->getGrid();
    //vpr.geoGrid = &geoGrid;

    lonAccessor.setDouble(geoGrid.getIndex(0, 0, 0), 10);
    lonAccessor.setDouble(geoGrid.getIndex(0, 0, 1), 11);
    lonAccessor.setDouble(geoGrid.getIndex(0, 1, 0), 12);
    lonAccessor.setDouble(geoGrid.getIndex(0, 1, 1), 13);
    lonAccessor.setDouble(geoGrid.getIndex(0, 2, 0), 14);
    lonAccessor.setDouble(geoGrid.getIndex(0, 2, 1), 15);
    lonAccessor.setDouble(geoGrid.getIndex(1, 0, 0), 12);
    lonAccessor.setDouble(geoGrid.getIndex(1, 0, 1), 13);
    lonAccessor.setDouble(geoGrid.getIndex(1, 1, 0), 14);
    lonAccessor.setDouble(geoGrid.getIndex(1, 1, 1), 15);
    lonAccessor.setDouble(geoGrid.getIndex(1, 2, 0), 16);
    lonAccessor.setDouble(geoGrid.getIndex(1, 2, 1), 17);

    double minLon = 181;
    double maxLon = -181;
    vpr.getMinMaxSourceLon(minLon, maxLon);
    CPPUNIT_ASSERT(std::abs(maxLon - 17) < 0.001);
    CPPUNIT_ASSERT(std::abs(minLon - 10) < 0.001);
}

void VprTest::testMinMaxVgtLat() {
    Vpr vpr;
    Context context;
    Segment& vgtSegment = context.addSwathSegment(Constants::SEGMENT_VGP, 1000, vpr.COL_COUNT, 1, 0, vpr.LINE_COUNT - 1);
    vpr.vgpSegment = &vgtSegment;

    double minLat = 91;
    double maxLat = -91;
    //vpr.getMinMaxTargetLat(0, 999, minLat, maxLat);
    CPPUNIT_ASSERT(std::abs(maxLat - 75) < 0.001);
    CPPUNIT_ASSERT(std::abs(minLat - 66.0804) < 0.001);
}

void VprTest::testGetPixelPosInGrid() {
    Vpr vpr;
    const shared_ptr<Segment> geoSegment = shared_ptr<Segment>(new SwathSegment(Constants::SEGMENT_GEO, 3, 2, 2, 0, 2));
    Accessor& latAccessor = geoSegment->addVariable("latitude", Constants::TYPE_DOUBLE);
    Accessor& lonAccessor = geoSegment->addVariable("longitude", Constants::TYPE_DOUBLE);
    vpr.latAccessor = &latAccessor;
    vpr.lonAccessor = &lonAccessor;

    const Grid& geoGrid = geoSegment->getGrid();
    //vpr.geoGrid = &geoGrid;

    latAccessor.setDouble(geoGrid.getIndex(0, 0, 0), 20);
    latAccessor.setDouble(geoGrid.getIndex(0, 0, 1), 21);
    latAccessor.setDouble(geoGrid.getIndex(0, 1, 0), 22);
    latAccessor.setDouble(geoGrid.getIndex(0, 1, 1), 23);
    latAccessor.setDouble(geoGrid.getIndex(0, 2, 0), 24);
    latAccessor.setDouble(geoGrid.getIndex(0, 2, 1), 25);
    latAccessor.setDouble(geoGrid.getIndex(1, 0, 0), 40);
    latAccessor.setDouble(geoGrid.getIndex(1, 0, 1), 41);
    latAccessor.setDouble(geoGrid.getIndex(1, 1, 0), 42);
    latAccessor.setDouble(geoGrid.getIndex(1, 1, 1), 43);
    latAccessor.setDouble(geoGrid.getIndex(1, 2, 0), 44);
    latAccessor.setDouble(geoGrid.getIndex(1, 2, 1), 45);

    lonAccessor.setDouble(geoGrid.getIndex(0, 0, 0), 10);
    lonAccessor.setDouble(geoGrid.getIndex(0, 0, 1), 11);
    lonAccessor.setDouble(geoGrid.getIndex(0, 1, 0), 12);
    lonAccessor.setDouble(geoGrid.getIndex(0, 1, 1), 13);
    lonAccessor.setDouble(geoGrid.getIndex(0, 2, 0), 14);
    lonAccessor.setDouble(geoGrid.getIndex(0, 2, 1), 15);
    lonAccessor.setDouble(geoGrid.getIndex(1, 0, 0), 12);
    lonAccessor.setDouble(geoGrid.getIndex(1, 0, 1), 13);
    lonAccessor.setDouble(geoGrid.getIndex(1, 1, 0), 14);
    lonAccessor.setDouble(geoGrid.getIndex(1, 1, 1), 15);
    lonAccessor.setDouble(geoGrid.getIndex(1, 2, 0), 16);
    lonAccessor.setDouble(geoGrid.getIndex(1, 2, 1), 17);
    valarray<long> synIndices(3);

    //vpr.findPixelPosInWholeGrid(20.0, 10.0, synIndices);
    CPPUNIT_ASSERT(synIndices[0] == 0);
    CPPUNIT_ASSERT(synIndices[1] == 0);
    CPPUNIT_ASSERT(synIndices[2] == 0);

    //vpr.findPixelPosInWholeGrid(45.0, 17.0, synIndices);
    CPPUNIT_ASSERT(synIndices[0] == 1);
    CPPUNIT_ASSERT(synIndices[1] == 2);
    CPPUNIT_ASSERT(synIndices[2] == 1);
}

void VprTest::testFindPixelPosAroundGivenIndices() {
    Vpr vpr;
    const shared_ptr<Segment> geoSegment = shared_ptr<Segment>(new SwathSegment(Constants::SEGMENT_GEO, 100, 100, 1, 0, 99));
    Accessor& latAccessor = geoSegment->addVariable("latitude", Constants::TYPE_DOUBLE);
    Accessor& lonAccessor = geoSegment->addVariable("longitude", Constants::TYPE_DOUBLE);
    vpr.latAccessor = &latAccessor;
    vpr.lonAccessor = &lonAccessor;

    const Grid& geoGrid = geoSegment->getGrid();
    //vpr.geoGrid = &geoGrid;

    for(long l = 0; l < 100; l++) {
        for(long m = 0; m < 100; m++) {
            size_t index = geoGrid.getIndex(0, l, m);
            latAccessor.setDouble(index, l * 100 + m);
            lonAccessor.setDouble(index, 10000 - l * 100 - m);
        }
    }

    valarray<long> synIndices(3);
    synIndices[0] = 0;
    synIndices[1] = 10;
    synIndices[2] = 49;

    //vpr.findPixelPosAroundGivenIndices(1050.0, 8950.0, synIndices);
    CPPUNIT_ASSERT(synIndices[0] == 0);
    CPPUNIT_ASSERT(synIndices[1] == 10);
    CPPUNIT_ASSERT(synIndices[2] == 50);

    synIndices[0] = 0;
    synIndices[1] = 20;
    synIndices[2] = 70;

    //vpr.findPixelPosAroundGivenIndices(1464.0, 8636.0, synIndices);
    CPPUNIT_ASSERT(synIndices[0] == 0);
    CPPUNIT_ASSERT(synIndices[1] == 15);
    CPPUNIT_ASSERT(synIndices[2] == 65);
}

void VprTest::testGetLatitude() {
    CPPUNIT_ASSERT(std::abs(75 - Vpr::getTargetLat(0)) < 0.001);
    CPPUNIT_ASSERT(std::abs(74.1071 - Vpr::getTargetLat(100)) < 0.001);
    CPPUNIT_ASSERT(std::abs(9.5 - Vpr::getTargetLat(7336)) < 0.001);
    CPPUNIT_ASSERT(std::abs(-55.9911 - Vpr::getTargetLat(14671)) < 0.001);

    CPPUNIT_ASSERT_THROW(Vpr::getTargetLat(14672), std::invalid_argument);
    CPPUNIT_ASSERT_THROW(Vpr::getTargetLat(-1), std::invalid_argument);
}

void VprTest::testGetLongitude() {
    CPPUNIT_ASSERT(std::abs(-180 - Vpr::getTargetLon(0)) < 0.001);
    CPPUNIT_ASSERT(std::abs(-179.1071 - Vpr::getTargetLon(100)) < 0.001);
    CPPUNIT_ASSERT(std::abs(0 - Vpr::getTargetLon(20160)) < 0.001);
    CPPUNIT_ASSERT(std::abs(179.9911 - Vpr::getTargetLon(40319)) < 0.001);

    CPPUNIT_ASSERT_THROW(Vpr::getTargetLon(40320), std::invalid_argument);
    CPPUNIT_ASSERT_THROW(Vpr::getTargetLon(-1), std::invalid_argument);
}

void VprTest::testVpr() {
	BasicTask task("SY_UNT_VPR");

	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
	shared_ptr<Module> vbm = shared_ptr<Module>(new Vbm());
	shared_ptr<Module> vfl = shared_ptr<Module>(new Vfl());
	shared_ptr<Module> vpr = shared_ptr<Module>(new Vpr());
	shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

	task.getContext().addModule(reader);
	task.getContext().addModule(col);
	task.getContext().addModule(pcl);
	task.getContext().addModule(vbm);
	task.getContext().addModule(vfl);
	task.getContext().addModule(vpr);
	task.getContext().addModule(writer);

	task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_VPR.xml");
}
