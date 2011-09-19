/*
 * LookupTableReaderTest.cpp
 *
 *  Created on: Aug 22, 2011
 *      Author: ralf
 */

#include "../../../main/cpp/util/IOUtils.h"

#include "LookupTableReaderTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(LookupTableReaderTest);

LookupTableReaderTest::LookupTableReaderTest() {
}

LookupTableReaderTest::~LookupTableReaderTest() {
}

void LookupTableReaderTest::setUp() {
	reader = new LookupTableReader(IOUtils::getEnvironment("S3_SYNERGY_HOME") + "/auxdata/v1/S3__SY_2_SYRTAX.nc");
}

void LookupTableReaderTest::tearDown() {
	delete reader;
}

void LookupTableReaderTest::testRead_OLC_R_atm() {
	shared_ptr<const VectorLookupTable<float> > lut(reader->readVectorLookupTable<float>("OLC_R_atm"));

	CPPUNIT_ASSERT(lut != 0);
	CPPUNIT_ASSERT(lut->getId().compare("OLC_R_atm") == 0);
	CPPUNIT_ASSERT(lut->getDimensionCount() == 7);
	CPPUNIT_ASSERT(lut->getDimensionLength(0) == 31);
	CPPUNIT_ASSERT(lut->getDimensionLength(1) == 21);
	CPPUNIT_ASSERT(lut->getDimensionLength(2) == 18);
	CPPUNIT_ASSERT(lut->getDimensionLength(3) == 4);
	CPPUNIT_ASSERT(lut->getDimensionLength(4) == 3);
	CPPUNIT_ASSERT(lut->getDimensionLength(5) == 11);
	CPPUNIT_ASSERT(lut->getDimensionLength(6) == 40);
	CPPUNIT_ASSERT(lut->getStride(0) == 1 * 18 * 40 * 11 * 3 * 4 * 18 * 21);

	CPPUNIT_ASSERT(lut->getScaleFactor() == 0.004f);
	CPPUNIT_ASSERT(lut->getAddOffset() == 0.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(0) == 0.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(0) == 180.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(1) == 0.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(1) == 70.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(2) == 0.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(2) == 55.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(3) == 800.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(3) == 1030.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(4) == 0.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(4) == 5.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(5) == 0.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(5) == 4.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(6) == 1.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(6) == 40.0f);

	valarray<float> coordinates(7);
	valarray<float> values(18);

	coordinates[0] = 0.0f; // ADA
	coordinates[1] = 0.0f; // SZA
	coordinates[2] = 0.0f; // VZA
	coordinates[3] = 1000.0f; // air pressure
	coordinates[4] = 2.0f; // WV
	coordinates[5] = 0.1f; // aerosol
	coordinates[6] = 1.0f; // aerosol model index

	CPPUNIT_ASSERT( lut->getValues(&coordinates[0], values)[0] == 31 * lut->getScaleFactor());

	const float minAda = lut->getMinCoordinate(0);
	const float maxAda = lut->getMaxCoordinate(0);
	for (int i = 0; i < 31; i++) {
		coordinates[0] = minAda + i * (maxAda - minAda) / 30.0f;
		const size_t index = i * lut->getStride(0) + 2 * lut->getStride(3) + lut->getStride(4) + 2 * lut->getStride(5);

		CPPUNIT_ASSERT( lut->getValues(&coordinates[0], values)[0] == lut->getValue(index));
	}
}

void LookupTableReaderTest::testRead_SLN_R_atm() {
	const shared_ptr<ScalarLookupTable<float> > lut(reader->readScalarLookupTable<float>("SLN_R_atm"));

	CPPUNIT_ASSERT(lut != 0);
	CPPUNIT_ASSERT(lut->getId().compare("SLN_R_atm") == 0);
	CPPUNIT_ASSERT(lut->getDimensionCount() == 8);
	CPPUNIT_ASSERT(lut->getScaleFactor() == 0.004f);
	CPPUNIT_ASSERT(lut->getAddOffset() == 0.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(0) == 0.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(0) == 180.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(1) == 0.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(1) == 70.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(2) == 6.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(2) == 58.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(3) == 800.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(3) == 1030.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(4) == 0.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(4) == 5.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(5) == 0.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(5) == 4.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(6) == 1.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(6) == 40.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(7) == 19.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(7) == 24.0f);
}

void LookupTableReaderTest::testRead_SLO_R_atm() {
	const shared_ptr<ScalarLookupTable<float> > lut(reader->readScalarLookupTable<float>("SLO_R_atm"));

	CPPUNIT_ASSERT(lut != 0);
	CPPUNIT_ASSERT(lut->getId().compare("SLO_R_atm") == 0);
	CPPUNIT_ASSERT(lut->getDimensionCount() == 8);
	CPPUNIT_ASSERT(lut->getScaleFactor() == 0.004f);
	CPPUNIT_ASSERT(lut->getAddOffset() == 0.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(0) == 0.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(0) == 180.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(1) == 0.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(1) == 70.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(2) == 55.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(2) == 55.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(3) == 800.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(3) == 1030.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(4) == 0.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(4) == 5.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(5) == 0.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(5) == 4.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(6) == 1.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(6) == 40.0f);

	CPPUNIT_ASSERT(lut->getMinCoordinate(7) == 25.0f);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(7) == 30.0f);
}
