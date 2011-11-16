/* 
 * File:   Syl2_2_Test.h
 * Author: ralf
 *
 * Created on August 16, 2011, 1:36 PM
 */

#ifndef SYL2_2_TEST_H
#define	SYL2_2_TEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"

class Syl2_2_Test : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(Syl2_2_Test);
    CPPUNIT_TEST(testIntegrationOfSynL2Modules);
    CPPUNIT_TEST_SUITE_END();

public:
    Syl2_2_Test();
    virtual ~Syl2_2_Test();
    void setUp();
    void tearDown();

private:
    void prepareContext();
    void testIntegrationOfSynL2Modules();
    shared_ptr<Context> context;
};

#endif	/* SYL2_2_TEST_H */

