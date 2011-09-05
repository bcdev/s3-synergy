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
	reader = new LookupTableReader(
			IOUtils::getEnvironment("S3_SYNERGY_HOME")
					+ "/auxdata/v1/S3__SY_2_SYRTAX.nc");
}

void LookupTableReaderTest::tearDown() {
	delete reader;
}

void LookupTableReaderTest::testRead_OLC_R_atm() {
	shared_ptr<const LookupTable<float> > lut(
			reader->readLookupTable<float>("OLC_R_atm"));

	CPPUNIT_ASSERT(lut != 0);
	CPPUNIT_ASSERT(lut->getId().compare("OLC_R_atm") == 0);
	CPPUNIT_ASSERT(lut->getDimensionCount() == 8);
	CPPUNIT_ASSERT(lut->getScaleFactor() == 0.004f);
	CPPUNIT_ASSERT(lut->getAddOffset() == 0.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(0) == 0.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(0) == 180.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(1) == 0.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(1) == 70.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(2) == 0.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(2) == 55.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(3) == 800.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(3) == 1030.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(4) == 0.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(4) == 5.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(5) == 0.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(5) == 4.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(6) == 1.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(6) == 40.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(7) == 1.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(7) == 18.0f);

	valarray<float> coordinates(8);
	coordinates[3] = 800.0f;
	coordinates[6] = 1.0f;
	coordinates[7] = 1.0f;

	const float result = lut->operator()(&coordinates[0]);

	CPPUNIT_ASSERT(result >= 0.0f);
}

void LookupTableReaderTest::testRead_SLN_R_atm() {
	const shared_ptr<LookupTable<float> > lut(
			reader->readLookupTable<float>("SLN_R_atm"));

	CPPUNIT_ASSERT(lut != 0);
	CPPUNIT_ASSERT(lut->getId().compare("SLN_R_atm") == 0);
	CPPUNIT_ASSERT(lut->getDimensionCount() == 8);
	CPPUNIT_ASSERT(lut->getScaleFactor() == 0.004f);
	CPPUNIT_ASSERT(lut->getAddOffset() == 0.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(0) == 0.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(0) == 180.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(1) == 0.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(1) == 70.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(2) == 6.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(2) == 58.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(3) == 800.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(3) == 1030.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(4) == 0.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(4) == 5.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(5) == 0.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(5) == 4.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(6) == 1.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(6) == 40.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(7) == 19.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(7) == 24.0f);
}

void LookupTableReaderTest::testRead_SLO_R_atm() {
	const shared_ptr<LookupTable<float> > lut(
			reader->readLookupTable<float>("SLO_R_atm"));

	CPPUNIT_ASSERT(lut != 0);
	CPPUNIT_ASSERT(lut->getId().compare("SLO_R_atm") == 0);
	CPPUNIT_ASSERT(lut->getDimensionCount() == 8);
	CPPUNIT_ASSERT(lut->getScaleFactor() == 0.004f);
	CPPUNIT_ASSERT(lut->getAddOffset() == 0.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(0) == 0.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(0) == 180.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(1) == 0.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(1) == 70.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(2) == 55.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(2) == 55.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(3) == 800.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(3) == 1030.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(4) == 0.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(4) == 5.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(5) == 0.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(5) == 4.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(6) == 1.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(6) == 40.0f);

	CPPUNIT_ASSERT(lut->minCoordinate(7) == 25.0f);
	CPPUNIT_ASSERT(lut->maxCoordinate(7) == 30.0f);
}
