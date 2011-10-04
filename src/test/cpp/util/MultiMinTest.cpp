/*
 * MultiMinTest.cpp
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#include "MultiMinTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(MultiMinTest);

const double MultiMinTest::ACCURACY_GOAL = 1.0E-8;

MultiMinTest::MultiMinTest() {
}

MultiMinTest::~MultiMinTest() {
}

void MultiMinTest::setUp() {
    multiMin = shared_ptr<MultiMin>(new MultiMin());
}

void MultiMinTest::tearDown() {

}

void MultiMinTest::testPowellCigar() {
    double xInit[] = {1.0, 1.0, 1.0};
    valarray<double> x = valarray<double>(xInit, 3);

    double eInit1[] = {1.0, 0.0, 0.0};
    double eInit2[] = {0.0, 1.0, 0.0};
    double eInit3[] = {0.0, 0.0, 1.0};
    valarray<double> firstE = valarray<double>(eInit1, 3);
    valarray<double> secondE = valarray<double>(eInit2, 3);
    valarray<double> thirdE = valarray<double>(eInit3, 3);

    valarray<valarray<double> > e = valarray<valarray<double> >(3);
    e[0] = firstE;
    e[1] = secondE;
    e[2] = thirdE;

    const bool success = multiMin->powell(shared_ptr<MultivariateFunction>(new Cigar()), x, e, ACCURACY_GOAL, 200);
    CPPUNIT_ASSERT(success);

    CPPUNIT_ASSERT(std::abs(0.0 - x[0]) <= ACCURACY_GOAL);
    CPPUNIT_ASSERT(std::abs(0.0 - x[1]) <= ACCURACY_GOAL);
    CPPUNIT_ASSERT(std::abs(0.0 - x[2]) <= ACCURACY_GOAL);
}

void MultiMinTest::testPowellRosenbrock() {
    double xInit[] = {0.0, 0.0, 0.0};
    valarray<double> x = valarray<double>(xInit, 3);

    double eInit1[] = {1.0, 0.0, 0.0};
    double eInit2[] = {0.0, 1.0, 0.0};
    double eInit3[] = {0.0, 0.0, 1.0};
    valarray<double> firstE = valarray<double>(eInit1, 3);
    valarray<double> secondE = valarray<double>(eInit2, 3);
    valarray<double> thirdE = valarray<double>(eInit3, 3);

    valarray<valarray<double> > e = valarray<valarray<double> >(3);
    e[0] = firstE;
    e[1] = secondE;
    e[2] = thirdE;

    const bool success = multiMin->powell(shared_ptr<MultivariateFunction>(new Rosenbrock()), x, e, ACCURACY_GOAL, 200);
    CPPUNIT_ASSERT(success);
    CPPUNIT_ASSERT(std::abs(1.0 - x[0]) <= ACCURACY_GOAL);
    CPPUNIT_ASSERT(std::abs(1.0 - x[1]) <= ACCURACY_GOAL);
    CPPUNIT_ASSERT(std::abs(1.0 - x[2]) <= ACCURACY_GOAL);
}
