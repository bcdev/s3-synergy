/*
 * TimeConverter.cpp
 *
 *  Created on: 07.02.2012
 *      Author: thomasstorm
 */

#include "../core/Boost.h"

#include "TimeConverter.h"

TimeConverter::TimeConverter(const string& startTimeString) {
    string s(startTimeString);
    s.replace(8, 1, "T");
    s = s.substr(0, 15);

    ptime a(from_iso_string(s));
    ptime b(from_iso_string("20000101T000000"));
    time_duration td = a - b;
    startSeconds = td.total_seconds();
}

TimeConverter::~TimeConverter() {
}

int64_t TimeConverter::getMinutesSinceStartTime(int64_t microSeconds) const {
    const int64_t seconds = microSeconds / 1000L / 1000L;
    const int64_t secondsSinceStartTime = seconds - startSeconds;

    return secondsSinceStartTime / 60;
}
