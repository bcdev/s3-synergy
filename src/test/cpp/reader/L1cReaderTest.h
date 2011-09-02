/* 
 * File:   L1cReaderTest.h
 * Author: ralf
 *
 * Created on August 16, 2011, 1:36 PM
 */

#ifndef L1CREADERTEST_H
#define	L1CREADERTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"

class L1cReaderTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(L1cReaderTest);
    CPPUNIT_TEST(testReader);
    CPPUNIT_TEST_SUITE_END();

public:
    L1cReaderTest();
    virtual ~L1cReaderTest();
    void setUp();
    void tearDown();

private:
    void testReader();
};

#endif	/* L1CREADERTEST_H */

