/*
 * MulitMinTest.h
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#ifndef MULTIMINTEST_H_
#define MINTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/Boost.h"
#include "../../../main/cpp/util/MultiMin.h"
#include "../../../main/cpp/util/MultivariateFunction.h"

class MultiMinTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(MultiMinTest);
    CPPUNIT_TEST(testPowellCigar);
    CPPUNIT_TEST(testPowellRosenbrock);
    CPPUNIT_TEST_SUITE_END();

public:
    MultiMinTest();
    virtual ~MultiMinTest();
    void setUp();
    void tearDown();

private:
    const static double ACCURACY_GOAL;

    void testPowellCigar();
    void testPowellRosenbrock();
};

class Cigar : public MultivariateFunction {

    double value(valarray<double>& x) {
        double sum = 0.0;

        for (size_t i = 1; i < x.size(); ++i) {
            sum += MultiMin::square(1000.0 * x[i]);
        }

        return MultiMin::square(x[0] * x[0]) + sum;
    }
};

class Rosenbrock : public MultivariateFunction {

    double value(valarray<double>& x) {
        double sum = 0.0;

        for (size_t i = 0; i < x.size() - 1; ++i) {
            sum += 100.0 * MultiMin::square(MultiMin::square(x[i]) - x[i + 1]) + MultiMin::square(x[i] - 1.0);
        }

        return sum;
    }
};


#endif /* MULTIMINTEST_H_ */
