/*
 * VgtWriterTest.h
 *
 *  Created on: Jan 16, 2012
 *      Author: thomasstorm
 */

#ifndef VGTWRITERTEST_H_
#define VGTWRITERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"

class VgtWriterTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VgtWriterTest);
    CPPUNIT_TEST(testWriter);
    CPPUNIT_TEST_SUITE_END();

public:
    VgtWriterTest();
    virtual ~VgtWriterTest();
    void setUp();
    void tearDown();

protected:
    void testWriter();
    void prepareContext();
    shared_ptr<Context> context;
};

#endif /* VGTWRITERTEST_H_ */
