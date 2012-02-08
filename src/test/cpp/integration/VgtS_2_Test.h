/* 
 * File:   VgtS_2_Test.h
 * Author: thomasstorm
 *
 * Created on January 24, 2012
 */

#ifndef VGTS_2_TEST_H
#define	VGTS_2_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class VgtS_2_Test : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VgtS_2_Test);
    CPPUNIT_TEST(testIntegrationOfVgtSModules);
    CPPUNIT_TEST_SUITE_END();

public:
    VgtS_2_Test();
    virtual ~VgtS_2_Test();
    void setUp();
    void tearDown();

private:
    void testIntegrationOfVgtSModules();
};

#endif	/* VGTS_2_TEST_H */

