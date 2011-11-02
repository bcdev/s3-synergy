/*
 * ScalarLookupTableTest.h
 *
 *  Created on: Nov 1, 2011
 *      Author: ralf
 */

#ifndef SCALARLOOKUPTABLETEST_H_
#define SCALARLOOKUPTABLETEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/GridImpl.h"

class ScalarLookupTableTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(ScalarLookupTableTest);
    CPPUNIT_TEST(testInterpolation1D);
    CPPUNIT_TEST(testInterpolation2D);
    CPPUNIT_TEST(testInterpolation3D);
    CPPUNIT_TEST_SUITE_END();

public:
    ScalarLookupTableTest();
    virtual ~ScalarLookupTableTest();
    void setUp();
    void tearDown();

private:
    void testInterpolation1D();
    void testInterpolation2D();
    void testInterpolation3D();
};

#endif /* SCALARLOOKUPTABLETEST_H_ */
