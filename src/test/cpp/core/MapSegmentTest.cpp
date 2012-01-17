/*
 * MapSegmentTest.cpp
 *
 *  Created on: Jan 17, 2012
 *      Author: ralf
 */

#include <stdexcept>

#include "MapSegmentTest.h"

using std::invalid_argument;
using std::logic_error;

CPPUNIT_TEST_SUITE_REGISTRATION(MapSegmentTest);

MapSegmentTest::MapSegmentTest() {
}

MapSegmentTest::~MapSegmentTest() {
}

void MapSegmentTest::setUp() {
    segment = new MapSegment("TEST", 1000, 2000);
}

void MapSegmentTest::tearDown() {
    delete segment;
}

void MapSegmentTest::testAddVariable() {
    CPPUNIT_ASSERT(segment->hasVariable("V") == false);
    segment->addVariableByte("V");
    CPPUNIT_ASSERT(segment->hasVariable("V") == true);

    CPPUNIT_ASSERT_THROW(segment->addVariableByte("V"), logic_error);
    CPPUNIT_ASSERT_THROW(segment->addVariableDouble("V"), logic_error);
    CPPUNIT_ASSERT_THROW(segment->addVariableFloat("V"), logic_error);
    CPPUNIT_ASSERT_THROW(segment->addVariableInt("V"), logic_error);
    CPPUNIT_ASSERT_THROW(segment->addVariableLong("V"), logic_error);
    CPPUNIT_ASSERT_THROW(segment->addVariableShort("V"), logic_error);
    CPPUNIT_ASSERT_THROW(segment->addVariableUByte("V"), logic_error);
    CPPUNIT_ASSERT_THROW(segment->addVariableUInt("V"), logic_error);
    CPPUNIT_ASSERT_THROW(segment->addVariableULong("V"), logic_error);
    CPPUNIT_ASSERT_THROW(segment->addVariableUShort("V"), logic_error);
}

void MapSegmentTest::testGetId() {
    CPPUNIT_ASSERT(segment->getId() == "TEST");
}

void MapSegmentTest::testGetGrid() {
    const Grid& grid = segment->getGrid();

    CPPUNIT_ASSERT(grid.getFirstK() == 0);
    CPPUNIT_ASSERT(grid.getFirstL() == 0);
    CPPUNIT_ASSERT(grid.getFirstM() == 0);
    CPPUNIT_ASSERT(grid.getSizeK() == 1);
    CPPUNIT_ASSERT(grid.getSizeL() == 1000);
    CPPUNIT_ASSERT(grid.getSizeM() == 2000);
}

void MapSegmentTest::testGetAccessor() {
    CPPUNIT_ASSERT_THROW(segment->getAccessor("B"), logic_error);
    segment->addVariableByte("B");
    CPPUNIT_ASSERT_NO_THROW(segment->getAccessor("B"));
    CPPUNIT_ASSERT_THROW(segment->getAccessor("B").getByteData(), logic_error);

    CPPUNIT_ASSERT_THROW(segment->getAccessor("D"), logic_error);
    segment->addVariableDouble("D");
    CPPUNIT_ASSERT_NO_THROW(segment->getAccessor("D"));
    CPPUNIT_ASSERT_THROW(segment->getAccessor("D").getDoubleData(), logic_error);
}
