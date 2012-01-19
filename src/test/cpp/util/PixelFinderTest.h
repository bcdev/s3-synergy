/*
 * PixelFinderTest.h
 *
 *  Created on: Jan 18, 2012
 *      Author: ralf
 */

#ifndef PIXELFINDERTEST_H_
#define PIXELFINDERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

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
