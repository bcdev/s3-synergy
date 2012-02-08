/*
 * Copyright (C) 2010 by Brockmann Consult (info@brockmann-consult.de)
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * File:   UtilsTest.cpp
 * Author: thomass
 *
 * Created on January 6, 2011, 12:14 PM
 */

#include <netcdf.h>
#include <string>

#include "../../../main/cpp/core/SwathSegment.h"
#include "../../../main/cpp/util/Utils.h"

#include "UtilsTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(UtilsTest);

UtilsTest::UtilsTest() {
}

UtilsTest::~UtilsTest() {
}

void UtilsTest::setUp() {

}

void UtilsTest::tearDown() {

}

void UtilsTest::testCreateCountVector() {
    valarray<size_t> countVector3Dims;
    CPPUNIT_ASSERT_THROW(Utils::createCountVector(0, 5, 2, 10, countVector3Dims), std::invalid_argument);
    CPPUNIT_ASSERT_THROW(Utils::createCountVector(4, 5, 2, 10, countVector3Dims), std::invalid_argument);

    Utils::createCountVector(3, 5, 2, 10, countVector3Dims);
    CPPUNIT_ASSERT(countVector3Dims[0] == 5);
    CPPUNIT_ASSERT(countVector3Dims[1] == 2);
    CPPUNIT_ASSERT(countVector3Dims[2] == 10);

    valarray<size_t> countVector2Dims;
    Utils::createCountVector(2, 5, 2, 10, countVector2Dims);
    CPPUNIT_ASSERT(countVector2Dims[0] == 2);
    CPPUNIT_ASSERT(countVector2Dims[1] == 10);

    valarray<size_t> countVector1Dim;
    Utils::createCountVector(1, 5, 2, 10, countVector1Dim);
    CPPUNIT_ASSERT(countVector1Dim[0] == 10);
}

void UtilsTest::testCreateStartVector() {
	valarray<size_t> startVector3Dims;
	CPPUNIT_ASSERT_THROW(Utils::createStartVector(0, 5, startVector3Dims), std::invalid_argument);
    CPPUNIT_ASSERT_THROW(Utils::createStartVector(4, 5, startVector3Dims), std::invalid_argument);

    Utils::createStartVector(3, 60000, startVector3Dims);
    CPPUNIT_ASSERT(startVector3Dims[0] == 0);
    CPPUNIT_ASSERT(startVector3Dims[1] == 60000);
    CPPUNIT_ASSERT(startVector3Dims[2] == 0);

    valarray<size_t> startVector2Dims;
    Utils::createStartVector(2, 60000, startVector2Dims);
    CPPUNIT_ASSERT(startVector2Dims[0] == 60000);
    CPPUNIT_ASSERT(startVector2Dims[1] == 00);

    valarray<size_t> startVector1Dim;
    Utils::createStartVector(1, 60000, startVector1Dim);
    CPPUNIT_ASSERT(startVector1Dim[0] == 0);
}

void UtilsTest::testReplacing() {
    string first = "abcdefg";
    string toReplace = "b";
    Utils::replaceString(toReplace, "31415", first);
    CPPUNIT_ASSERT(first.compare("a31415cdefg") == 0);

    string second = "<checksum checksumName=\"MD5\">${checksum-tiepoints_meteo.nc}</checksum>";
    Utils::replaceString("\\$\\{checksum-tiepoints_meteo\\.nc\\}", "someChecksum", second);
    CPPUNIT_ASSERT(second.compare("<checksum checksumName=\"MD5\">someChecksum</checksum>") == 0);
}
