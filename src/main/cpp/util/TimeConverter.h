/*
 * TimeConverter.h
 *
 *  Created on: 07.02.2012
 *      Author: thomasstorm
 */

#ifndef TIMECONVERTER_H_
#define TIMECONVERTER_H_

#include <string>

using std::string;

class TimeConverter {
public:
    /**
     * Creating a new instance with the given start time.
     *
     * @param startTime the start time, expected in one of the following formats:
     *  <ul>
     *      <li>yyyyMMdd_hhmmss.MMMM</li>
     *      <li>yyyyMMddThhmmss.MMMM<li>
     *      <li>yyyyMMdd_hhmmss<li>
     *      <li>yyyyMMddThhmmss<li>
     *  </ul>
     */
    TimeConverter(const string& startTime);

    ~TimeConverter();

    /**
     * Returns the number of minutes the given date differs from the start time.
     * The date is given as microseconds from 01.01.2000.
     */
    int64_t getMinutesSinceStartTime(int64_t microSeconds) const;

private:
    int64_t startSeconds;
};

#endif /* TIMECONVERTER_H_ */
