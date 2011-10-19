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
    const valarray<int16_t> amins = auxdataProvider->getInt16TArray("AMIN");
    CPPUNIT_ASSERT(amins[0] == 1);
    CPPUNIT_ASSERT(amins[20] == 21);
    CPPUNIT_ASSERT(amins[39] == 40);
}

void ConfigurationAuxdataTest::testGetInitialTau550() {
    const float initialTau550 = auxdataProvider->getFloat("T550_ini");
    CPPUNIT_ASSERT(initialTau550 == 0.1f);
}

void ConfigurationAuxdataTest::testGetInitialNus() {
    const valarray<float> initialNus = auxdataProvider->getFloatArray("v_ini");
    CPPUNIT_ASSERT(initialNus.size() == 2);
    CPPUNIT_ASSERT(std::abs(initialNus[0] - 0.5) < EPSILON);
    CPPUNIT_ASSERT(std::abs(initialNus[1] - 0.3) < EPSILON);
}

void ConfigurationAuxdataTest::testGetInitialOmegas() {
    const valarray<float> initialOmegas = auxdataProvider->getFloatArray("w_ini");
    CPPUNIT_ASSERT(initialOmegas.size() == 6);
    for(size_t i = 0; i < initialOmegas.size(); i++) {
        CPPUNIT_ASSERT(std::abs(initialOmegas[i] - 0.1) < EPSILON);
    }
}
