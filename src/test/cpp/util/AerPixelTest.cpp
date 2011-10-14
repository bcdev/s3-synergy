/*
 * AerPixelTest.cpp
 *
 *  Created on: 14.10.2011
 *      Author: thomasstorm
 */

#include "AerPixelTest.h"
#include "../../../main/cpp/core/SegmentImpl.h"

CPPUNIT_TEST_SUITE_REGISTRATION(AerPixelTest);

AerPixelTest::AerPixelTest() {
}

AerPixelTest::~AerPixelTest() {
}

void AerPixelTest::setUp() {
}

void AerPixelTest::tearDown() {

}

void AerPixelTest::testCopyConstructor() {
    SegmentImpl segment("id", 10, 10, 10, 0, 100);
    segment.addVariable("SYN_flags", Constants::TYPE_SHORT, 1.0, 0.0);
    segment.addVariable("T550", Constants::TYPE_FLOAT, 1.0, 0.0);
    segment.addVariable("T550_er", Constants::TYPE_FLOAT, 1.0, 0.0);
    segment.addVariable("A550", Constants::TYPE_FLOAT, 1.0, 0.0);
    segment.addVariable("AMIN", Constants::TYPE_SHORT, 1.0, 0.0);
    AerPixel p(segment, 2, 5, 7);
    AerPixel q(p);
    CPPUNIT_ASSERT(q.l == 5);
}
