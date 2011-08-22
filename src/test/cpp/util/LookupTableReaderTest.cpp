/*
 * LookupTableReaderTest.cpp
 *
 *  Created on: Aug 22, 2011
 *      Author: ralf
 */

#include "../../../main/cpp/util/LookupTableReader.h"

#include "LookupTableReaderTest.h"

const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");

LookupTableReaderTest::LookupTableReaderTest() {
}

LookupTableReaderTest::~LookupTableReaderTest() {
}

void LookupTableReaderTest::setUp() {
}

void LookupTableReaderTest::tearDown() {
}

void LookupTableReaderTest::testA() {
	const LookupTableReader reader(S3_SYNERGY_HOME + "/data/SYN/aux/v1/S3__SY_2_SYRTAX.nc");

	const LookupTable<float>* lut = reader.readLookupTable<float>("OLC_R_atm");

	CPPUNIT_ASSERT(lut != 0);
}

void LookupTableReaderTest::testB() {

}
