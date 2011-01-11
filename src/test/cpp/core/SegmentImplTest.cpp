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
 * File:   SegmentImplTest.cpp
 * Author: ralf
 *
 * Created on December 20, 2010, 11:17 AM
 */

#include <stdexcept>

#include "SegmentImplTest.h"
#include "../../../main/cpp/core/SegmentImpl.h"

using std::invalid_argument;
using std::logic_error;

CPPUNIT_TEST_SUITE_REGISTRATION(SegmentImplTest);

SegmentImplTest::SegmentImplTest() {
}

SegmentImplTest::~SegmentImplTest() {
}

void SegmentImplTest::setUp() {
    segment = new SegmentImpl("TEST", 2000);
}

void SegmentImplTest::tearDown() {
    delete segment;
}

void SegmentImplTest::testAddVariable() {
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

void SegmentImplTest::testGetId() {
    CPPUNIT_ASSERT(segment->getId() == "TEST");
}

void SegmentImplTest::testGetGrid() {
    const Grid& grid = segment->getGrid();

    CPPUNIT_ASSERT(grid.getStartK() == 0);
    CPPUNIT_ASSERT(grid.getStartL() == 0);
    CPPUNIT_ASSERT(grid.getStartM() == 0);
    CPPUNIT_ASSERT(grid.getSizeK() == 5);
    CPPUNIT_ASSERT(grid.getSizeL() == 2000);
    CPPUNIT_ASSERT(grid.getSizeM() == 760);
}

void SegmentImplTest::testGetAccessor() {
    CPPUNIT_ASSERT_THROW(segment->getAccessor("B"), logic_error);
    segment->addVariableByte("B");
    CPPUNIT_ASSERT_NO_THROW(segment->getAccessor("B").getByteData());
    CPPUNIT_ASSERT(segment->getAccessor("B").getByteData().size() == 5 * 2000 * 760);

    CPPUNIT_ASSERT_THROW(segment->getAccessor("D"), logic_error);
    segment->addVariableDouble("D");
    CPPUNIT_ASSERT_NO_THROW(segment->getAccessor("D").getDoubleData());

    CPPUNIT_ASSERT_THROW(segment->getAccessor("F"), logic_error);
    segment->addVariableFloat("F");
    CPPUNIT_ASSERT_NO_THROW(segment->getAccessor("F").getFloatData());

    CPPUNIT_ASSERT_THROW(segment->getAccessor("I"), logic_error);
    segment->addVariableInt("I");
    CPPUNIT_ASSERT_NO_THROW(segment->getAccessor("I").getIntData());

    CPPUNIT_ASSERT_THROW(segment->getAccessor("L"), logic_error);
    segment->addVariableLong("L");
    CPPUNIT_ASSERT_NO_THROW(segment->getAccessor("L").getLongData());

    CPPUNIT_ASSERT_THROW(segment->getAccessor("S"), logic_error);
    segment->addVariableShort("S");
    CPPUNIT_ASSERT_NO_THROW(segment->getAccessor("S").getShortData());

    CPPUNIT_ASSERT_THROW(segment->getAccessor("UB"), logic_error);
    segment->addVariableUByte("UB");
    CPPUNIT_ASSERT_NO_THROW(segment->getAccessor("UB").getUByteData());

    CPPUNIT_ASSERT_THROW(segment->getAccessor("UI"), logic_error);
    segment->addVariableUInt("UI");
    CPPUNIT_ASSERT_NO_THROW(segment->getAccessor("UI").getUIntData());

    CPPUNIT_ASSERT_THROW(segment->getAccessor("UL"), logic_error);
    segment->addVariableULong("UL");
    CPPUNIT_ASSERT_NO_THROW(segment->getAccessor("UL").getULongData());

    CPPUNIT_ASSERT_THROW(segment->getAccessor("US"), logic_error);
    segment->addVariableUShort("US");
    CPPUNIT_ASSERT_NO_THROW(segment->getAccessor("US").getUShortData());
}

void SegmentImplTest::testShift() {
    segment->addVariableUInt("U");
    valarray<uint32_t>& data = segment->getAccessor("U").getUIntData();

    const Grid& grid = segment->getGrid();
    for (size_t k = 0; k < grid.getSizeK(); k++) {
        for (size_t l = 0; l < grid.getSizeL(); l++) {
            for (size_t m = 0; m < grid.getSizeM(); m++) {
                const size_t i = grid.getIndex(k, l, m);
                data[i] = l;
            }
        }
    }

    segment->setStartL(200);
    CPPUNIT_ASSERT(grid.getStartL() == 200);

    for (size_t k = 0; k < grid.getSizeK(); k++) {
        for (size_t l = grid.getStartL(); l < grid.getStartL() + grid.getSizeL(); l++) {
            for (size_t m = 0; m < grid.getSizeM(); m++) {
                const size_t i = grid.getIndex(k, l, m);
                if (l < grid.getStartL() + grid.getSizeL() - 200) {
                    CPPUNIT_ASSERT(data[i] == l);
                } else {
                    CPPUNIT_ASSERT(data[i] == 0);
                }
            }
        }
    }

    CPPUNIT_ASSERT_THROW(segment->setStartL(100), logic_error);
    CPPUNIT_ASSERT(grid.getStartL() == 200);

    CPPUNIT_ASSERT_THROW(segment->setStartL(2201), logic_error);
    CPPUNIT_ASSERT(grid.getStartL() == 200);

    segment->setStartL(2200);
    CPPUNIT_ASSERT(grid.getStartL() == 2200);

    for (size_t k = 0; k < grid.getSizeK(); k++) {
        for (size_t l = grid.getStartL(); l < grid.getStartL() + grid.getSizeL(); l++) {
            for (size_t m = 0; m < grid.getSizeM(); m++) {
                const size_t i = grid.getIndex(k, l, m);
                CPPUNIT_ASSERT(data[i] == 0);
            }
        }
    }
}
