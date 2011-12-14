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
//    CPPUNIT_TEST(testComputeT550);
//    CPPUNIT_TEST(testSurfaceReflectance);
//    CPPUNIT_TEST(testHyperspectralUpscale);
    CPPUNIT_TEST(testVbmIntegration);
    CPPUNIT_TEST_SUITE_END();

public:
    VbmTest();
    virtual ~VbmTest();
    void setUp();
    void tearDown();

private:
    shared_ptr<Context> context;
    shared_ptr<Vbm> vbm;

    void prepareContext();
    void testComputeT550();
    void testSurfaceReflectance();
    void testHyperspectralUpscale();
    void testVbmIntegration();
};

#endif	/* VBMTEST_H */

