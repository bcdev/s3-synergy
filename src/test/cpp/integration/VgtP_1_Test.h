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

#ifndef VGTP_1_TEST_H
#define	VGTP_1_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class VgtP_1_Test : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VgtP_1_Test);
    CPPUNIT_TEST(testParseJobOrder_01);
    CPPUNIT_TEST(testParseJobOrder_02);
    CPPUNIT_TEST_SUITE_END();

public:
    VgtP_1_Test();
    virtual ~VgtP_1_Test();
    void setUp();
    void tearDown();

private:
    void testParseJobOrder_01();
    void testParseJobOrder_02();
};

#endif	/* VGTP_1_TEST_H */

