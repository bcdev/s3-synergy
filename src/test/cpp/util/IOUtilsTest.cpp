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
 * File:   IOUtilsTest.cpp
 * Author: thomass
 *
 * Created on January 6, 2011, 12:14 PM
 */

#include <netcdf.h>
#include <string>

#include "../../../main/cpp/util/IOUtils.h"
#include "../../../main/cpp/core/SegmentImpl.h"

#include "IOUtilsTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(IOUtilsTest);

IOUtilsTest::IOUtilsTest() {
}

IOUtilsTest::~IOUtilsTest() {
}

void IOUtilsTest::setUp() {

}

void IOUtilsTest::tearDown() {

}

void IOUtilsTest::testCreateCountVector() {
    CPPUNIT_ASSERT_THROW(IOUtils::createCountVector(0, 5, 2, 10), std::invalid_argument);
    CPPUNIT_ASSERT_THROW(IOUtils::createCountVector(4, 5, 2, 10), std::invalid_argument);

    const valarray<size_t> countVector3Dims = IOUtils::createCountVector(3, 5, 2, 10);
    CPPUNIT_ASSERT(countVector3Dims[0] == 5);
    CPPUNIT_ASSERT(countVector3Dims[1] == 2);
    CPPUNIT_ASSERT(countVector3Dims[2] == 10);

    const valarray<size_t> countVector2Dims = IOUtils::createCountVector(2, 5, 2, 10);
    CPPUNIT_ASSERT(countVector2Dims[0] == 2);
    CPPUNIT_ASSERT(countVector2Dims[1] == 10);

    const valarray<size_t> countVector1Dim = IOUtils::createCountVector(1, 5, 2, 10);
    CPPUNIT_ASSERT(countVector1Dim[0] == 10);
}

void IOUtilsTest::testCreateStartVector() {
    CPPUNIT_ASSERT_THROW(IOUtils::createStartVector(0, 5), std::invalid_argument);
    CPPUNIT_ASSERT_THROW(IOUtils::createStartVector(4, 5), std::invalid_argument);

    const valarray<size_t> startVector3Dims = IOUtils::createStartVector(3, 60000);
    CPPUNIT_ASSERT(startVector3Dims[0] == 0);
    CPPUNIT_ASSERT(startVector3Dims[1] == 60000);
    CPPUNIT_ASSERT(startVector3Dims[2] == 0);

    const valarray<size_t> startVector2Dims = IOUtils::createStartVector(2, 60000);
    CPPUNIT_ASSERT(startVector2Dims[0] == 60000);
    CPPUNIT_ASSERT(startVector2Dims[1] == 00);

    const valarray<size_t> startVector1Dim = IOUtils::createStartVector(1, 60000);
    CPPUNIT_ASSERT(startVector1Dim[0] == 0);
}
