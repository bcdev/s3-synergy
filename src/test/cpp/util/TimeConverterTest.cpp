/*
 * TimeConverterTest.cpp
 *
 *  Created on: 07.02.2012
 *      Author: thomasstorm
 */
#include <string>

#include "TimeConverterTest.h"
#include "../../../main/cpp/util/TimeConverter.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TimeConverterTest);


TimeConverterTest::TimeConverterTest() {
    // TODO Auto-generated constructor stub

}

TimeConverterTest::~TimeConverterTest() {
    // TODO Auto-generated destructor stub
}

void TimeConverterTest::testGetStartTime() {
    const TimeConverter tc("20000101_000000.0000");
    const int64_t minutes = tc.getMinutesSinceReferenceTime(0);
    CPPUNIT_ASSERT_EQUAL((int64_t) 0, minutes);
}

void TimeConverterTest::testGetTime() {
    const TimeConverter tc("20000101_000000.0000");
    const int64_t minutes = tc.getMinutesSinceReferenceTime(60 * 1000 * 1000);
    CPPUNIT_ASSERT_EQUAL((int64_t) 1, minutes);
}

void TimeConverterTest::testLeapYear() {
    const TimeConverter tc("20000301_000000.0000");
    const int64_t minutes = tc.getMinutesSinceReferenceTime(60UL * 24UL * 60UL * 60UL * 1000UL * 1000UL);
    CPPUNIT_ASSERT_EQUAL((int64_t) 0, minutes);
}

void TimeConverterTest::testMicrosSinceReferenceTime() {
    const TimeConverter tc1("20130621_000000.0000");
    const int64_t micros1 = tc1.getMicrosSinceReferenceTime(425124560655099L);
    CPPUNIT_ASSERT(micros1 > 0);

    const TimeConverter tc2("20130622_000000.0000");
    const int64_t micros2 = tc2.getMicrosSinceReferenceTime(425124560655099L);
    CPPUNIT_ASSERT(micros2 < 0);
}
