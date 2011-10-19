/*
 * AuxdataProviderTest.h
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#ifndef AUXDATAPROVIDERTEST_H_
#define AUXDATAPROVIDERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/Boost.h"
#include "../../../main/cpp/util/AuxdataProvider.h"

class AuxdataProviderTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(AuxdataProviderTest);
    CPPUNIT_TEST(testGetAMINs);
    CPPUNIT_TEST(testGetInitialTau550);
    CPPUNIT_TEST(testGetInitialNus);
    CPPUNIT_TEST(testGetInitialOmegas);
    CPPUNIT_TEST_SUITE_END();

public:
    AuxdataProviderTest();
    virtual ~AuxdataProviderTest();
    void setUp();
    void tearDown();

private:
    void testGetAMINs();
    void testGetInitialTau550();
    void testGetInitialNus();
    void testGetInitialOmegas();
    shared_ptr<AuxdataProvider> auxdataProvider;
    static const double EPSILON;
};

#endif /* AUXDATAPROVIDERTEST_H_ */
