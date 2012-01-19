/*
 * PixelFinderTest.cpp
 *
 *  Created on: Jan 18, 2012
 *      Author: ralf
 */

#include "../../../main/cpp/core/GridImpl.h"
#include "../../../main/cpp/util/PixelFinder.h"

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
		return 90.0 - l * (180.0 / grid.getMaxL());
	}

	double lon(long k, long m) const {
		return m * (22.2 / grid.getMaxM()) - 44.4 + k * 22.2;
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
}

void PixelFinderTest::tearDown() {

}

void PixelFinderTest::testFindPixel() {
	using std::string;

	TestGeoLocation geoLocation;
	PixelFinder pixelFinder(geoLocation, 0.12);

	long k = 0;
	long l = 0;
	long m = 0;

	double lat;
	double lon;

	lat = geoLocation.lat(0);
	lon = geoLocation.lon(0, 0);
	pixelFinder.findSourcePixel(lat, lon, k, l, m);

	CPPUNIT_ASSERT(k == 0);
	CPPUNIT_ASSERT(l == 0);
	CPPUNIT_ASSERT(m == 0);

	lat = geoLocation.lat(5999);
	lon = geoLocation.lon(4, 739);
	pixelFinder.findSourcePixel(lat, lon, k, l, m);

	CPPUNIT_ASSERT_EQUAL(4L, k);
	CPPUNIT_ASSERT_EQUAL(5999L, l);
	CPPUNIT_ASSERT_EQUAL(739L, m);
}
