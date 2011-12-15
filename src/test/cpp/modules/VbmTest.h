/* 
 * File:   VbmTest.h
 * Author: thomasstorm
 *
 * Created on Nov 17, 2011
 */

#ifndef VBMTEST_H
#define	VBMTEST_H

#include <cppunit/extensions/HelperMacros.h>

class VbmTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VbmTest);
    CPPUNIT_TEST(testVbm);
    CPPUNIT_TEST_SUITE_END();

public:
    VbmTest();
    virtual ~VbmTest();
    void setUp();
    void tearDown();

private:
    void testVbm();
};

#endif	/* VBMTEST_H */

