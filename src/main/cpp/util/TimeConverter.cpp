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

#include "../core/Boost.h"

#include "TimeConverter.h"

using boost::posix_time::from_iso_string;
using boost::posix_time::ptime;
using boost::posix_time::time_duration;

TimeConverter::TimeConverter(const string& referenceTime) {
    string s(referenceTime);
    s.replace(8, 1, "T");
    s = s.substr(0, 15);

    ptime a(from_iso_string(s));
    ptime b(from_iso_string("20000101T000000"));
    time_duration td = a - b;

    referenceSeconds = td.total_seconds();
}

TimeConverter::~TimeConverter() {
}

int64_t TimeConverter::getMinutesSinceReferenceTime(int64_t microSeconds) const {
    const int64_t seconds = microSeconds / 1000L / 1000L;
    const int64_t secondsSinceStartTime = seconds - referenceSeconds;

    return secondsSinceStartTime / 60;
}

int64_t TimeConverter::getMicrosSinceReferenceTime(int64_t microSeconds) const {
    return microSeconds - referenceSeconds * 1000000;
}
