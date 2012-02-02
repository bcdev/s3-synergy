/*
 * MapAccessorTest.cpp
 *
 *  Created on: Jan 17, 2012
 *      Author: ralf
 */

#include <typeinfo>

#include "MapAccessorTest.h"
#include "../../../main/cpp/core/MapAccessor.h"

CPPUNIT_TEST_SUITE_REGISTRATION(MapAccessorTest);

MapAccessorTest::MapAccessorTest() {
}

MapAccessorTest::~MapAccessorTest() {
}

void MapAccessorTest::setUp() {
    accessor = new MapAccessor<int8_t, Constants::TYPE_BYTE>(2);
}

void MapAccessorTest::tearDown() {
    delete accessor;
}

void MapAccessorTest::testGetSampleCount() {
    CPPUNIT_ASSERT(accessor->getSampleCount() == 2);
}

void MapAccessorTest::testSetByteGetByte() {
    accessor->setByte(0, 127);
    CPPUNIT_ASSERT(accessor->getByte(0) == 127);

    accessor->setByte(0, -128);
    CPPUNIT_ASSERT(accessor->getByte(0) == -128);

    accessor->setByte(1, 20);
    CPPUNIT_ASSERT(accessor->getByte(1) == 20);
    CPPUNIT_ASSERT(accessor->getSampleCount() == 2);
}

void MapAccessorTest::testSetDoubleGetByte() {
    accessor->setDouble(0, 127.0);
    CPPUNIT_ASSERT(accessor->getByte(0) == 127);

    accessor->setDouble(0, -128.0);
    CPPUNIT_ASSERT(accessor->getByte(0) == -128);

    accessor->setDouble(0, 1.5);
    CPPUNIT_ASSERT(accessor->getByte(0) == 1);

    accessor->setDouble(0, -1.5);
    CPPUNIT_ASSERT(accessor->getByte(0) == -1);
}

void MapAccessorTest::testSetFloatGetByte() {
    accessor->setFloat(0, 127.0f);
    CPPUNIT_ASSERT(accessor->getByte(0) == 127);

    accessor->setFloat(0, -128.0f);
    CPPUNIT_ASSERT(accessor->getByte(0) == -128);

    accessor->setFloat(0, 1.5f);
    CPPUNIT_ASSERT(accessor->getByte(0) == 1);

    accessor->setFloat(0, -1.5f);
    CPPUNIT_ASSERT(accessor->getByte(0) == -1);
}

void MapAccessorTest::testSetIntGetByte() {
    accessor->setInt(0, 127);
    CPPUNIT_ASSERT(accessor->getByte(0) == 127);

    accessor->setInt(0, -128);
    CPPUNIT_ASSERT(accessor->getByte(0) == -128);
}

void MapAccessorTest::testSetUByteGetByte() {
    accessor->setUByte(0, 127);
    CPPUNIT_ASSERT(accessor->getByte(0) == 127);

    CPPUNIT_ASSERT_THROW(accessor->setUByte(0, 255), bad_cast);
    CPPUNIT_ASSERT(accessor->getByte(0) == 127);
}

void MapAccessorTest::testGetByteData() {
    CPPUNIT_ASSERT_THROW(accessor->getByteData(), logic_error);
}

void MapAccessorTest::testGetOtherTypeData() {
    CPPUNIT_ASSERT_THROW(accessor->getDoubleData(), logic_error);
    CPPUNIT_ASSERT_THROW(accessor->getFloatData(), logic_error);
    CPPUNIT_ASSERT_THROW(accessor->getIntData(), logic_error);
    CPPUNIT_ASSERT_THROW(accessor->getLongData(), logic_error);
    CPPUNIT_ASSERT_THROW(accessor->getShortData(), logic_error);
    CPPUNIT_ASSERT_THROW(accessor->getUByteData(), logic_error);
    CPPUNIT_ASSERT_THROW(accessor->getUIntData(), logic_error);
    CPPUNIT_ASSERT_THROW(accessor->getULongData(), logic_error);
    CPPUNIT_ASSERT_THROW(accessor->getUShortData(), logic_error);
}
