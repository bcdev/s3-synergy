/*
 * TimeConverterTest.h
 *
 *  Created on: 07.02.2012
 *      Author: thomasstorm
 */

#ifndef TIMECONVERTERTEST_H_
#define TIMECONVERTERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class TimeConverterTest: public CPPUNIT_NS::TestFixture {
CPPUNIT_TEST_SUITE(TimeConverterTest);
    CPPUNIT_TEST(testGetStartTime);
    CPPUNIT_TEST(testGetTime);
    CPPUNIT_TEST(testLeapYear);
CPPUNIT_TEST_SUITE_END();

public:
    TimeConverterTest();
    virtual ~TimeConverterTest();

private:
    void testGetStartTime();
    void testGetTime();
    void testLeapYear();
};

#endif /* TIMECONVERTERTEST_H_ */
