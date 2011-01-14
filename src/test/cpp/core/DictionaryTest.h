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
 * File:   DictionaryTest.h
 * Author: ralf
 *
 * Created on January 14, 2011, 10:03 AM
 */

#ifndef DICTIONARYTEST_H
#define	DICTIONARYTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include <boost/foreach.hpp>
#include <string>
#include <vector>

#include "../../../main/cpp/core/Dictionary.h"

class DictionaryTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(DictionaryTest);
    CPPUNIT_TEST(testContainers);
    CPPUNIT_TEST_SUITE_END();

public:
    DictionaryTest();
    virtual ~DictionaryTest();
    void setUp();
    void tearDown();

private:
    Dictionary* dictionary;
    void testContainers();
};

#endif	/* DICTIONARYTEST_H */

