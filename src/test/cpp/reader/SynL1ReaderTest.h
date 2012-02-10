/* 
 * File:   SynL1ReaderTest.h
 * Author: ralf
 *
 * Created on August 16, 2011, 1:36 PM
 */

#ifndef SYNL1READERTEST_H
#define	SYNL1READERTEST_H

#include <cppunit/extensions/HelperMacros.h>

class SynL1ReaderTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(SynL1ReaderTest);
    CPPUNIT_TEST(testReader);
    CPPUNIT_TEST_SUITE_END();

public:
    SynL1ReaderTest();
    virtual ~SynL1ReaderTest();
    void setUp();
    void tearDown();

private:
    void testReader();
};

#endif	/* SYNL1READERTEST_H */

