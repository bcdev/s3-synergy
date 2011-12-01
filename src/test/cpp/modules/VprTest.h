/* 
 * File:   VprTest.h
 * Author: thomasstorm
 *
 * Created on Dec 01, 2011
 */

#ifndef VPRTEST_H
#define	VPRTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"
#include "../../../../src/main/cpp/modules/Vpr.h"

class VprTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VprTest);
    CPPUNIT_TEST(testGetLatitude);
    CPPUNIT_TEST(testGetLongitude);
    CPPUNIT_TEST_SUITE_END();

public:
    VprTest();
    virtual ~VprTest();
    void setUp();
    void tearDown();

private:
    shared_ptr<Context> context;

    void prepareContext();
    void testGetLatitude();
    void testGetLongitude();
};

#endif	/* VPRTEST_H */

