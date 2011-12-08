/*
 * AuxdataProviderTest.cpp
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#include <string>
#include <valarray>

#include "../../../main/cpp/core/Constants.h"
#include "AuxdataProviderTest.h"

using std::string;
using std::valarray;

CPPUNIT_TEST_SUITE_REGISTRATION(AuxdataProviderTest);

AuxdataProviderTest::AuxdataProviderTest() {
}

AuxdataProviderTest::~AuxdataProviderTest() {
}

void AuxdataProviderTest::setUp() {
	const char* value = std::getenv("S3_SYNERGY_HOME");
	const string filePath = string(value) + "/auxdata/v" + Constants::PROCESSOR_VERSION + "/S3__SY_2_SYCPAX.nc";
	auxdataProvider = shared_ptr<AuxdataProvider>(new AuxdataProvider("id", filePath));
}

void AuxdataProviderTest::tearDown() {

}

void AuxdataProviderTest::testGetAMINs() {
	valarray<int16_t> amins;
	auxdataProvider->getVectorShort("AMIN", amins);

	CPPUNIT_ASSERT(amins.size() == 3);
	CPPUNIT_ASSERT(amins[0] == 1);
	CPPUNIT_ASSERT(amins[1] == 2);
	CPPUNIT_ASSERT(amins[2] == 3);
}

void AuxdataProviderTest::testGetInitialTau550() {
	double initialTau550;
	auxdataProvider->getDouble("T550_ini", initialTau550);

	CPPUNIT_ASSERT(initialTau550 == (double) 0.1f);
}

void AuxdataProviderTest::testGetInitialNus() {
	valarray<double> initialNus;
	auxdataProvider->getVectorDouble("v_ini", initialNus);

	CPPUNIT_ASSERT(initialNus.size() == 2);

	CPPUNIT_ASSERT(initialNus[0] == (double) 0.5f);
	CPPUNIT_ASSERT(initialNus[1] == (double) 0.3f);
}

void AuxdataProviderTest::testGetInitialOmegas() {
	valarray<double> initialOmegas;
	auxdataProvider->getVectorDouble("w_ini", initialOmegas);

	CPPUNIT_ASSERT(initialOmegas.size() == 6);

	for (size_t i = 0; i < initialOmegas.size(); i++) {
		CPPUNIT_ASSERT(initialOmegas[i] == (double) 0.1f);
	}
}

void AuxdataProviderTest::testGetAngularWeights() {
	matrix<double> angWeights;
	auxdataProvider->getMatrixDouble("weight_ang", angWeights);

	CPPUNIT_ASSERT(angWeights.size1() == 2);
	CPPUNIT_ASSERT(angWeights.size2() == 6);

	CPPUNIT_ASSERT(angWeights(0, 0) == 1.5);
	CPPUNIT_ASSERT(angWeights(0, 1) == 1.0);
	CPPUNIT_ASSERT(angWeights(0, 2) == 0.5);
	CPPUNIT_ASSERT(angWeights(0, 5) == 1.0);
}
