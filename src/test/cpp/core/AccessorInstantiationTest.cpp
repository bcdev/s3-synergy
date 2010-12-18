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
 * File:   AccessorInstantiationTest.cpp
 * Author: ralf
 * 
 * Created on December 17, 2010, 5:17 PM
 */

#include "AccessorInstantiationTest.h"
#include "../../../main/cpp/core/Accessors.h"

CPPUNIT_TEST_SUITE_REGISTRATION(AccessorInstantiationTest);

AccessorInstantiationTest::AccessorInstantiationTest() {
}

AccessorInstantiationTest::~AccessorInstantiationTest() {
}

void AccessorInstantiationTest::setUp() {
    accessor = 0;
}

void AccessorInstantiationTest::tearDown() {
    if (accessor != 0) {
        delete accessor;
    }
}

void AccessorInstantiationTest::testInstantiateByteAccessor() {
    accessor = new ByteAccessor(1);
    CPPUNIT_ASSERT(accessor != 0);
    CPPUNIT_ASSERT_NO_THROW(accessor->getByteData());
}

void AccessorInstantiationTest::testInstantiateDoubleAccessor() {
    accessor = new DoubleAccessor(1);
    CPPUNIT_ASSERT(accessor != 0);
    CPPUNIT_ASSERT_NO_THROW(accessor->getDoubleData());
}

void AccessorInstantiationTest::testInstantiateFloatAccessor() {
    accessor = new FloatAccessor(1);
    CPPUNIT_ASSERT(accessor != 0);
    CPPUNIT_ASSERT_NO_THROW(accessor->getFloatData());
}

void AccessorInstantiationTest::testInstantiateIntAccessor() {
    accessor = new IntAccessor(1);
    CPPUNIT_ASSERT(accessor != 0);
    CPPUNIT_ASSERT_NO_THROW(accessor->getIntData());
}

void AccessorInstantiationTest::testInstantiateLongAccessor() {
    accessor = new LongAccessor(1);
    CPPUNIT_ASSERT(accessor != 0);
    CPPUNIT_ASSERT_NO_THROW(accessor->getLongData());
}

void AccessorInstantiationTest::testInstantiateShortAccessor() {
    accessor = new ShortAccessor(1);
    CPPUNIT_ASSERT(accessor != 0);
    CPPUNIT_ASSERT_NO_THROW(accessor->getShortData());
}

void AccessorInstantiationTest::testInstantiateUByteAccessor() {
    accessor = new UByteAccessor(1);
    CPPUNIT_ASSERT(accessor != 0);
    CPPUNIT_ASSERT_NO_THROW(accessor->getUByteData());
}

void AccessorInstantiationTest::testInstantiateUIntAccessor() {
    accessor = new UIntAccessor(1);
    CPPUNIT_ASSERT(accessor != 0);
    CPPUNIT_ASSERT_NO_THROW(accessor->getUIntData());
}

void AccessorInstantiationTest::testInstantiateULongAccessor() {
    accessor = new ULongAccessor(1);
    CPPUNIT_ASSERT(accessor != 0);
    CPPUNIT_ASSERT_NO_THROW(accessor->getULongData());
}

void AccessorInstantiationTest::testInstantiateUShortAccessor() {
    accessor = new UShortAccessor(1);
    CPPUNIT_ASSERT(accessor != 0);
    CPPUNIT_ASSERT_NO_THROW(accessor->getUShortData());
}
