/*
 * MinTest.cpp
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#include "MinTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(MinTest);

const double MinTest::ACCURACY_GOAL = 1.0E-6;

MinTest::MinTest() {
}

MinTest::~MinTest() {
}

void MinTest::setUp() {
    min = shared_ptr<Min>(new Min());
}

void MinTest::tearDown() {

}

void MinTest::testBracketCos() {
    const shared_ptr<UnivariateFunction> cosine = shared_ptr<UnivariateFunction>(new Cos());
    shared_ptr<Bracket> bracket = shared_ptr<Bracket>(new Bracket());

    bracket = min->brack(cosine, 0.0, 0.5, bracket);

    CPPUNIT_ASSERT(bracket->lowerX < bracket->minimumX);
    CPPUNIT_ASSERT(bracket->upperX > bracket->minimumX);

    CPPUNIT_ASSERT(bracket->lowerF > bracket->minimumF);
    CPPUNIT_ASSERT(bracket->upperF > bracket->minimumF);
}

void MinTest::testBracketSin() {
    const shared_ptr<UnivariateFunction> sinus = shared_ptr<UnivariateFunction>(new Sin());
    shared_ptr<Bracket> bracket = shared_ptr<Bracket>(new Bracket());
    bracket = min->brack(sinus, 0.0, 0.5, bracket);

    CPPUNIT_ASSERT(bracket->lowerX < bracket->minimumX);
    CPPUNIT_ASSERT(bracket->upperX > bracket->minimumX);

    CPPUNIT_ASSERT(bracket->lowerF > bracket->minimumF);
    CPPUNIT_ASSERT(bracket->upperF > bracket->minimumF);
}

void MinTest::testBrentCos() {
    const shared_ptr<UnivariateFunction> cosine = shared_ptr<UnivariateFunction>(new Cos());
    shared_ptr<Bracket> bracket = shared_ptr<Bracket>(new Bracket(2.0, 5.0, cosine));

    const bool success = min->brent(cosine, bracket, ACCURACY_GOAL);

    CPPUNIT_ASSERT(success);
    CPPUNIT_ASSERT(std::abs(M_PI - bracket->minimumX) < ACCURACY_GOAL);
    CPPUNIT_ASSERT(std::abs(-1.0 - bracket->minimumF) < ACCURACY_GOAL);
}

void MinTest::testBrentSin() {
    const shared_ptr<UnivariateFunction> sin = shared_ptr<UnivariateFunction>(new Sin());
    shared_ptr<Bracket> bracket = shared_ptr<Bracket>(new Bracket(3.0, 6.0, sin));

    const bool success = min->brent(sin, bracket, ACCURACY_GOAL);

    CPPUNIT_ASSERT(success);
    CPPUNIT_ASSERT(std::abs(1.5 * M_PI - bracket->minimumX) < ACCURACY_GOAL);
    CPPUNIT_ASSERT(std::abs(-1.0 - bracket->minimumF) < ACCURACY_GOAL);
}
