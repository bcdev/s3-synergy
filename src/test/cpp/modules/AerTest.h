/* 
 * File:   AerTest.h
 * Author: thomasstorm
 *
 * Created on Sep 22, 2011
 */

#ifndef AERTEST_H
#define	AERTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"

class AerTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(AerTest);
    CPPUNIT_TEST(testAer);
    CPPUNIT_TEST_SUITE_END();

public:
    AerTest();
    virtual ~AerTest();
    void setUp();
    void tearDown();

private:
    void prepareContext();
    void testAer();
    shared_ptr<Context> context;
    shared_ptr<Aer> aer;
};

#endif	/* AERTEST_H */

