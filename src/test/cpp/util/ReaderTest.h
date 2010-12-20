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
 * File:   ReaderTest.h
 * Author: thomass
 *
 * Created on December 20, 2010, 4:20 PM
 */

#ifndef READERTEST_H
#define	READERTEST_H

#include <cppunit/extensions/HelperMacros.h>

class ReaderTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(ReaderTest);
    CPPUNIT_TEST(testReading_OLCI_1);
    CPPUNIT_TEST(testReading_OLCI_2);
    CPPUNIT_TEST_SUITE_END();

public:
    ReaderTest();
    virtual ~ReaderTest();
    void setUp();
    void tearDown();
private:
    void testReading_OLCI_1();
    void testReading_OLCI_2();
};

#endif	/* READERTEST_H */

