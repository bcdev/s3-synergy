/*
 * TimeConverter.h
 *
 *  Created on: 07.02.2012
 *      Author: thomasstorm
 */

#ifndef TIMECONVERTER_H_
#define TIMECONVERTER_H_

#include "../core/Boost.h"
#include <string>

using std::string;

class TimeConverter {
public:
    TimeConverter(const string& startTime);
    virtual ~TimeConverter();

    /**
     * Returns the number of minutes the given date differs from the start time.
     * The date is given as microseconds from 01.01.2000.
     */
    int16_t getMinutesSinceStartTime(uint64_t microSeconds) const;

private:
    int64_t startTime;

    // copied from AbstractWriter
    // TODO - put into utility class
    static void replaceString(const string& toReplace, const string& replacement, string& input) {
        regex expr(toReplace.c_str());
        input = regex_replace(input, expr, replacement);
    }

};

#endif /* TIMECONVERTER_H_ */