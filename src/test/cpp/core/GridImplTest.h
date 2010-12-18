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
 * File:   GridImplTest.h
 * Author: ralf
 *
 * Created on December 18, 2010, 8:48 PM
 */

#ifndef GRIDIMPLTEST_H
#define	GRIDIMPLTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/Grid.h"

class GridImplTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(GridImplTest);
    CPPUNIT_TEST(testStarts);
    CPPUNIT_TEST(testSizes);
    CPPUNIT_TEST(testStrides);
    CPPUNIT_TEST(testGetIndex);
    CPPUNIT_TEST_SUITE_END();

public:
    GridImplTest();
    virtual ~GridImplTest();
    void setUp();
    void tearDown();

private:
    Grid* grid;
    void testStarts();
    void testSizes();
    void testStrides();
    void testGetIndex();
};

#endif	/* GRIDIMPLTEST_H */

