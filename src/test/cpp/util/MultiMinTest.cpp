/*
 * MultiMinTest.cpp
 *
 *  Created on: 28.09.2011
 *      Author: thomasstorm
 */

#include "MultiMinTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(MultiMinTest);

class Cigar: public MultivariateFunction {
public:
	double value(valarray<double>& x) {
		double sum = 0.0;

		for (size_t i = 1; i < x.size(); ++i) {
			sum += MultiMin::square(1000.0 * x[i]);
		}

		return MultiMin::square(x[0] * x[0]) + sum;
	}
};

class Rosenbrock: public MultivariateFunction {
public:
	double value(valarray<double>& x) {
		double sum = 0.0;

		for (size_t i = 0; i < x.size() - 1; ++i) {
			sum += 100.0 * MultiMin::square(MultiMin::square(x[i]) - x[i + 1]) + MultiMin::square(x[i] - 1.0);
		}

		return sum;
	}
};

MultiMinTest::MultiMinTest() {
}

MultiMinTest::~MultiMinTest() {
}

void MultiMinTest::setUp() {
}

void MultiMinTest::tearDown() {

}

void MultiMinTest::testPowellCigar() {
	double xInit[] = { 1.0, 1.0, 1.0 };
	valarray<double> x(xInit, 3);

	double eInit1[] = { 1.0, 0.0, 0.0 };
	double eInit2[] = { 0.0, 1.0, 0.0 };
	double eInit3[] = { 0.0, 0.0, 1.0 };
	valarray<double> firstE(eInit1, 3);
	valarray<double> secondE(eInit2, 3);
	valarray<double> thirdE(eInit3, 3);

	valarray<valarray<double> > e(valarray<double>(3), 3);
	e[0] = firstE;
	e[1] = secondE;
	e[2] = thirdE;

	Cigar cigar;
	const bool success = MultiMin::powell(cigar, x, e, 1.0e-8, 200);
	CPPUNIT_ASSERT(success);

	CPPUNIT_ASSERT(std::abs(0.0 - x[0]) <= 1.0e-8);
	CPPUNIT_ASSERT(std::abs(0.0 - x[1]) <= 1.0e-8);
	CPPUNIT_ASSERT(std::abs(0.0 - x[2]) <= 1.0e-8);
}

void MultiMinTest::testPowellRosenbrock() {
	double xInit[] = { 0.0, 0.0, 0.0 };
	valarray<double> x(xInit, 3);

	double eInit1[] = { 1.0, 0.0, 0.0 };
	double eInit2[] = { 0.0, 1.0, 0.0 };
	double eInit3[] = { 0.0, 0.0, 1.0 };
	valarray<double> firstE(eInit1, 3);
	valarray<double> secondE(eInit2, 3);
	valarray<double> thirdE(eInit3, 3);

	valarray<valarray<double> > e(valarray<double>(3), 3);
	e[0] = firstE;
	e[1] = secondE;
	e[2] = thirdE;

	Rosenbrock rosenbrock;
	const bool success = MultiMin::powell(rosenbrock, x, e, 1.0e-8, 200);
	CPPUNIT_ASSERT(success);
	CPPUNIT_ASSERT(std::abs(1.0 - x[0]) <= 1.0e-8);
	CPPUNIT_ASSERT(std::abs(1.0 - x[1]) <= 1.0e-8);
	CPPUNIT_ASSERT(std::abs(1.0 - x[2]) <= 1.0e-8);
}
