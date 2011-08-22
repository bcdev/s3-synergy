/*
 * ArrayTest.cpp
 *
 *  Created on: Aug 19, 2011
 *      Author: ralf
 */

#include "ArrayTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ArrayTest);

ArrayTest::ArrayTest() {
}

ArrayTest::~ArrayTest() {
}

void ArrayTest::setUp() {
}

void ArrayTest::tearDown() {
}

void ArrayTest::testByteArray() {
	valarray<uint8_t> byteArray(2);

	Array<double>* array = Array<double>::newArray(byteArray, 1.0, 0.0);
	CPPUNIT_ASSERT(array->size() == 2);
	CPPUNIT_ASSERT(array->get(0) == 0.0);
	CPPUNIT_ASSERT(array->get(1) == 0.0);
	CPPUNIT_ASSERT(array->getScaleFactor() == 1.0);
	CPPUNIT_ASSERT(array->getAddOffset() == 0.0);

	array->set(0, 1.0);
	array->set(1, 7.0);

	CPPUNIT_ASSERT(array->get(0) == 1.0);
	CPPUNIT_ASSERT(array->get(1) == 7.0);
}

void ArrayTest::testByteArrayScaled() {
	valarray<uint8_t> byteArray(2);

	Array<double>* array = Array<double>::newArray(byteArray, 0.1, 0.0);

	CPPUNIT_ASSERT(array->size() == 2);
	CPPUNIT_ASSERT(array->get(0) == 0.0);
	CPPUNIT_ASSERT(array->get(1) == 0.0);
	CPPUNIT_ASSERT(array->getScaleFactor() == 0.1);
	CPPUNIT_ASSERT(array->getAddOffset() == 0.0);

	array->set(0, 1.0);
	array->set(1, 7.0);

	CPPUNIT_ASSERT(array->get(0) == 1.0);
	CPPUNIT_ASSERT(array->get(1) == 7.0);

	uint8_t* buffer = (uint8_t*) array->getUntypedData();

	CPPUNIT_ASSERT(buffer[0] == 10);
	CPPUNIT_ASSERT(buffer[1] == 70);
}

void ArrayTest::testDoubleArray() {
	valarray<double> doubleArray(2);

	Array<double>* array = Array<double>::newArray(doubleArray, 1.0, 0.0);
	CPPUNIT_ASSERT(array->size() == 2);
	CPPUNIT_ASSERT(array->get(0) == 0.0);
	CPPUNIT_ASSERT(array->get(1) == 0.0);
	CPPUNIT_ASSERT(array->getScaleFactor() == 1.0);
	CPPUNIT_ASSERT(array->getAddOffset() == 0.0);

	array->set(0, 1.0);
	array->set(1, 7.0);

	CPPUNIT_ASSERT(array->get(0) == 1.0);
	CPPUNIT_ASSERT(array->get(1) == 7.0);
}

void ArrayTest::testDoubleArrayScaled() {
	valarray<double> doubleArray(2);

	Array<double>* array = Array<double>::newArray(doubleArray, 2.0, 0.0);

	CPPUNIT_ASSERT(array->size() == 2);
	CPPUNIT_ASSERT(array->get(0) == 0.0);
	CPPUNIT_ASSERT(array->get(1) == 0.0);
	CPPUNIT_ASSERT(array->getScaleFactor() == 2.0);
	CPPUNIT_ASSERT(array->getAddOffset() == 0.0);

	array->set(0, 1.0);
	array->set(1, 7.0);

	CPPUNIT_ASSERT(array->get(0) == 1.0);
	CPPUNIT_ASSERT(array->get(1) == 7.0);

	double* buffer = (double*) array->getUntypedData();

	CPPUNIT_ASSERT(buffer[0] == 0.5);
	CPPUNIT_ASSERT(buffer[1] == 3.5);
}

