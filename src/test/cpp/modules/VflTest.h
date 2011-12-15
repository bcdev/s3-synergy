/* 
 * File:   VflTest.h
 * Author: thomasstorm
 *
 * Created on Nov 17, 2011
 */

#ifndef VFLTEST_H
#define	VFLTEST_H

#include <cppunit/extensions/HelperMacros.h>

class VflTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VflTest);
    CPPUNIT_TEST(testVfl);
    CPPUNIT_TEST_SUITE_END();

public:
    VflTest();
    virtual ~VflTest();
    void setUp();
    void tearDown();

private:
    void testVfl();
};

#endif	/* VFLTEST_H */

