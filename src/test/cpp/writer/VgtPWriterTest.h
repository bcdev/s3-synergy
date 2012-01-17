/*
 * VgtPWriterTest.h
 *
 *  Created on: Jan 16, 2012
 *      Author: thomasstorm
 */

#ifndef VGTPWRITERTEST_H_
#define VGTPWRITERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"

class VgtPWriterTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VgtPWriterTest);
    CPPUNIT_TEST(testWriter);
    CPPUNIT_TEST_SUITE_END();

public:
    VgtPWriterTest();
    virtual ~VgtPWriterTest();
    void setUp();
    void tearDown();

protected:
    void testWriter();
    void prepareContext();
    shared_ptr<Context> context;
};

#endif /* VGTPWRITERTEST_H_ */
