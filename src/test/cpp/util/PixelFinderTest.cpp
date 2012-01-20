/*
 * PixelFinderTest.cpp
 *
 *  Created on: Jan 18, 2012
 *      Author: ralf
 */

#include <string>

#include "../../../main/cpp/core/Boost.h"
#include "../../../main/cpp/core/GridImpl.h"

#include "PixelFinderTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(PixelFinderTest);

class TestGeoLocation : public GeoLocation {
public:

	TestGeoLocation() : grid(4, 6000, 740, 0, 5999) {
	}

	~TestGeoLocation() {

	}

	const Grid& getGrid() const {
		return grid;
	}

	double lat(long l) const {
		return 80.0 - l * (160.0 / grid.getMaxL());
	}

	double lon(long k, long m) const {
		return m * (22.2 / grid.getSizeM()) - 44.4 + k * 22.2;
	}

	double getLat(size_t index) const {
		long k = index / grid.getStrideK();
		long l = (index - k * grid.getStrideK()) / grid.getStrideL();
		return lat(l);
	}

	double getLon(size_t index) const {
		long k = index / grid.getStrideK();
		long m = index % grid.getSizeM();
		return lon(k, m);
	}

private:
	GridImpl grid;
};

PixelFinderTest::PixelFinderTest() {
}

PixelFinderTest::~PixelFinderTest() {
}

void PixelFinderTest::setUp() {
	geoLocation = new TestGeoLocation();
	pixelFinder = new PixelFinder(*geoLocation, 0.12);
}

void PixelFinderTest::tearDown() {
	delete pixelFinder;
	delete geoLocation;
}

void PixelFinderTest::testFindSinglePixels() {
	long k = 0;
	long l = 0;
	long m = 0;

	double lat;
	double lon;

	lat = geoLocation->lat(0L);
	lon = geoLocation->lon(0L, 0L);
	CPPUNIT_ASSERT(pixelFinder->findSourcePixel(lat, lon, k, l, m));

	CPPUNIT_ASSERT_EQUAL(0L, k);
	CPPUNIT_ASSERT_EQUAL(0L, l);
	CPPUNIT_ASSERT_EQUAL(0L, m);

	lat = geoLocation->lat(1313L);
	lon = geoLocation->lon(1L, 456L);
	CPPUNIT_ASSERT(pixelFinder->findSourcePixel(lat, lon, k, l, m));

	CPPUNIT_ASSERT_EQUAL(1L, k);
	CPPUNIT_ASSERT_EQUAL(1313L, l);
	CPPUNIT_ASSERT_EQUAL(456L, m);

	lat = geoLocation->lat(5999L);
	lon = geoLocation->lon(3L, 739L);
	CPPUNIT_ASSERT(pixelFinder->findSourcePixel(lat, lon, k, l, m));

	CPPUNIT_ASSERT_EQUAL(3L, k);
	CPPUNIT_ASSERT_EQUAL(5999L, l);
	CPPUNIT_ASSERT_EQUAL(739L, m);

	lat = geoLocation->lat(5999L);
	lon = geoLocation->lon(4L, 739L);
	CPPUNIT_ASSERT(!pixelFinder->findSourcePixel(lat, lon, k, l, m));
}

void PixelFinderTest::testFindPixelsInLine() {
	using std::string;

	long resultK = 0;
	long resultL = 0;
	long resultM = 0;

	double lat;
	double lon;

	lat = geoLocation->lat(7L);
	lon = geoLocation->lon(1L, 0L);
	CPPUNIT_ASSERT(pixelFinder->findSourcePixel(lat, lon, resultK, resultL, resultM));

	CPPUNIT_ASSERT_EQUAL(1L, resultK);
	CPPUNIT_ASSERT_EQUAL(7L, resultL);
	CPPUNIT_ASSERT_EQUAL(0L, resultM);

	for (long k = 0; k <= geoLocation->getGrid().getMaxK(); k++) {
		for (long m = 0; m <= geoLocation->getGrid().getMaxM(); m++) {
			lat = geoLocation->lat(7L);
			lon = geoLocation->lon(k, m);
			CPPUNIT_ASSERT(pixelFinder->findSourcePixel(lat, lon, resultK, resultL, resultM));

			CPPUNIT_ASSERT_EQUAL_MESSAGE("k: (m = " + lexical_cast<string>(m) + ", resultM = " + lexical_cast<string>(resultM) + ") ", k, resultK);
			CPPUNIT_ASSERT_EQUAL_MESSAGE("l: ", 7L, resultL);
			CPPUNIT_ASSERT_EQUAL_MESSAGE("m: (k = " + lexical_cast<string>(k) + ", resultK = " + lexical_cast<string>(resultK) + ") ", m, resultM);
		}
	}
}
