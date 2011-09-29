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
    shared_ptr<valarray<double> > x = shared_ptr<valarray<double> >(new valarray<double>(xInit, 3));

    double eInit1[] = {1.0, 0.0, 0.0};
    double eInit2[] = {0.0, 1.0, 0.0};
    double eInit3[] = {0.0, 0.0, 1.0};
    shared_ptr<valarray<double> > firstE = shared_ptr<valarray<double> >(new valarray<double>(eInit1, 3));
    shared_ptr<valarray<double> > secondE = shared_ptr<valarray<double> >(new valarray<double>(eInit2, 3));
    shared_ptr<valarray<double> > thirdE = shared_ptr<valarray<double> >(new valarray<double>(eInit3, 3));

    valarray<shared_ptr<valarray<double> > > e = valarray<shared_ptr<valarray<double> > >(3);
    e[0] = firstE;
    e[1] = secondE;
    e[2] = thirdE;

    const bool success = multiMin->powell(shared_ptr<MultivariateFunction>(new Cigar()), x, e, ACCURACY_GOAL, 200);
    CPPUNIT_ASSERT(success);

    CPPUNIT_ASSERT(0.0 - (*x)[0] <= ACCURACY_GOAL);
    CPPUNIT_ASSERT(0.0 - (*x)[1] <= ACCURACY_GOAL);
    CPPUNIT_ASSERT(0.0 - (*x)[2] <= ACCURACY_GOAL);
}

void MultiMinTest::testPowellRosenbrock() {
    double xInit[] = {0.0, 0.0, 0.0};
    shared_ptr<valarray<double> > x = shared_ptr<valarray<double> >(new valarray<double>(xInit, 3));

    double eInit1[] = {1.0, 0.0, 0.0};
    double eInit2[] = {0.0, 1.0, 0.0};
    double eInit3[] = {0.0, 0.0, 1.0};
    shared_ptr<valarray<double> > firstE = shared_ptr<valarray<double> >(new valarray<double>(eInit1, 3));
    shared_ptr<valarray<double> > secondE = shared_ptr<valarray<double> >(new valarray<double>(eInit2, 3));
    shared_ptr<valarray<double> > thirdE = shared_ptr<valarray<double> >(new valarray<double>(eInit3, 3));

    valarray<shared_ptr<valarray<double> > > e = valarray<shared_ptr<valarray<double> > >(3);
    e[0] = firstE;
    e[1] = secondE;
    e[2] = thirdE;

    const bool success = multiMin->powell(shared_ptr<MultivariateFunction>(new Rosenbrock()), x, e, ACCURACY_GOAL, 200);
    CPPUNIT_ASSERT(success);
    CPPUNIT_ASSERT(1.0 - (*x)[0] <= ACCURACY_GOAL);
    CPPUNIT_ASSERT(1.0 - (*x)[1] <= ACCURACY_GOAL);
    CPPUNIT_ASSERT(1.0 - (*x)[2] <= ACCURACY_GOAL);
}
