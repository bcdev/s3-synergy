/*
 * TimeConverter.cpp
 *
 *  Created on: 07.02.2012
 *      Author: thomasstorm
 */

#include "TimeConverter.h"
#include "Utils.h"

TimeConverter::TimeConverter(const string& startTimeString) {
    string modifiedStartTime(startTimeString);
    Utils::replaceString("_", "T", modifiedStartTime);
    modifiedStartTime = modifiedStartTime.substr(0, 14);

    ptime a(from_iso_string(modifiedStartTime));
    ptime b(from_iso_string("20000101T000000"));
    time_duration td = a - b;
    startTime = td.total_seconds();
}

TimeConverter::~TimeConverter() {
}

int16_t TimeConverter::getMinutesSinceStartTime(uint64_t microSeconds) const {
    const uint64_t seconds = microSeconds / 1000L / 1000L;
    const int64_t secondsSinceStartTime = seconds - startTime;
    return secondsSinceStartTime / 60;
}
