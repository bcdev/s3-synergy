/* 
 * Copyright (C) 2010 by Brockmann Consult (info@brockmann-consult.de)
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
 *
 * File:   AccessorInstantiationTest.h
 * Author: ralf
 *
 * Created on December 17, 2010, 5:17 PM
 */

#ifndef ACCESSORINSTANTIATIONTEST_H
#define	ACCESSORINSTANTIATIONTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/Accessor.h"

class AccessorInstantiationTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(AccessorInstantiationTest);
    CPPUNIT_TEST(testInstantiateByteAccessor);
    CPPUNIT_TEST(testInstantiateDoubleAccessor);
    CPPUNIT_TEST(testInstantiateFloatAccessor);
    CPPUNIT_TEST(testInstantiateIntAccessor);
    CPPUNIT_TEST(testInstantiateLongAccessor);
    CPPUNIT_TEST(testInstantiateShortAccessor);
    CPPUNIT_TEST_SUITE_END();

public:
    AccessorInstantiationTest();
    virtual ~AccessorInstantiationTest();
    void setUp();
    void tearDown();

private:
    Accessor* accessor;
    void testInstantiateByteAccessor();
    void testInstantiateDoubleAccessor();
    void testInstantiateFloatAccessor();
    void testInstantiateIntAccessor();
    void testInstantiateLongAccessor();
    void testInstantiateShortAccessor();
};

#endif	/* ACCESSORINSTANTIATIONTEST_H */

