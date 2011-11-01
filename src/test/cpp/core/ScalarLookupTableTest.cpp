/*
 * ScalarLookupTableTest.cpp
 *
 *  Created on: Nov 1, 2011
 *      Author: ralf
 */

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

	CPPUNIT_ASSERT(lut->getValue(0.0) == 0.0);
	CPPUNIT_ASSERT(lut->getValue(1.0) == 1.0);

	const double coordinate = 0.5;
	const double value = lut->getValue(&coordinate);

	CPPUNIT_ASSERT(value == 0.5);
}

    /*
    public void testInterpolation2D() {
        final double[][] dimensions = new double[][]{{0, 1}, {0, 1}};
        final double[] values = new double[]{0, 1, 2, 3};

        final LookupTable lut = new LookupTable(values, dimensions);
        assertEquals(2, lut.getDimensionCount());

        assertEquals(0.0, lut.getDimension(0).getMin(), 0.0);
        assertEquals(1.0, lut.getDimension(0).getMax(), 0.0);
        assertEquals(0.0, lut.getDimension(1).getMin(), 0.0);
        assertEquals(1.0, lut.getDimension(1).getMax(), 0.0);

        assertEquals(0.0, lut.getValue(0.0, 0.0), 0.0);
        assertEquals(1.0, lut.getValue(0.0, 1.0), 0.0);
        assertEquals(2.0, lut.getValue(1.0, 0.0), 0.0);
        assertEquals(3.0, lut.getValue(1.0, 1.0), 0.0);

        assertEquals(0.5, lut.getValue(0.0, 0.5), 0.0);
        assertEquals(1.5, lut.getValue(0.5, 0.5), 0.0);
        assertEquals(2.5, lut.getValue(1.0, 0.5), 0.0);
    }

    public void testInterpolation3D() {
        final IntervalPartition[] dimensions = IntervalPartition.createArray(
                new double[]{0, 1, 2, 3, 4}, new double[]{1, 2, 3, 4, 5}, new double[]{2, 3, 4, 5, 6});

        assertEquals(125, LookupTable.getVertexCount(dimensions));

        final double[] values = new double[125];
        for (int i = 0; i < values.length; ++i) {
            values[i] = i;
        }

        final LookupTable lut = new LookupTable(values, dimensions);
        assertEquals(3, lut.getDimensionCount());

        final double[] r = new double[3];
        final double[] x = new double[3];
        final FracIndex[] fi = FracIndex.createArray(3);
        final Random rng = new Random(27182);

        final double[] v = new double[1 << 3];
        for (int i = 0; i < 10; ++i) {
            // Compute random coordinates and fractional indices
            for (int j = 0; j < 3; ++j) {
                r[j] = rng.nextDouble() * (lut.getDimension(j).getMax() - lut.getDimension(j).getMin());
                x[j] = r[j] + lut.getDimension(j).getMin();

                final double floor = Math.floor(r[j]);
                fi[j].i = (int) floor;
                fi[j].f = r[j] - Math.floor(r[j]);
            }

            // Check computation of fractional indices
            for (int j = 0; j < 3; ++j) {
                final FracIndex fracIndex = new FracIndex();

                LookupTable.computeFracIndex(dimensions[j], x[j], fracIndex);
                assertEquals(fi[j].i, fracIndex.i);
                assertEquals(fi[j].f, fracIndex.f, 1.0E-10);
            }

            final double expected = r[2] + 5.0 * (r[1] + 5.0 * r[0]);
            final double a = lut.getValue(x);
            final double b = lut.getValue(fi, v);

            assertEquals(expected, a, 1.0E-10);
            assertEquals(expected, b, 1.0E-10);
        }
    }
    */
