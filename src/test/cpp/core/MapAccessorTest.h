/*
 * MapAccessorTest.h
 *
 *  Created on: Jan 17, 2012
 *      Author: ralf
 */

#ifndef MAPACCESSORTEST_H_
#define MAPACCESSORTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/Accessor.h"

class MapAccessorTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(MapAccessorTest);
    CPPUNIT_TEST(testGetSampleCount);
    CPPUNIT_TEST(testSetByteGetByte);
    CPPUNIT_TEST(testSetDoubleGetByte);
    CPPUNIT_TEST(testSetFloatGetByte);
    CPPUNIT_TEST(testSetIntGetByte);
    CPPUNIT_TEST(testSetUByteGetByte);
    CPPUNIT_TEST(testGetByteData);
    CPPUNIT_TEST(testGetOtherTypeData);
    CPPUNIT_TEST(testGetUntypedValue);
    CPPUNIT_TEST_SUITE_END();

public:
    MapAccessorTest();
    virtual ~MapAccessorTest();
    void setUp();
    void tearDown();

private:
    Accessor* accessor;
    void testGetSampleCount();
    void testSetByteGetByte();
    void testSetDoubleGetByte();
    void testSetFloatGetByte();
    void testSetIntGetByte();
    void testSetUByteGetByte();
    void testGetByteData();
    void testGetOtherTypeData();
    void testGetUntypedValue();
};

#endif /* MAPACCESSORTEST_H_ */
