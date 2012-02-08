/* 
 * File:   AeiTest.h
 * Author: thomasstorm
 *
 * Created on Sep 22, 2011
 */

#ifndef AEITEST_H
#define	AEITEST_H

#include <cppunit/extensions/HelperMacros.h>

class AeiTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(AeiTest);
    CPPUNIT_TEST(testAei);
    CPPUNIT_TEST_SUITE_END();

public:
    AeiTest();
    virtual ~AeiTest();
    void setUp();
    void tearDown();

private:
    void testAei();
};

#endif	/* AEITEST_H */

