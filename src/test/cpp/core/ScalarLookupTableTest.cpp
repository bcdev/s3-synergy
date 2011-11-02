/*
 * ScalarLookupTableTest.cpp
 *
 *  Created on: Nov 1, 2011
 *      Author: ralf
 */

#include <cstdlib>
#include <cmath>

#include "ScalarLookupTableTest.h"
#include "../../../main/cpp/core/LookupTable.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ScalarLookupTableTest);

ScalarLookupTableTest::ScalarLookupTableTest() {
}

ScalarLookupTableTest::~ScalarLookupTableTest() {
}

void ScalarLookupTableTest::setUp() {
}

void ScalarLookupTableTest::tearDown() {
}

void ScalarLookupTableTest::testInterpolation1D() {
	vector<valarray<double> > dimensions(1, valarray<double>(2));
	dimensions[0][0] = 0.0;
	dimensions[0][1] = 1.0;

	shared_array<double> values = shared_array<double>(new double[2]);
	values[0] = 0.0;
	values[1] = 1.0;

	shared_ptr<ScalarLookupTable<double> > lut = ScalarLookupTable<double>::newScalarLookupTable("id", dimensions, values);
	CPPUNIT_ASSERT(lut->getDimensionCount() == 1);

	CPPUNIT_ASSERT(lut->getMinCoordinate(0) == 0.0);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(0) == 1.0);

	double coordinate;

	coordinate = 0.0;
	CPPUNIT_ASSERT(lut->getValue(&coordinate) == 0.0);
	coordinate = 1.0;
	CPPUNIT_ASSERT(lut->getValue(&coordinate) == 1.0);
	coordinate = 0.5;
	CPPUNIT_ASSERT(lut->getValue(&coordinate) == 0.5);
}

void ScalarLookupTableTest::testInterpolation2D() {
	vector<valarray<double> > dimensions(2, valarray<double>(2));
	dimensions[0][0] = 0.0;
	dimensions[0][1] = 1.0;
	dimensions[1][0] = 0.0;
	dimensions[1][1] = 1.0;

	shared_array<double> values = shared_array<double>(new double[4]);
	values[0] = 0.0;
	values[1] = 1.0;
	values[2] = 2.0;
	values[3] = 3.0;

	shared_ptr<ScalarLookupTable<double> > lut = ScalarLookupTable<double>::newScalarLookupTable("id", dimensions, values);
	CPPUNIT_ASSERT(lut->getDimensionCount() == 2);

	CPPUNIT_ASSERT(lut->getMinCoordinate(0) == 0.0);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(0) == 1.0);
	CPPUNIT_ASSERT(lut->getMinCoordinate(1) == 0.0);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(1) == 1.0);

	double coordinates[2];

	coordinates[0] = 0.0;
	coordinates[1] = 0.0;
	CPPUNIT_ASSERT(lut->getValue(coordinates) == 0.0);

	coordinates[0] = 0.0;
	coordinates[1] = 1.0;
	CPPUNIT_ASSERT(lut->getValue(coordinates) == 1.0);

	coordinates[0] = 1.0;
	coordinates[1] = 0.0;
	CPPUNIT_ASSERT(lut->getValue(coordinates) == 2.0);

	coordinates[0] = 1.0;
	coordinates[1] = 1.0;
	CPPUNIT_ASSERT(lut->getValue(coordinates) == 3.0);

	coordinates[0] = 0.0;
	coordinates[1] = 0.5;
	CPPUNIT_ASSERT(lut->getValue(coordinates) == 0.5);

	coordinates[0] = 0.5;
	coordinates[1] = 0.5;
	CPPUNIT_ASSERT(lut->getValue(coordinates) == 1.5);

	coordinates[0] = 1.0;
	coordinates[1] = 0.5;
	CPPUNIT_ASSERT(lut->getValue(coordinates) == 2.5);

	valarray<double> t(lut->getStride(0));
	valarray<double> w(lut->getStride(1));

	lut->getTable(coordinates, 1, t);
	CPPUNIT_ASSERT(lut->getValue(coordinates, 1, t, w) == 2.5);
}

void ScalarLookupTableTest::testInterpolation3D() {
	using std::abs;
	using std::rand;
	using std::srand;

	vector<valarray<double> > dimensions(3, valarray<double>(5));
	dimensions[0][0] = 0.0;
	dimensions[0][1] = 1.0;
	dimensions[0][2] = 2.0;
	dimensions[0][3] = 3.0;
	dimensions[0][4] = 4.0;
	dimensions[1][0] = 1.0;
	dimensions[1][1] = 2.0;
	dimensions[1][2] = 3.0;
	dimensions[1][3] = 4.0;
	dimensions[1][4] = 5.0;
	dimensions[2][0] = 2.0;
	dimensions[2][1] = 3.0;
	dimensions[2][2] = 4.0;
	dimensions[2][3] = 5.0;
	dimensions[2][4] = 6.0;

	shared_array<double> values = shared_array<double>(new double[125]);
	for (size_t i = 0; i < 125; ++i) {
		values[i] = i;
	}

	shared_ptr<ScalarLookupTable<double> > lut = ScalarLookupTable<double>::newScalarLookupTable("id", dimensions, values);
	CPPUNIT_ASSERT(lut->getDimensionCount() == 3);

	double r[3];
	double x[3];

	srand(5483);

	for (size_t i = 0; i < 10; ++i) {
		// compute random coordinates
		for (size_t j = 0; j < 3; ++j) {
			const double random = rand() / double(RAND_MAX);
			r[j] = random * (lut->getMaxCoordinate(j) - lut->getMinCoordinate(j));
			x[j] = r[j] + lut->getMinCoordinate(j);
		}

		const double expected = r[2] + 5.0 * (r[1] + 5.0 * r[0]);
		double actual;

		actual = lut->getValue(x);
		CPPUNIT_ASSERT(abs(expected - actual) < 1.0E-10);

		valarray<double> t(lut->getStride(0));
		valarray<double> w(lut->getStride(1));

		lut->getTable(x, 1, t);

		actual = lut->getValue(x, 1, t, w);
		CPPUNIT_ASSERT(abs(expected - actual) < 1.0E-10);

		lut->getTable(x, 2, t);

		actual = lut->getValue(x, 2, t, w);
		CPPUNIT_ASSERT(abs(expected - actual) < 1.0E-10);
	}
}
