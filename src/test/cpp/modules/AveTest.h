/* 
 * File:   AveTest.h
 * Author: thomasstorm
 *
 * Created on Sep 22, 2011
 */

#ifndef AVETEST_H
#define	AVETEST_H

#include <cppunit/extensions/HelperMacros.h>

class AveTest : public CPPUNIT_NS::TestFixture {
CPPUNIT_TEST_SUITE(AveTest);
    CPPUNIT_TEST(testIsRadianceName);
    CPPUNIT_TEST(testAve);
CPPUNIT_TEST_SUITE_END();

public:
    AveTest();
    virtual ~AveTest();
    void setUp();
    void tearDown();

private:
    void testAve();
    void testIsRadianceName();
};

#endif	/* AVETEST_H */

