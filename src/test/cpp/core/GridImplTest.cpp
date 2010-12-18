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
 * File:   GridImplTest.cpp
 * Author: ralf
 * 
 * Created on December 18, 2010, 8:48 PM
 */

#include "GridImplTest.h"
#include "../../../main/cpp/core/GridImpl.h"

CPPUNIT_TEST_SUITE_REGISTRATION(GridImplTest);

GridImplTest::GridImplTest() {
}

GridImplTest::~GridImplTest() {
}

void GridImplTest::setUp() {
    grid = new GridImpl(5, 60000, 760);
}

void GridImplTest::tearDown() {
    delete grid;
}

void GridImplTest::testStarts() {
    CPPUNIT_ASSERT(grid->getStartK() == 0);
    CPPUNIT_ASSERT(grid->getStartL() == 0);
    CPPUNIT_ASSERT(grid->getStartM() == 0);
}

void GridImplTest::testSizes() {
    CPPUNIT_ASSERT(grid->getSizeK() == 5);
    CPPUNIT_ASSERT(grid->getSizeL() == 60000);
    CPPUNIT_ASSERT(grid->getSizeM() == 760);
    CPPUNIT_ASSERT(grid->getSize() == 5 * 60000 * 760);
}

void GridImplTest::testStrides() {
    CPPUNIT_ASSERT(grid->getStrideK() == 60000 * 760);
    CPPUNIT_ASSERT(grid->getStrideL() == 760);
    CPPUNIT_ASSERT(grid->getStrideM() == 1);
}

void GridImplTest::testGetIndex() {
    CPPUNIT_ASSERT(grid->getIndex(0, 0, 0) == 0);
    CPPUNIT_ASSERT(grid->getIndex(4, 59999, 759) == 4 * 60000 * 760 + 59999 * 760 + 759 * 1);
    CPPUNIT_ASSERT(grid->getIndex(2, 17341, 467) == 2 * 60000 * 760 + 17341 * 760 + 467 * 1);

    CPPUNIT_ASSERT_THROW(grid->getIndex(5, 0, 0), out_of_range);
    CPPUNIT_ASSERT_THROW(grid->getIndex(0, 60000, 0), out_of_range);
    CPPUNIT_ASSERT_THROW(grid->getIndex(0, 0, 760), out_of_range);
}
