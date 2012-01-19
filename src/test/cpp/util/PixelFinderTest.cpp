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

class GeoLocationProvider : public GeoLocation {
public:

	GeoLocationProvider() : grid(4, 6000, 740, 0, 5999) {
	}

	~GeoLocationProvider() {

	}

	const Grid& getGrid() const {
		return grid;
	}

	double getLat(size_t index) const {
		long k = index / grid.getStrideK();
		long l = (index - k * grid.getStrideK()) / grid.getStrideL();
		return 90.0 - l * (180.0/grid.getMaxL());
	}

	double getLon(size_t index) const {
		long k = index / grid.getStrideK();
		long m = index % grid.getSizeM();
		return m * (22.2/grid.getMaxM()) - 44.4 + k * 22.2;
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
	GeoLocationProvider geoLocation;
	PixelFinder pixelFinder(geoLocation, 0.12);

	long k = 0;
	long l = 0;
	long m = 0;

	pixelFinder.findSourcePixel(90.0, -44.4, k, l, m);

	CPPUNIT_ASSERT(k == 0);
	CPPUNIT_ASSERT(l == 0);
	CPPUNIT_ASSERT(m == 0);

	pixelFinder.findSourcePixel(90.0, -44.4, k, l, m);

	CPPUNIT_ASSERT(k == 0);
	CPPUNIT_ASSERT(l == 0);
	CPPUNIT_ASSERT(m == 0);
}
