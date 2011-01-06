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
 * File:   ReaderTest.cpp
 * Author: thomass
 * 
 * Created on December 20, 2010, 4:20 PM
 */

#include <iostream>

#include "../../../main/cpp/util/OlciGridReader.h"

#include "ReaderTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ReaderTest);

ReaderTest::ReaderTest() {
}

ReaderTest::~ReaderTest() {
}

void ReaderTest::setUp() {

}

void ReaderTest::tearDown() {
    
}

void ReaderTest::testReading_OLCI_1() {
//    Reader reader;
//    NcFile* dataFile = new NcFile("/mnt/hgfs/S3L2PP/src/test/resources/syn/SY_1_SYN/OLC_RADIANCE_O1.nc", NcFile::ReadOnly);
//    short* data = new short[1000 * 5 * 740];
//    reader.readData(dataFile, 1000, 5, 740, "TOA_Radiance_Meas", data);
//    CPPUNIT_ASSERT(data[0] == 0);
//    CPPUNIT_ASSERT(data[1] == 1);
//    CPPUNIT_ASSERT(data[2] == 2);
//    CPPUNIT_ASSERT(data[3] == 3);
//    CPPUNIT_ASSERT(data[4] == 4);
//    CPPUNIT_ASSERT(data[5] == 5);
//    CPPUNIT_ASSERT(data[6] == 6);
}

void ReaderTest::testReading_OLCI_2() {
//    Reader reader;
//    NcFile* dataFile = new NcFile("/mnt/hgfs/S3L2PP/src/test/resources/syn/SY_1_SYN/OLC_RADIANCE_O1.nc", NcFile::ReadOnly);
//    short* data = new short[1000 * 5 * 740];
//    reader.readData(dataFile, 1000, 5, 740, "error_estimates", data);
//    CPPUNIT_ASSERT(data[0] == 5);
//    CPPUNIT_ASSERT(data[1] == 4);
//    CPPUNIT_ASSERT(data[2] == 3);
//    CPPUNIT_ASSERT(data[3] == 2);
//    CPPUNIT_ASSERT(data[4] == 1);
//    CPPUNIT_ASSERT(data[5] == 0);
}
