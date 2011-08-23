/*
 * LookupTableReaderTest.h
 *
 *  Created on: Aug 22, 2011
 *      Author: ralf
 */

#ifndef LOOKUPTABLEREADERTEST_H_
#define LOOKUPTABLEREADERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/util/LookupTableReader.h"

class LookupTableReaderTest: public CPPUNIT_NS::TestFixture {
CPPUNIT_TEST_SUITE(LookupTableReaderTest);
		CPPUNIT_TEST(testRead_OLC_R_atm);
		CPPUNIT_TEST(testRead_SLN_R_atm);
		CPPUNIT_TEST(testRead_SLO_R_atm);
	CPPUNIT_TEST_SUITE_END();

public:
	LookupTableReaderTest();
	virtual ~LookupTableReaderTest();
	void setUp();
	void tearDown();
private:

	void testRead_OLC_R_atm();
	void testRead_SLN_R_atm();
	void testRead_SLO_R_atm();

	LookupTableReader* reader;
};

#endif /* LOOKUPTABLEREADERTEST_H_ */
