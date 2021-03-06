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

#ifndef DICTIONARYTEST_H
#define	DICTIONARYTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/Dictionary.h"

class DictionaryTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(DictionaryTest);
    CPPUNIT_TEST(testDescriptors);
    CPPUNIT_TEST_SUITE_END();

public:
    DictionaryTest();
    virtual ~DictionaryTest();
    void setUp();
    void tearDown();

private:
    Dictionary* dictionary;
    void testDescriptors();
};

#endif	/* DICTIONARYTEST_H */

