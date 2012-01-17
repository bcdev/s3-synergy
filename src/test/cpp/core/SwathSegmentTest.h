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
 * File:   SwathSegmentTest.h
 * Author: ralf
 *
 * Created on December 20, 2010, 11:17 AM
 */

#ifndef SWATHSEGMENTTEST_H
#define	SWATHSEGMENTTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/SwathSegment.h"

class SwathSegmentTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(SwathSegmentTest);
    CPPUNIT_TEST(testAddVariable);
    CPPUNIT_TEST(testGetId);
    CPPUNIT_TEST(testGetGrid);
    CPPUNIT_TEST(testGetAccessor);
    CPPUNIT_TEST(testMoveForward);
    CPPUNIT_TEST_SUITE_END();

public:
    SwathSegmentTest();
    virtual ~SwathSegmentTest();
    void setUp();
    void tearDown();

private:
    void testAddVariable();
    void testGetId();
    void testGetGrid();
    void testGetAccessor();
    void testMoveForward();

    SwathSegment* segment;
};

#endif	/* SWATHSEGMENTTEST_H */

