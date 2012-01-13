/*
 * AbstractWriterTest.h
 *
 *  Created on: Sep 2, 2011
 *      Author: ralf
 */

#ifndef ABSTRACTWRITERTEST_H_
#define ABSTRACTWRITERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"

class AbstractWriterTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(AbstractWriterTest);
    CPPUNIT_TEST(testReplacing);
    CPPUNIT_TEST_SUITE_END();

public:
    AbstractWriterTest();
    virtual ~AbstractWriterTest();
    void setUp();
    void tearDown();

protected:
    void testReplacing();
    void prepareContext();
    shared_ptr<Context> context;
};

#endif /* ABSTRACTWRITERTEST_H_ */
