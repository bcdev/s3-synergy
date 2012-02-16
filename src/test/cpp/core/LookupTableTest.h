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

#ifndef LOOKUPTABLETEST_H_
#define LOOKUPTABLETEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/GridImpl.h"

class LookupTableTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(LookupTableTest);
    CPPUNIT_TEST(testInterpolation1D);
    CPPUNIT_TEST(testInterpolation2D);
    CPPUNIT_TEST(testInterpolation3D);
    CPPUNIT_TEST(testInterpolation4D);
    CPPUNIT_TEST_SUITE_END();

public:
    LookupTableTest();
    virtual ~LookupTableTest();
    void setUp();
    void tearDown();

private:
    void testInterpolation1D();
    void testInterpolation2D();
    void testInterpolation3D();
    void testInterpolation4D();
};

#endif /* LOOKUPTABLETEST_H_ */
