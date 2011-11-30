/* 
 * File:   VflTest.h
 * Author: thomasstorm
 *
 * Created on Nov 17, 2011
 */

#ifndef VFLTEST_H
#define	VFLTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"
#include "../../../../src/main/cpp/modules/Vfl.h"

class VflTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VflTest);
    CPPUNIT_TEST_SUITE_END();

public:
    VflTest();
    virtual ~VflTest();
    void setUp();
    void tearDown();

private:
    shared_ptr<Context> context;
    shared_ptr<Vfl> vbm;

    void prepareContext();
};

#endif	/* VFLTEST_H */

