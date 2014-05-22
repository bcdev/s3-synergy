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

#ifndef ACOTEST_H
#define	ACOTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"

class AcoTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(AcoTest);
    CPPUNIT_TEST(testAco);
    CPPUNIT_TEST(testAco2);
    CPPUNIT_TEST_SUITE_END();

public:
    AcoTest();
    virtual ~AcoTest();
    void setUp();
    void tearDown();

private:
    void testAco();
    void testAco2();
};

#endif	/* ACOTEST_H */

