/* 
 * File:   PclTest.h
 * Author: thomasstorm
 *
 * Created on Sep 14, 2011, 1:36 PM
 */

#ifndef PCLTEST_H
#define	PCLTEST_H

#include <cppunit/extensions/HelperMacros.h>

class PclTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(PclTest);
    CPPUNIT_TEST(testPcl);
    CPPUNIT_TEST_SUITE_END();

public:
    PclTest();
    virtual ~PclTest();
    void setUp();
    void tearDown();

private:
    void testPcl();
};

#endif	/* PCLTEST_H */

