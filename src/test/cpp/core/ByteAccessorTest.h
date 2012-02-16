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

#ifndef BYTEACCESSORTEST_H
#define	BYTEACCESSORTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/Accessor.h"

class ByteAccessorTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(ByteAccessorTest);
    CPPUNIT_TEST(testGetSampleCount);
    CPPUNIT_TEST(testSetByteGetByte);
    CPPUNIT_TEST(testSetDoubleGetByte);
    CPPUNIT_TEST(testSetFloatGetByte);
    CPPUNIT_TEST(testSetIntGetByte);
    CPPUNIT_TEST(testSetUByteGetByte);
    CPPUNIT_TEST(testGetByteData);
    CPPUNIT_TEST(testGetOtherTypeData);
    CPPUNIT_TEST_SUITE_END();

public:
    ByteAccessorTest();
    virtual ~ByteAccessorTest();
    void setUp();
    void tearDown();

private:
    Accessor* accessor;
    void testGetSampleCount();
    void testSetByteGetByte();
    void testSetDoubleGetByte();
    void testSetFloatGetByte();
    void testSetIntGetByte();
    void testSetUByteGetByte();
    void testGetByteData();
    void testGetOtherTypeData();
};

#endif	/* BYTEACCESSORTEST_H */

