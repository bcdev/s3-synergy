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

#ifndef TIEPOINTINTERPOLATORTEST_H_
#define TIEPOINTINTERPOLATORTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/TiePointInterpolator.h"

class TiePointInterpolatorTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(TiePointInterpolatorTest);
    CPPUNIT_TEST(testWeightsAndIndexesForIntermediatePoint);
    CPPUNIT_TEST(testWeightsAndIndexesForTiePointLocation);
    CPPUNIT_TEST(testInterpolation);
    CPPUNIT_TEST_SUITE_END();

public:
    TiePointInterpolatorTest();
    virtual ~TiePointInterpolatorTest();
    void setUp();
    void tearDown();

private:
    void testWeightsAndIndexesForIntermediatePoint();
    void testWeightsAndIndexesForTiePointLocation();
    void testInterpolation();

    TiePointInterpolator<double>* tpi;
};

#endif /* TIEPOINTINTERPOLATORTEST_H_ */
