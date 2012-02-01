/*
 * DirectLocatorTest.cpp
 *
 *  Created on: Dec 20, 2011
 *      Author: ralf
 */

#include "../../../main/cpp/core/DirectLocator.h"
#include "../../../main/cpp/core/DoubleAccessor.h"

#include "DirectLocatorTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(DirectLocatorTest);

DirectLocatorTest::DirectLocatorTest() {
}

DirectLocatorTest::~DirectLocatorTest() {
}

void DirectLocatorTest::setUp() {
}

void DirectLocatorTest::tearDown() {
}

void DirectLocatorTest::testRotation_Identity() {
	const Rotation r;

	double lat;
	double lon;

	lat = 0.0;
	lon = 0.0;
	r.transform(lat, lon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lat, 0.0);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lon, 0.0);

	r.inverseTransform(lat, lon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lat, 0.0);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lon, 0.0);

	lat = 7.0;
	lon = 2.0;
	r.transform(lat, lon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, lat, 1.0E-15);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, lon, 1.0E-15);

	r.inverseTransform(lat, lon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, lat, 1.0E-15);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, lon, 1.0E-15);
}

void DirectLocatorTest::testRotation_Antipode() {
	const Rotation r(0.0, 180.0);

	double lat;
	double lon;

	lat = 0.0;
	lon = 0.0;
	r.transform(lat, lon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lat, 1.0E-12);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-180.0, lon, 1.0E-12);

	r.inverseTransform(lat, lon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lat, 1.0E-12);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lon, 1.0E-12);

	lat = 7.0;
	lon = -180.0;
	r.transform(lat, lon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, lat, 1.0E-12);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lon, 1.0E-12);

	r.inverseTransform(lat, lon);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, lat, 1.0E-12);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(-180.0, lon, 1.0E-12);
}

void DirectLocatorTest::testLocation() {
	class TestGrid : public virtual Grid {
	public:
		TestGrid() {
		}

		~TestGrid() {
		}

		long getFirstK() const {
			return 0;
		}

		long getFirstL() const {
			return 0;
		}

		long getLastL() const {
			return 2;
		}

		long getFirstM() const {
			return 0;
		}

		long getSizeK() const {
			return 1;
		}

		long getSizeL() const {
			return 3;
		}

		long getSizeM() const {
			return 3;
		}

		long getSize() const {
			return getSizeK() * getSizeL() * getSizeM();
		}

		long getStrideK() const {
			return getSizeL() * getSizeM();
		}

		long getStrideL() const {
			return getSizeM();
		}

		long getStrideM() const {
			return 1;
		}

		size_t getIndex(long k, long l, long m) const throw (out_of_range) {
			return k * getStrideK() + l * getStrideL() + m * getStrideM();
		}

		long getMinK() const {
			return getFirstK();
		}

		long getMaxK() const {
			return getFirstK() + getSizeK() - 1;
		}

		long getMinL() const {
			return getFirstL();
		}

		long getMaxL() const {
			return getFirstL() + getSizeL() - 1;
		}

		long getMinM() const {
			return getFirstM();
		}

		long getMaxM() const {
			return getFirstM() + getSizeM() - 1;
		}

		bool isValidPosition(long k, long l, long m) const {
			return true;
		}

		long getK(size_t i) const {
		    return 0;
		}

		long getM(size_t i) const {
		    return 0;
		}

		long getL(size_t i) const {
		    return 0;
		}
	};

	TestGrid grid;
	DoubleAccessor latAccessor(9);
	DoubleAccessor lonAccessor(9);

	latAccessor.setDouble(0, 2.0);
	latAccessor.setDouble(1, 2.0);
	latAccessor.setDouble(2, 2.0);
	latAccessor.setDouble(3, 1.0);
	latAccessor.setDouble(4, 1.0);
	latAccessor.setDouble(5, 1.0);
	latAccessor.setDouble(6, 0.0);
	latAccessor.setDouble(7, 0.0);
	latAccessor.setDouble(8, 0.0);

	lonAccessor.setDouble(0, 0.0);
	lonAccessor.setDouble(1, 1.0);
	lonAccessor.setDouble(2, 2.0);
	lonAccessor.setDouble(3, 0.0);
	lonAccessor.setDouble(4, 1.0);
	lonAccessor.setDouble(5, 2.0);
	lonAccessor.setDouble(6, 0.0);
	lonAccessor.setDouble(7, 1.0);
	lonAccessor.setDouble(8, 2.0);

	DirectLocator dl(latAccessor, lonAccessor, grid);

	double lat;
	double lon;

	dl.getLocation(0, 0.5, 0.5, lat, lon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, lat, 1.0E-15);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lon, 1.0E-15);

	dl.getLocation(0, 1.5, 0.5, lat, lon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, lat, 1.0E-15);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, lon, 1.0E-15);

	dl.getLocation(0, 2.5, 0.5, lat, lon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, lat, 1.0E-15);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, lon, 1.0E-15);

	dl.getLocation(0, 0.5, 1.5, lat, lon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, lat, 1.0E-15);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lon, 1.0E-15);

	dl.getLocation(0, 1.5, 1.5, lat, lon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, lat, 1.0E-15);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, lon, 1.0E-15);

	dl.getLocation(0, 2.5, 1.5, lat, lon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, lat, 1.0E-15);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, lon, 1.0E-15);

	dl.getLocation(0, 0.5, 2.5, lat, lon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lat, 1.0E-15);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lon, 1.0E-15);

	dl.getLocation(0, 1.5, 2.5, lat, lon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lat, 1.0E-15);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, lon, 1.0E-15);

	dl.getLocation(0, 2.5, 2.5, lat, lon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lat, 1.0E-15);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, lon, 1.0E-15);

	dl.getLocation(0, 1.0, 1.0, lat, lon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5, lat, 1.0E-4);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, lon, 1.0E-4);

	dl.getLocation(0, 2.0, 1.0, lat, lon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5, lat, 1.0E-4);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5, lon, 1.0E-4);

	dl.getLocation(0, 1.0, 2.0, lat, lon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, lat, 1.0E-4);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, lon, 1.0E-4);

	dl.getLocation(0, 2.0, 2.0, lat, lon);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, lat, 1.0E-4);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5, lon, 1.0E-4);
}

