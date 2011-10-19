/*
 * AuxdataProviderTest.cpp
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#include <valarray>
#include <string>

#include "../../../main/cpp/core/Constants.h"
#include "AuxdataProviderTest.h"

using std::string;
using std::valarray;

CPPUNIT_TEST_SUITE_REGISTRATION(AuxdataProviderTest);

const double AuxdataProviderTest::EPSILON = 1.0E-6;

AuxdataProviderTest::AuxdataProviderTest() {
}

AuxdataProviderTest::~AuxdataProviderTest() {
}

void AuxdataProviderTest::setUp() {
    const char* value = std::getenv("S3_SYNERGY_HOME");

    string auxdataPath = string(value) + "/auxdata/v" + Constants::PROCESSOR_VERSION + "/";
    auxdataProvider = shared_ptr<AuxdataProvider>(new AuxdataProvider("id", auxdataPath));
}

void AuxdataProviderTest::tearDown() {

}

void AuxdataProviderTest::testGetAMINs() {
    const valarray<int16_t> amins = auxdataProvider->getShortArray("AMIN");
    CPPUNIT_ASSERT(amins[0] == 1);
    CPPUNIT_ASSERT(amins[20] == 21);
    CPPUNIT_ASSERT(amins[39] == 40);
}

void AuxdataProviderTest::testGetInitialTau550() {
    const double initialTau550 = auxdataProvider->getDouble("T550_ini");
    CPPUNIT_ASSERT(initialTau550 == 0.1f);
}

void AuxdataProviderTest::testGetInitialNus() {
    const valarray<double> initialNus = auxdataProvider->getDoubleArray("v_ini");
    CPPUNIT_ASSERT(initialNus.size() == 2);
    CPPUNIT_ASSERT(std::abs(initialNus[0] - 0.5) < EPSILON);
    CPPUNIT_ASSERT(std::abs(initialNus[1] - 0.3) < EPSILON);
}

void AuxdataProviderTest::testGetInitialOmegas() {
    const valarray<double> initialOmegas = auxdataProvider->getDoubleArray("w_ini");
    CPPUNIT_ASSERT(initialOmegas.size() == 6);
    for(size_t i = 0; i < initialOmegas.size(); i++) {
        CPPUNIT_ASSERT(std::abs(initialOmegas[i] - 0.1) < EPSILON);
    }
}

void AuxdataProviderTest::testGetAngularWeights() {
    matrix<double> angWeights = auxdataProvider->getDoubleMatrix("weight_ang");
    CPPUNIT_ASSERT(angWeights.size1() == 2);
    CPPUNIT_ASSERT(angWeights.size2() == 6);

    CPPUNIT_ASSERT(angWeights.at_element(0,0) == 1.5);
    CPPUNIT_ASSERT(angWeights.at_element(0,1) == 1.0);
    CPPUNIT_ASSERT(angWeights.at_element(0,2) == 0.5);
    CPPUNIT_ASSERT(angWeights.at_element(0,5) == 1.0);
}
