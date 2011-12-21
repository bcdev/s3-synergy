/* 
 * File:   VprTest.cpp
 * Author: thomasstorm
 * 
 * Created on Dec 01, 2011
 */

#include <cstdlib>

#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Vbm.h"
#include "../../../../src/main/cpp/modules/Vfl.h"
#include "../../../../src/main/cpp/modules/Vpr.h"
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

void VprTest::testGetLatitude() {
    CPPUNIT_ASSERT(std::abs(75 - Vpr::getLatitude(0)) < 0.001);
    CPPUNIT_ASSERT(std::abs(74.1071 - Vpr::getLatitude(100)) < 0.001);
    CPPUNIT_ASSERT(std::abs(9.5 - Vpr::getLatitude(7336)) < 0.001);
    CPPUNIT_ASSERT(std::abs(-55.9911 - Vpr::getLatitude(14671)) < 0.001);

    CPPUNIT_ASSERT_THROW(Vpr::getLatitude(14672), std::invalid_argument);
    CPPUNIT_ASSERT_THROW(Vpr::getLatitude(-1), std::invalid_argument);
}

void VprTest::testGetLongitude() {
    CPPUNIT_ASSERT(std::abs(-180 - Vpr::getLongitude(0)) < 0.001);
    CPPUNIT_ASSERT(std::abs(-179.1071 - Vpr::getLongitude(100)) < 0.001);
    CPPUNIT_ASSERT(std::abs(0 - Vpr::getLongitude(20160)) < 0.001);
    CPPUNIT_ASSERT(std::abs(179.9911 - Vpr::getLongitude(40319)) < 0.001);

    CPPUNIT_ASSERT_THROW(Vpr::getLongitude(40320), std::invalid_argument);
    CPPUNIT_ASSERT_THROW(Vpr::getLongitude(-1), std::invalid_argument);
}

void VprTest::testFindClosestPixel() {
    valarray<shared_ptr<Pixel> > pixels(4);
    shared_ptr<Pixel> p0 = shared_ptr<Pixel>(new Pixel());
    p0->lat = 53.0;
    p0->lon = 9.957;
    pixels[0] = p0;

    shared_ptr<Pixel> p1 = shared_ptr<Pixel>(new Pixel());
    p1->lat = 55.0;
    p1->lon = 10.0;
    pixels[1] = p1;

    shared_ptr<Pixel> p2 = shared_ptr<Pixel>(new Pixel());
    p2->lat = 54.0;
    p2->lon = 10.0;
    pixels[2] = p2;

    shared_ptr<Pixel> p3 = shared_ptr<Pixel>(new Pixel());
    p3->lat = 55.0;
    p3->lon = 9.9;
    pixels[3] = p3;

    double lat = 54.545;
    double lon = 9.957;
    Vpr vpr;
    Pixel& closestPixel = vpr.findClosestPixel(pixels, lat, lon);

    CPPUNIT_ASSERT(closestPixel.lat == 55.0);
    CPPUNIT_ASSERT(closestPixel.lon == 10.0);
}

void VprTest::testVpr() {
	BasicTask task("SY_UNT_VPR");

	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> vbm = shared_ptr<Module>(new Vbm());
	shared_ptr<Module> vfl = shared_ptr<Module>(new Vfl());
	shared_ptr<Module> vpr = shared_ptr<Module>(new Vpr());
	shared_ptr<Module> writer = shared_ptr<Module>(new SegmentWriter());

	task.getContext().addModule(reader);
	task.getContext().addModule(col);
	task.getContext().addModule(vbm);
	task.getContext().addModule(vfl);
	task.getContext().addModule(vpr);
	task.getContext().addModule(writer);

	task.execute(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_VPR.xml");
}

