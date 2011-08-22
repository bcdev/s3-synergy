/*
 * ArrayTest.h
 *
 *  Created on: Aug 19, 2011
 *      Author: ralf
 */

#ifndef ARRAYTEST_H_
#define ARRAYTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/Array.h"

class ArrayTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(ArrayTest);
    CPPUNIT_TEST(testByteArray);
    CPPUNIT_TEST(testByteArrayScaled);
    CPPUNIT_TEST(testDoubleArray);
    CPPUNIT_TEST(testDoubleArrayScaled);
    CPPUNIT_TEST_SUITE_END();

public:
    ArrayTest();
    virtual ~ArrayTest();
    void setUp();
    void tearDown();

private:
    void testByteArray();
    void testByteArrayScaled();
    void testDoubleArray();
    void testDoubleArrayScaled();
};

#endif /* ARRAYTEST_H_ */
