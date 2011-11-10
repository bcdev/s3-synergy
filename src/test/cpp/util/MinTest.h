/*
 * MinTest.h
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#ifndef MINTEST_H_
#define MINTEST_H_

#include <cmath>

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/util/UnivariateFunction.h"

class MinTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(MinTest);
    CPPUNIT_TEST(testBracketCos);
    CPPUNIT_TEST(testBracketSin);
    CPPUNIT_TEST(testBrentCos);
    CPPUNIT_TEST(testBrentSin);
    CPPUNIT_TEST_SUITE_END();

public:
    MinTest();
    virtual ~MinTest();
    void setUp();
    void tearDown();

private:
    void testBracketCos();
    void testBracketSin();
    void testBrentCos();
    void testBrentSin();
};

class Cos : public UnivariateFunction {

    double getValue(double x) {
        return std::cos(x);
    }
};

class Sin : public UnivariateFunction {

    double getValue(double x) {
        return std::sin(x);
    }
};

#endif /* MINTEST_H_ */
