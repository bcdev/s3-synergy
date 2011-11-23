/* 
 * File:   Syl2_1_Test.h
 * Author: ralf
 *
 * Created on August 16, 2011, 1:36 PM
 */

#ifndef SYL2_2_TEST_H
#define	SYL2_2_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class Syl2_1_Test : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(Syl2_1_Test);
    CPPUNIT_TEST(testParseJobOrder1);
    CPPUNIT_TEST_SUITE_END();

public:
    Syl2_1_Test();
    virtual ~Syl2_1_Test();
    void setUp();
    void tearDown();

private:
    void testParseJobOrder1();
};

#endif	/* SYL2_2_TEST_H */

