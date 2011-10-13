/*
 * MulitMinTest.h
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#ifndef MULTIMINTEST_H_
#define MULTIMINTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/Boost.h"
#include "../../../main/cpp/util/MultiMin.h"
#include "../../../main/cpp/util/MultivariateFunction.h"

class MultiMinTest: public CPPUNIT_NS::TestFixture {
CPPUNIT_TEST_SUITE(MultiMinTest);
		CPPUNIT_TEST(testPowellCigar);
		CPPUNIT_TEST(testPowellRosenbrock);
	CPPUNIT_TEST_SUITE_END();

public:
	MultiMinTest();
	virtual ~MultiMinTest();
	void setUp();
	void tearDown();

private:
	void testPowellCigar();
	void testPowellRosenbrock();
};

#endif /* MULTIMINTEST_H_ */
