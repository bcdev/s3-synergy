/*
 * LookupTableTest.h
 *
 *  Created on: Nov 1, 2011
 *      Author: ralf
 */

#ifndef LOOKUPTABLETEST_H_
#define LOOKUPTABLETEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/GridImpl.h"

class LookupTableTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(LookupTableTest);
    CPPUNIT_TEST(testInterpolation1D);
    CPPUNIT_TEST(testInterpolation2D);
    CPPUNIT_TEST(testInterpolation3D);
    CPPUNIT_TEST(testInterpolation4D);
    CPPUNIT_TEST_SUITE_END();

public:
    LookupTableTest();
    virtual ~LookupTableTest();
    void setUp();
    void tearDown();

private:
    void testInterpolation1D();
    void testInterpolation2D();
    void testInterpolation3D();
    void testInterpolation4D();
};

#endif /* LOOKUPTABLETEST_H_ */
