/*
 * TimeConverter.cpp
 *
 *  Created on: 07.02.2012
 *      Author: thomasstorm
 */

#include "TimeConverter.h"

TimeConverter::TimeConverter(const string& startTimeString) {
    // startTime expected in the format "yyyyMMdd_hhmmss.MMMM";
    // todo: set start time to seconds since 01.01.2000

    /*
     * 1) construct date object a from input string
     * 2) construct date object b from 01.01.2000
     * 3) construct time_duration td from a - b
     * 4) return td.total_seconds()
     */

    string modifiedStartTime(startTimeString);
    replaceString("_", "T", modifiedStartTime);
    modifiedStartTime = modifiedStartTime.substr(0, 14);

    ptime a(from_iso_string(modifiedStartTime));
    ptime b(from_iso_string("20000101T000000"));
    time_duration td = a - b;
    startTime = td.total_seconds();
}

TimeConverter::~TimeConverter() {
    // TODO Auto-generated destructor stub
}

int16_t TimeConverter::getMinutesSinceStartTime(uint64_t microSeconds) const {
    const int64_t seconds = microSeconds / 1000L / 1000L;
    const int64_t secondsSinceStartTime = seconds - startTime;
    return secondsSinceStartTime / 60;
}
