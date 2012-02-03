/*
 * LookupTableTest.cpp
 *
 *  Created on: Nov 1, 2011
 *      Author: ralf
 */

#include <cstdlib>
#include <cmath>

#include "../../../main/cpp/core/LookupTable.h"

#include "LookupTableTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(LookupTableTest);

LookupTableTest::LookupTableTest() {
}

LookupTableTest::~LookupTableTest() {
}

void LookupTableTest::setUp() {
}

void LookupTableTest::tearDown() {
}

void LookupTableTest::testInterpolation1D() {
	vector<valarray<double> > dimensions(1, valarray<double>(2));
	dimensions[0][0] = 0.0;
	dimensions[0][1] = 1.0;

	shared_array<double> values = shared_array<double>(new double[2]);
	values[0] = 0.0;
	values[1] = 1.0;

	shared_ptr<LookupTable<double> > lut = LookupTable<double>::newLookupTable("id", dimensions, values);
	CPPUNIT_ASSERT(lut->getDimensionCount() == 1);

	CPPUNIT_ASSERT(lut->getMinCoordinate(0) == 0.0);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(0) == 1.0);

	double coordinate;
	valarray<double> f(lut->getDimensionCount());
	valarray<double> workspace(lut->getScalarWorkspaceSize());

	coordinate = 0.0;
	CPPUNIT_ASSERT(lut->getScalar(&coordinate, f, workspace) == 0.0);
	coordinate = 1.0;
	CPPUNIT_ASSERT(lut->getScalar(&coordinate, f, workspace) == 1.0);
	coordinate = 0.5;
	CPPUNIT_ASSERT(lut->getScalar(&coordinate, f, workspace) == 0.5);
}

void LookupTableTest::testInterpolation2D() {
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

	shared_ptr<LookupTable<double> > lut = LookupTable<double>::newLookupTable("id", dimensions, values);
	CPPUNIT_ASSERT(lut->getDimensionCount() == 2);

	CPPUNIT_ASSERT(lut->getMinCoordinate(0) == 0.0);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(0) == 1.0);
	CPPUNIT_ASSERT(lut->getMinCoordinate(1) == 0.0);
	CPPUNIT_ASSERT(lut->getMaxCoordinate(1) == 1.0);

	double coordinates[2];
	valarray<double> f(lut->getDimensionCount());
	valarray<double> workspace(lut->getScalarWorkspaceSize());

	coordinates[0] = 0.0;
	coordinates[1] = 0.0;
	CPPUNIT_ASSERT(lut->getScalar(coordinates, f, workspace) == 0.0);

	coordinates[0] = 0.0;
	coordinates[1] = 1.0;
	CPPUNIT_ASSERT(lut->getScalar(coordinates, f, workspace) == 1.0);

	coordinates[0] = 1.0;
	coordinates[1] = 0.0;
	CPPUNIT_ASSERT(lut->getScalar(coordinates, f, workspace) == 2.0);

	coordinates[0] = 1.0;
	coordinates[1] = 1.0;
	CPPUNIT_ASSERT(lut->getScalar(coordinates, f, workspace) == 3.0);

	coordinates[0] = 0.0;
	coordinates[1] = 0.5;
	CPPUNIT_ASSERT(lut->getScalar(coordinates, f, workspace) == 0.5);

	coordinates[0] = 0.5;
	coordinates[1] = 0.5;
	CPPUNIT_ASSERT(lut->getScalar(coordinates, f, workspace) == 1.5);

	coordinates[0] = 1.0;
	coordinates[1] = 0.5;
	CPPUNIT_ASSERT(lut->getScalar(coordinates, f, workspace) == 2.5);
}

void LookupTableTest::testInterpolation3D() {
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

	shared_ptr<LookupTable<double> > lut = LookupTable<double>::newLookupTable("id", dimensions, values);
	CPPUNIT_ASSERT(lut->getDimensionCount() == 3);

	double r[3];
	double x[3];
	valarray<double> f(lut->getDimensionCount());
	valarray<double> workspace(lut->getScalarWorkspaceSize());

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

		actual = lut->getScalar(x, f, workspace);
		CPPUNIT_ASSERT(abs(expected - actual) < 1.0E-10);
	}
}

void LookupTableTest::testInterpolation4D() {
	using std::abs;
	using std::rand;
	using std::srand;

	vector<valarray<double> > dimensions(4, valarray<double>(5));
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
	dimensions[3][0] = 3.0;
	dimensions[3][1] = 4.0;
	dimensions[3][2] = 5.0;
	dimensions[3][3] = 6.0;
	dimensions[3][4] = 7.0;

	shared_array<double> values = shared_array<double>(new double[625]);
	for (size_t i = 0; i < 625; ++i) {
		values[i] = i;
	}

	shared_ptr<LookupTable<double> > lut = LookupTable<double>::newLookupTable("id", dimensions, values);
	CPPUNIT_ASSERT(lut->getDimensionCount() == 4);

	double r[4];
	double x[4];
	valarray<double> f(lut->getDimensionCount());
	valarray<double> workspace(lut->getScalarWorkspaceSize());

	srand(5483);

	for (size_t i = 0; i < 10; ++i) {
		// compute random coordinates
		for (size_t j = 0; j < 4; ++j) {
			const double random = rand() / double(RAND_MAX);
			r[j] = random * (lut->getMaxCoordinate(j) - lut->getMinCoordinate(j));
			x[j] = r[j] + lut->getMinCoordinate(j);
		}

		const double expected = r[3] + 5.0 * (r[2] + 5.0 * (r[1] + 5.0 * r[0]));
		double actual;

		actual = lut->getScalar(x, f, workspace);
		CPPUNIT_ASSERT(abs(expected - actual) < 1.0E-10);
	}
}
