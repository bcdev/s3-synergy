/*
 * PixelFinderTest.cpp
 *
 *  Created on: Jan 18, 2012
 *      Author: ralf
 */

#include "PixelFinderTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(PixelFinderTest);


PixelFinderTest::PixelFinderTest() {
}

PixelFinderTest::~PixelFinderTest() {
}

void PixelFinderTest::setUp() {
}

void PixelFinderTest::tearDown() {

}

void PixelFinderTest::testFindPixel() {
	GeoLocationProvider geoLocation;
	PixelFinder pixelFinder(geoLocation, 0.12);

	long k = 0;
	long l = 0;
	long m = 0;
	pixelFinder.findSourcePixel(90.0, -44.4, k, l, m);

	CPPUNIT_ASSERT(k == 0);
	CPPUNIT_ASSERT(l == 0);
	CPPUNIT_ASSERT(m == 0);
}
