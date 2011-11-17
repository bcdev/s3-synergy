/* 
 * File:   VbmTest.h
 * Author: thomasstorm
 *
 * Created on Nov 17, 2011
 */

#ifndef VBMTEST_H
#define	VBMTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"
#include "../../../../src/main/cpp/modules/Vbm.h"

class VbmTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VbmTest);
    CPPUNIT_TEST(testComputeT550);
    CPPUNIT_TEST(testDownscaling);
    CPPUNIT_TEST_SUITE_END();

public:
    VbmTest();
    virtual ~VbmTest();
    void setUp();
    void tearDown();

private:
    void prepareContext();
    void testComputeT550();
    void testDownscaling();
    shared_ptr<Context> context;
    shared_ptr<Vbm> vbm;
};

#endif	/* VBMTEST_H */

