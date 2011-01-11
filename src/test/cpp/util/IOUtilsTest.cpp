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

void IOUtilsTest::testReadOlciDataPart() {

    string fileName = "/mnt/hgfs/S3L2PP/src/test/resources/syn/SY_1_SYN/OLC_RADIANCE_O1.nc";
    int ncId;
    CPPUNIT_ASSERT(nc_open(fileName.c_str(), NC_NOWRITE, &ncId) == NC_NOERR);

    string varName = "TOA_Radiance_Meas";
    int varId;
    CPPUNIT_ASSERT(nc_inq_varid(ncId, varName.c_str(), &varId) == NC_NOERR);

    Segment* segment = new SegmentImpl("SiggiDasSegment", 2, 2, 4, 0, 4);

    string symbolicName = "SymbolicVarName";
    size_t dimCount = 3;
    size_t startLine = 0;
    CPPUNIT_ASSERT_THROW(IOUtils::readData(ncId, varId, segment->getAccessor(symbolicName), segment->getGrid(), dimCount, startLine), std::logic_error);

    segment->addVariableUShort(symbolicName);
    CPPUNIT_ASSERT_NO_THROW(IOUtils::readData(ncId, varId, segment->getAccessor(symbolicName), segment->getGrid(), dimCount, startLine));

    valarray<uint16_t>& data = segment->getAccessor(symbolicName).getUShortData();

    Grid& grid = segment->getGrid();
    int index = grid.getIndex(0, 0, 0);
    CPPUNIT_ASSERT(data[index] == 1);

    index = grid.getIndex(1, 0, 0);
    CPPUNIT_ASSERT(data[index] == 11);

    index = grid.getIndex(0, 1, 1);
    CPPUNIT_ASSERT(data[index] == 4);

    index = grid.getIndex(1, 1, 0);
    CPPUNIT_ASSERT(data[index] == 13);

    index = grid.getIndex(3, 1, 1);
    CPPUNIT_ASSERT(data[index] == 34);

    CPPUNIT_ASSERT_THROW(index = grid.getIndex(0, 4, 0), std::out_of_range);

    CPPUNIT_ASSERT_THROW(index = grid.getIndex(3, 4, 1), std::out_of_range);

    startLine = 1;
    segment->setStartL(startLine);
    CPPUNIT_ASSERT(data[grid.getIndex(0, 1, 0)] == 3);
    CPPUNIT_ASSERT(data[grid.getIndex(0, 1, 1)] == 4);
    CPPUNIT_ASSERT(data[grid.getIndex(1, 1, 0)] == 13);
    CPPUNIT_ASSERT(data[grid.getIndex(1, 1, 1)] == 14);
    CPPUNIT_ASSERT(data[grid.getIndex(2, 1, 0)] == 23);
    CPPUNIT_ASSERT(data[grid.getIndex(2, 1, 1)] == 24);
    CPPUNIT_ASSERT(data[grid.getIndex(3, 1, 0)] == 33);
    CPPUNIT_ASSERT(data[grid.getIndex(3, 1, 1)] == 34);

    IOUtils::readData(ncId, varId, segment->getAccessor(symbolicName), segment->getGrid(), dimCount, startLine + 1);
    data = segment->getAccessor(symbolicName).getUShortData();

    index = grid.getIndex(0, 1, 0);
    CPPUNIT_ASSERT(data[grid.getIndex(0, 1, 0)] == 3);
    CPPUNIT_ASSERT(data[grid.getIndex(0, 1, 1)] == 4);
    CPPUNIT_ASSERT(data[grid.getIndex(1, 1, 0)] == 13);
    CPPUNIT_ASSERT(data[grid.getIndex(1, 1, 1)] == 14);
    CPPUNIT_ASSERT(data[grid.getIndex(2, 1, 0)] == 23);
    CPPUNIT_ASSERT(data[grid.getIndex(2, 1, 1)] == 24);
    CPPUNIT_ASSERT(data[grid.getIndex(3, 1, 0)] == 33);
    CPPUNIT_ASSERT(data[grid.getIndex(3, 1, 1)] == 34);

    CPPUNIT_ASSERT(data[grid.getIndex(0, 2, 0)] == 5);
    CPPUNIT_ASSERT(data[grid.getIndex(0, 2, 1)] == 6);
    CPPUNIT_ASSERT(data[grid.getIndex(1, 2, 0)] == 15);
    CPPUNIT_ASSERT(data[grid.getIndex(1, 2, 1)] == 16);
    CPPUNIT_ASSERT(data[grid.getIndex(2, 2, 0)] == 25);
    CPPUNIT_ASSERT(data[grid.getIndex(2, 2, 1)] == 26);
    CPPUNIT_ASSERT(data[grid.getIndex(3, 2, 0)] == 35);
    CPPUNIT_ASSERT(data[grid.getIndex(3, 2, 1)] == 36);

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
    CPPUNIT_ASSERT(countVector1Dim[0] == 2);
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
    CPPUNIT_ASSERT(startVector1Dim[0] == 60000);
}

void IOUtilsTest::testAddVariableToSegment() {
    Segment* segment = new SegmentImpl("Asterix", 10, 10, 5, 0, 9);
    CPPUNIT_ASSERT(segment->hasVariable("Obelix") == false);
    CPPUNIT_ASSERT_THROW(segment->getAccessor("Obelix"), std::logic_error);
    IOUtils::addVariableToSegment("Obelix", NC_SHORT, *segment);
    CPPUNIT_ASSERT(segment->hasVariable("Obelix") == true);
    CPPUNIT_ASSERT_NO_THROW(segment->getAccessor("Obelix"));
}