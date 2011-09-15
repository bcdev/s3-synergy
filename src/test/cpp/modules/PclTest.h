/* 
 * File:   PclTest.h
 * Author: thomass
 *
 * Created on August 16, 2011, 1:36 PM
 */

#ifndef PCLTEST_H
#define	PCLTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"

class PclTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(PclTest);
    CPPUNIT_TEST(testPcl);
    CPPUNIT_TEST(testStart);
    CPPUNIT_TEST_SUITE_END();

public:
    PclTest();
    virtual ~PclTest();
    void setUp();
    void tearDown();

private:
    void testPcl();
    void testStart();
};

#endif	/* PCLTEST_H */

