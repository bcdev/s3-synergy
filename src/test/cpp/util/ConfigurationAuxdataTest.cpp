/*
 * ConfigurationAuxdataTest.cpp
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#include <string>

#include "../../../main/cpp/core/Constants.h"
#include "ConfigurationAuxdataTest.h"

using std::string;

CPPUNIT_TEST_SUITE_REGISTRATION(ConfigurationAuxdataTest);

const double ConfigurationAuxdataTest::EPSILON = 1.0E-6;

ConfigurationAuxdataTest::ConfigurationAuxdataTest() {
}

ConfigurationAuxdataTest::~ConfigurationAuxdataTest() {
}

void ConfigurationAuxdataTest::setUp() {
    const char* value = std::getenv("S3_SYNERGY_HOME");

    string auxdataPath = string(value) + "/auxdata/v" + Constants::PROCESSOR_VERSION + "/";
    auxdataProvider = shared_ptr<ConfigurationAuxdataProvider>(new ConfigurationAuxdataProvider(auxdataPath));
}

void ConfigurationAuxdataTest::tearDown() {

}

void ConfigurationAuxdataTest::testGetAMINs() {
    const valarray<int16_t> amins = auxdataProvider->getAMINs();
    CPPUNIT_ASSERT(amins[0] == 1);
    CPPUNIT_ASSERT(amins[20] == 21);
    CPPUNIT_ASSERT(amins[39] == 40);
    const valarray<int16_t> amins2 = auxdataProvider->getAMINs();
    CPPUNIT_ASSERT(amins2[0] == 1);
    CPPUNIT_ASSERT(amins2[20] == 21);
    CPPUNIT_ASSERT(amins2[39] == 40);
}

void ConfigurationAuxdataTest::testGetInitialTau550() {
    const float initialTau550 = auxdataProvider->getInitialTau550();
    CPPUNIT_ASSERT(initialTau550 == 0.1f);
    const float initialTau5502 = auxdataProvider->getInitialTau550();
    CPPUNIT_ASSERT(initialTau5502 == 0.1f);
}

void ConfigurationAuxdataTest::testGetInitialNus() {
    const valarray<float> initialNus = auxdataProvider->getInitialNus();
    CPPUNIT_ASSERT(initialNus.size() == 2);
    CPPUNIT_ASSERT(std::abs(initialNus[0] - 0.5) < EPSILON);
    CPPUNIT_ASSERT(std::abs(initialNus[1] - 0.3) < EPSILON);
    const valarray<float> initialNus2 = auxdataProvider->getInitialNus();
    CPPUNIT_ASSERT(initialNus2.size() == 2);
    CPPUNIT_ASSERT(std::abs(initialNus2[0] - 0.5) < EPSILON);
    CPPUNIT_ASSERT(std::abs(initialNus2[1] - 0.3) < EPSILON);
}

void ConfigurationAuxdataTest::testGetInitialOmegas() {
    const valarray<float> initialOmegas = auxdataProvider->getInitialOmegas();
    CPPUNIT_ASSERT(initialOmegas.size() == 6);
    for(size_t i = 0; i < initialOmegas.size(); i++) {
        CPPUNIT_ASSERT(std::abs(initialOmegas[i] - 0.1) < EPSILON);
    }
    const valarray<float> initialOmegas2 = auxdataProvider->getInitialOmegas();
    CPPUNIT_ASSERT(initialOmegas2.size() == 6);
    for(size_t i = 0; i < initialOmegas2.size(); i++) {
        CPPUNIT_ASSERT(std::abs(initialOmegas2[i] - 0.1) < EPSILON);
    }
}
