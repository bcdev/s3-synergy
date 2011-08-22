/*
 * LookupTableReaderTest.h
 *
 *  Created on: Aug 22, 2011
 *      Author: ralf
 */

#ifndef LOOKUPTABLEREADERTEST_H_
#define LOOKUPTABLEREADERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class LookupTableReaderTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(LookupTableReaderTest);
    CPPUNIT_TEST(testA);
    CPPUNIT_TEST(testB);
    CPPUNIT_TEST_SUITE_END();

public:
    LookupTableReaderTest();
    virtual ~LookupTableReaderTest();
    void setUp();
    void tearDown();
private:

    void testA();
    void testB();
};

#endif /* LOOKUPTABLEREADERTEST_H_ */
