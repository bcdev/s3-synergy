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
    const int16_t minutes = tc.getMinutesSinceReferenceTime(0);
    CPPUNIT_ASSERT_EQUAL((int16_t)0, minutes);
}

void TimeConverterTest::testGetTime() {
    const TimeConverter tc("20000101_000000.0000");
    const int16_t minutes = tc.getMinutesSinceReferenceTime(60 * 1000 * 1000);
    CPPUNIT_ASSERT_EQUAL((int16_t)1, minutes);
}

void TimeConverterTest::testLeapYear() {
    const TimeConverter tc("20000301_000000.0000");
    const int16_t minutes = tc.getMinutesSinceReferenceTime(60UL * 24UL * 60UL * 60UL * 1000UL * 1000UL);
    CPPUNIT_ASSERT_EQUAL((int16_t)0, minutes);
}
