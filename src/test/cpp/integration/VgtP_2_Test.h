/* 
 * File:   VgtP_2_Test.h
 * Author: thomasstorm
 *
 * Created on January 24, 2012
 */

#ifndef VGTP_2_TEST_H
#define	VGTP_2_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class VgtP_2_Test : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VgtP_2_Test);
    CPPUNIT_TEST(testIntegrationOfVgtPModules);
    CPPUNIT_TEST_SUITE_END();

public:
    VgtP_2_Test();
    virtual ~VgtP_2_Test();
    void setUp();
    void tearDown();

private:
    void testIntegrationOfVgtPModules();
};

#endif	/* VGTP_2_TEST_H */

