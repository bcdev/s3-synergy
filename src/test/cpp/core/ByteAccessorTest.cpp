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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 *
 * File:   ByteAccessorTest.cpp
 * Author: ralf
 * 
 * Created on December 17, 2010, 1:44 PM
 */

#include <typeinfo>

#include "ByteAccessorTest.h"
#include "../../../main/cpp/core/ByteAccessor.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ByteAccessorTest);

ByteAccessorTest::ByteAccessorTest() {
}

ByteAccessorTest::~ByteAccessorTest() {
}

void ByteAccessorTest::setUp() {
    accessor = new ByteAccessor(1);
}

void ByteAccessorTest::tearDown() {
    delete accessor;
}

void ByteAccessorTest::testGetSampleCount() {
    CPPUNIT_ASSERT(accessor->getSampleCount() == 1);
}

void ByteAccessorTest::testSetByteGetByte() {
    accessor->setByte(0, 127.0);
    CPPUNIT_ASSERT(accessor->getByte(0) == 127);

    accessor->setByte(0, -128.0);
    CPPUNIT_ASSERT(accessor->getByte(0) == -128);

    CPPUNIT_ASSERT_THROW(accessor->setByte(1, 0), out_of_range);
    CPPUNIT_ASSERT_THROW(accessor->getByte(1), out_of_range);
}

void ByteAccessorTest::testSetDoubleGetByte() {
    accessor->setDouble(0, 127.0);
    CPPUNIT_ASSERT(accessor->getByte(0) == 127);

    accessor->setDouble(0, -128.0);
    CPPUNIT_ASSERT(accessor->getByte(0) == -128);

    accessor->setDouble(0, 1.5);
    CPPUNIT_ASSERT(accessor->getByte(0) == 1);

    accessor->setDouble(0, -1.5);
    CPPUNIT_ASSERT(accessor->getByte(0) == -1);
}

void ByteAccessorTest::testSetFloatGetByte() {
    accessor->setFloat(0, 127.0f);
    CPPUNIT_ASSERT(accessor->getByte(0) == 127);

    accessor->setFloat(0, -128.0f);
    CPPUNIT_ASSERT(accessor->getByte(0) == -128);

    accessor->setFloat(0, 1.5f);
    CPPUNIT_ASSERT(accessor->getByte(0) == 1);

    accessor->setFloat(0, -1.5f);
    CPPUNIT_ASSERT(accessor->getByte(0) == -1);
}

void ByteAccessorTest::testSetIntGetByte() {
    accessor->setInt(0, 127);
    CPPUNIT_ASSERT(accessor->getByte(0) == 127);

    accessor->setInt(0, -128);
    CPPUNIT_ASSERT(accessor->getByte(0) == -128);
}

void ByteAccessorTest::testSetUByteGetByte() {
    accessor->setUByte(0, 127);
    CPPUNIT_ASSERT(accessor->getByte(0) == 127);

    CPPUNIT_ASSERT_THROW(accessor->setUByte(0, 255), bad_cast);
    CPPUNIT_ASSERT(accessor->getByte(0) == 127);
}

void ByteAccessorTest::testGetByteData() {
    CPPUNIT_ASSERT_NO_THROW(accessor->getByteData());
}

void ByteAccessorTest::testGetOtherTypeData() {
    CPPUNIT_ASSERT_THROW(accessor->getDoubleData(), bad_cast);
    CPPUNIT_ASSERT_THROW(accessor->getFloatData(), bad_cast);
    CPPUNIT_ASSERT_THROW(accessor->getIntData(), bad_cast);
    CPPUNIT_ASSERT_THROW(accessor->getLongData(), bad_cast);
    CPPUNIT_ASSERT_THROW(accessor->getShortData(), bad_cast);
    CPPUNIT_ASSERT_THROW(accessor->getUByteData(), bad_cast);
    CPPUNIT_ASSERT_THROW(accessor->getUIntData(), bad_cast);
    CPPUNIT_ASSERT_THROW(accessor->getULongData(), bad_cast);
    CPPUNIT_ASSERT_THROW(accessor->getUShortData(), bad_cast);
}
