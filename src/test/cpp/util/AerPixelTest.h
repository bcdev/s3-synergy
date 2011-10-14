/*
 * AerPixelTest.h
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#ifndef AERPIXELTEST_H_
#define AERPIXELTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/util/AerPixel.h"

class AerPixelTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(AerPixelTest);
    CPPUNIT_TEST(testCopyConstructor);
    CPPUNIT_TEST_SUITE_END();

public:
    AerPixelTest();
    virtual ~AerPixelTest();
    void setUp();
    void tearDown();

private:
    void testCopyConstructor();
};


#endif /* AERPIXELTEST_H_ */
