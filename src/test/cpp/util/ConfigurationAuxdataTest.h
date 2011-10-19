/*
 * ConfigurationAuxdataTest.h
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#ifndef CONFIGURATIONAUXDATATEST_H_
#define CONFIGURATIONAUXDATATEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/Boost.h"
#include "../../../main/cpp/util/ConfigurationAuxdataProvider.h"

class ConfigurationAuxdataTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(ConfigurationAuxdataTest);
    CPPUNIT_TEST(testGetAMINs);
    CPPUNIT_TEST(testGetInitialTau550);
    CPPUNIT_TEST(testGetInitialNus);
    CPPUNIT_TEST(testGetInitialOmegas);
    CPPUNIT_TEST_SUITE_END();

public:
    ConfigurationAuxdataTest();
    virtual ~ConfigurationAuxdataTest();
    void setUp();
    void tearDown();

private:
    void testGetAMINs();
    void testGetInitialTau550();
    void testGetInitialNus();
    void testGetInitialOmegas();
    shared_ptr<ConfigurationAuxdataProvider> auxdataProvider;
    static const double EPSILON;
};

#endif /* CONFIGURATIONAUXDATATEST_H_ */
