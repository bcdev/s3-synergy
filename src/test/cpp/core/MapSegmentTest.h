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

#ifndef MAPSEGMENTTEST_H_
#define MAPSEGMENTTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/MapSegment.h"

class MapSegmentTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(MapSegmentTest);
    CPPUNIT_TEST(testAddVariable);
    CPPUNIT_TEST(testGetId);
    CPPUNIT_TEST(testGetGrid);
    CPPUNIT_TEST(testGetAccessor);
    CPPUNIT_TEST_SUITE_END();

public:
    MapSegmentTest();
    virtual ~MapSegmentTest();
    void setUp();
    void tearDown();

private:
    void testAddVariable();
    void testGetId();
    void testGetGrid();
    void testGetAccessor();

    MapSegment* segment;
};

#endif /* MAPSEGMENTTEST_H_ */
