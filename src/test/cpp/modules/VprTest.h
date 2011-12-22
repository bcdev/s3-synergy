/* 
 * File:   VprTest.h
 * Author: thomasstorm
 *
 * Created on Dec 01, 2011
 */

#ifndef VPRTEST_H
#define	VPRTEST_H

#include <cppunit/extensions/HelperMacros.h>

class VprTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VprTest);
    CPPUNIT_TEST(testGetPixelPosInGrid);
    CPPUNIT_TEST(testFindPixelPosAroundGivenIndices);
    CPPUNIT_TEST(testGetLatitude);
    CPPUNIT_TEST(testGetLongitude);
    CPPUNIT_TEST(testVpr);
    CPPUNIT_TEST_SUITE_END();

public:
    VprTest();
    virtual ~VprTest();
    void setUp();
    void tearDown();

private:
    void testGetPixelPosInGrid();
    void testFindPixelPosAroundGivenIndices();
    void testGetLatitude();
    void testGetLongitude();
    void testVpr();
};

#endif	/* VPRTEST_H */

