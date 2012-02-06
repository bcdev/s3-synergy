/* 
 * File:   VcoTest.h
 * Author: thomasstorm
 *
 * Created on Dec 01, 2011
 */

#ifndef VCOTEST_H
#define	VCOTEST_H

#include <cppunit/extensions/HelperMacros.h>

class VcoTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VcoTest);
    CPPUNIT_TEST(testVco);
    CPPUNIT_TEST_SUITE_END();

public:
    VcoTest();
    virtual ~VcoTest();
    void setUp();
    void tearDown();

private:
    void testVco();
};

#endif	/* VCOTEST_H */

