/*
 * NetcdfSegmentTest.h
 *
 *  Created on: 01.02.2012
 *      Author: thomasstorm
 */

#ifndef NETCDFSEGMENTTEST_H_
#define NETCDFSEGMENTTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/Dictionary.h"
#include "../../../main/cpp/core/NetcdfSegment.h"

class NetcdfSegmentTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(NetcdfSegmentTest);
    CPPUNIT_TEST(testSomething);
    CPPUNIT_TEST_SUITE_END();

public:
    NetcdfSegmentTest();
    virtual ~NetcdfSegmentTest();

private:
    void testSomething();
};

#endif /* NETCDFSEGMENTTEST_H_ */
