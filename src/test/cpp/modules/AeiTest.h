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

#ifndef AEITEST_H
#define	AEITEST_H

#include <cppunit/extensions/HelperMacros.h>

class AeiTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(AeiTest);
    CPPUNIT_TEST(testAei);
    CPPUNIT_TEST(testAei2);
    CPPUNIT_TEST_SUITE_END();

public:
    AeiTest();
    virtual ~AeiTest();
    void setUp();
    void tearDown();

private:
    void testAei();
    void testAei2();
};

#endif	/* AEITEST_H */

