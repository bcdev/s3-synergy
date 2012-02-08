/* 
 * File:   VgtS_1_Test.h
 * Author: ralf
 *
 * Created on August 16, 2011, 1:36 PM
 */

#ifndef VGTS_1_TEST_H
#define	VGTS_1_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class VgtS_1_Test : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VgtS_1_Test);
    CPPUNIT_TEST(testParseJobOrder_01);
    CPPUNIT_TEST(testParseJobOrder_02);
    CPPUNIT_TEST_SUITE_END();

public:
    VgtS_1_Test();
    virtual ~VgtS_1_Test();
    void setUp();
    void tearDown();

private:
    void testParseJobOrder_01();
    void testParseJobOrder_02();
};

#endif	/* VGTS_1_TEST_H */

