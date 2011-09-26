/* 
 * File:   AcoTest.h
 * Author: ralf
 *
 * Created on August 16, 2011, 1:36 PM
 */

#ifndef ACOTEST_H
#define	ACOTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"

class AcoTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(AcoTest);
    CPPUNIT_TEST(testAco);
    CPPUNIT_TEST_SUITE_END();

public:
    AcoTest();
    virtual ~AcoTest();
    void setUp();
    void tearDown();

private:
    void prepareContext();
    void testAco();
    shared_ptr<Context> context;
};

#endif	/* ACOTEST_H */

