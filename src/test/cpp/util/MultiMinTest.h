/*
 * MulitMinTest.h
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#ifndef MULTIMINTEST_H_
#define MULTIMINTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class MultiMinTest: public CPPUNIT_NS::TestFixture {
CPPUNIT_TEST_SUITE(MultiMinTest);
		CPPUNIT_TEST(testChol2D);
		CPPUNIT_TEST(testPowellCigar);
		CPPUNIT_TEST(testPowellRosenbrock);
	CPPUNIT_TEST_SUITE_END();

public:
	MultiMinTest();
	virtual ~MultiMinTest();
	void setUp();
	void tearDown();

private:
	void testChol2D();
	void testPowellCigar();
	void testPowellRosenbrock();
};

#endif /* MULTIMINTEST_H_ */
