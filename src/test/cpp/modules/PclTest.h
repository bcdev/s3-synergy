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

#ifndef PCLTEST_H
#define	PCLTEST_H

#include <cppunit/extensions/HelperMacros.h>

class PclTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(PclTest);
    CPPUNIT_TEST(testPcl);
    CPPUNIT_TEST(testPcl2);
    CPPUNIT_TEST_SUITE_END();

public:
    PclTest();
    virtual ~PclTest();
    void setUp();
    void tearDown();

private:
    void testPcl();
    void testPcl2();
};

#endif	/* PCLTEST_H */

