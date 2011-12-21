/*
 * DirectLocatorTest.h
 *
 *  Created on: Dec 20, 2011
 *      Author: ralf
 */

#ifndef DIRECTLOCATORTEST_H_
#define DIRECTLOCATORTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class DirectLocatorTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(DirectLocatorTest);
    CPPUNIT_TEST(testRotation_Identity);
    CPPUNIT_TEST(testRotation_Antipode);
    CPPUNIT_TEST(testLocation);
    CPPUNIT_TEST_SUITE_END();

public:
    DirectLocatorTest();
    virtual ~DirectLocatorTest();
    void setUp();
    void tearDown();

private:
    void testRotation_Identity();
    void testRotation_Antipode();
    void testLocation();
};

#endif /* DIRECTLOCATORTEST_H_ */
