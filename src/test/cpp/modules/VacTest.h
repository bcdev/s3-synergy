/* 
 * File:   VacTest.h
 * Author: thomasstorm
 *
 * Created on Jan 09, 2012
 */

#ifndef VACTEST_H
#define	VACTEST_H

#include <cppunit/extensions/HelperMacros.h>

class VacTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VacTest);
    CPPUNIT_TEST(testVac);
    CPPUNIT_TEST_SUITE_END();

public:
    VacTest();
    virtual ~VacTest();
    void setUp();
    void tearDown();

private:
    void testVac();
};

#endif	/* VACTEST_H */

