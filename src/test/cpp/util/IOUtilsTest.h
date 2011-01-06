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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * File:   IOUtilsTest.h
 * Author: thomass
 *
 * Created on January 6, 2011, 12:14 PM
 */

#ifndef IOUTILSTEST_H
#define	IOUTILSTEST_H

#include <cppunit/extensions/HelperMacros.h>

class IOUtilsTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(IOUtilsTest);
    CPPUNIT_TEST(testCreateCountVector);
    CPPUNIT_TEST(testCreateStartVector);
    CPPUNIT_TEST(testAddVariableToSegment);
    CPPUNIT_TEST_SUITE_END();

public:
    IOUtilsTest();
    virtual ~IOUtilsTest();
    void setUp();
    void tearDown();
private:
    void testCreateCountVector();
    void testCreateStartVector();
    void testAddVariableToSegment();
};

#endif	/* IOUTILSTEST_H */

