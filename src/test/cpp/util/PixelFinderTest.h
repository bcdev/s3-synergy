/*
 * PixelFinderTest.h
 *
 *  Created on: Jan 18, 2012
 *      Author: ralf
 */

#ifndef PIXELFINDERTEST_H_
#define PIXELFINDERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/GridImpl.h"
#include "../../../main/cpp/util/PixelFinder.h"

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

class PixelFinderTest: public CPPUNIT_NS::TestFixture {
CPPUNIT_TEST_SUITE(PixelFinderTest);
		CPPUNIT_TEST(testFindPixel);
	CPPUNIT_TEST_SUITE_END();

public:
	PixelFinderTest();
	virtual ~PixelFinderTest();
	void setUp();
	void tearDown();

private:
	void testFindPixel();
};

#endif /* PIXELFINDERTEST_H_ */
