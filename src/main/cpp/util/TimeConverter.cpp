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
