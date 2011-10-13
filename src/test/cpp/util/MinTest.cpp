/*
 * MinTest.cpp
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#include "MinTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(MinTest);

MinTest::MinTest() {
}

MinTest::~MinTest() {
}

void MinTest::setUp() {
}

void MinTest::tearDown() {

}

void MinTest::testBracketCos() {
    Cos cosine;
    Bracket bracket;

    bracket = Min::brack(cosine, 0.0, 0.5, bracket);

    CPPUNIT_ASSERT(bracket.lowerX < bracket.minimumX);
    CPPUNIT_ASSERT(bracket.upperX > bracket.minimumX);

    CPPUNIT_ASSERT(bracket.lowerF > bracket.minimumF);
    CPPUNIT_ASSERT(bracket.upperF > bracket.minimumF);
}

void MinTest::testBracketSin() {
    Sin sinus;
    Bracket bracket;
    bracket = Min::brack(sinus, 0.0, 0.5, bracket);

    CPPUNIT_ASSERT(bracket.lowerX < bracket.minimumX);
    CPPUNIT_ASSERT(bracket.upperX > bracket.minimumX);

    CPPUNIT_ASSERT(bracket.lowerF > bracket.minimumF);
    CPPUNIT_ASSERT(bracket.upperF > bracket.minimumF);
}

void MinTest::testBrentCos() {
    Cos cosine;
    Bracket bracket(2.0, 5.0, cosine);

    const bool success = Min::brent(cosine, bracket, Min::ACCURACY_GOAL);

    CPPUNIT_ASSERT(success);
    CPPUNIT_ASSERT(std::abs(M_PI - bracket.minimumX) < Min::ACCURACY_GOAL);
    CPPUNIT_ASSERT(std::abs(-1.0 - bracket.minimumF) < Min::ACCURACY_GOAL);
}

void MinTest::testBrentSin() {
    Sin sinus;
    Bracket bracket(3.0, 6.0, sinus);

    const bool success = Min::brent(sinus, bracket, Min::ACCURACY_GOAL);

    CPPUNIT_ASSERT(success);
    CPPUNIT_ASSERT(std::abs(1.5 * M_PI - bracket.minimumX) < Min::ACCURACY_GOAL);
    CPPUNIT_ASSERT(std::abs(-1.0 - bracket.minimumF) < Min::ACCURACY_GOAL);
}
