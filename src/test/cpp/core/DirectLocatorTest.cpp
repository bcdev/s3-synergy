/*
 * DirectLocatorTest.cpp
 *
 *  Created on: Dec 20, 2011
 *      Author: ralf
 */

#include "DirectLocatorTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(DirectLocatorTest);

DirectLocatorTest::DirectLocatorTest() {
}

DirectLocatorTest::~DirectLocatorTest() {
}

void DirectLocatorTest::setUp() {
}

void DirectLocatorTest::tearDown() {
}

void DirectLocatorTest::testRotation_Identity() {
	const Rotation r;

	double lat;
	double lon;

	lat = 0.0;
	lon = 0.0;
	r.transform(lat, lon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lat, 0.0);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lon, 0.0);

	r.inverseTransform(lat, lon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lat, 0.0);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lon, 0.0);

	lat = 7.0;
	lon = 2.0;
	r.transform(lat, lon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, lat, 1.0E-15);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, lon, 1.0E-15);

	r.inverseTransform(lat, lon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, lat, 1.0E-15);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, lon, 1.0E-15);
}

void DirectLocatorTest::testRotation_Antipode() {
	const Rotation r(0.0, 180.0);

	double lat;
	double lon;

	lat = 0.0;
	lon = 0.0;
	r.transform(lat, lon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lat, 1.0E-12);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-180.0, lon, 1.0E-12);

	r.inverseTransform(lat, lon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lat, 1.0E-12);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lon, 1.0E-12);

	lat = 7.0;
	lon = -180.0;
	r.transform(lat, lon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, lat, 1.0E-12);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lon, 1.0E-12);

	r.inverseTransform(lat, lon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, lat, 1.0E-12);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-180.0, lon, 1.0E-12);
}


