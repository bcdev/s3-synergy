/*
 * TiePointInterpolatorTest.h
 *
 *  Created on: Aug 30, 2011
 *      Author: ralf
 */

#ifndef TIEPOINTINTERPOLATORTEST_H_
#define TIEPOINTINTERPOLATORTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/TiePointInterpolator.h"

class TiePointInterpolatorTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(TiePointInterpolatorTest);
    CPPUNIT_TEST(testWeightsAndIndexesForIntermediatePoint);
    CPPUNIT_TEST(testWeightsAndIndexesForTiePointLocation);
    CPPUNIT_TEST(testInterpolation);
    CPPUNIT_TEST_SUITE_END();

public:
    TiePointInterpolatorTest();
    virtual ~TiePointInterpolatorTest();
    void setUp();
    void tearDown();

private:
    void testWeightsAndIndexesForIntermediatePoint();
    void testWeightsAndIndexesForTiePointLocation();
    void testInterpolation();

    TiePointInterpolator<double>* tpi;
};

#endif /* TIEPOINTINTERPOLATORTEST_H_ */
