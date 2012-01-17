/*
 * MapSegmentTest.h
 *
 *  Created on: Jan 17, 2012
 *      Author: ralf
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
