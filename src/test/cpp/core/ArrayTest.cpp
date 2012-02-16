/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
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

	shared_ptr<Array<double> > array = Array<double>::newArray(byteArray, 1.0, 0.0);
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

	shared_ptr<Array<double> > array = Array<double>::newArray(byteArray, 0.1, 0.0);

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

	shared_ptr<Array<double> > array = Array<double>::newArray(doubleArray, 1.0, 0.0);
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

	shared_ptr<Array<double> > array = Array<double>::newArray(doubleArray, 2.0, 0.0);

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

