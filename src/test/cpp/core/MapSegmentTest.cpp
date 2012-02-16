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

    CPPUNIT_ASSERT(grid.getMinK() == 0);
    CPPUNIT_ASSERT(grid.getMinL() == 0);
    CPPUNIT_ASSERT(grid.getMinM() == 0);
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
