/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

#ifndef TIMECONVERTER_H_
#define TIMECONVERTER_H_

#include <string>

#include "../core/Types.h"

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
