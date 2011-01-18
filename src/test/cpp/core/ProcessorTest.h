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
 * File:   ProcessorTest.h
 * Author: thomass
 *
 * Created on January 18, 2011, 10:46 AM
 */

#ifndef PROCESSORTEST_H
#define	PROCESSORTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/Dictionary.h"

class ProcessorTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(ProcessorTest);
    CPPUNIT_TEST(testCatching);
    CPPUNIT_TEST_SUITE_END();

public:
    ProcessorTest();
    virtual ~ProcessorTest();
    void setUp();
    void tearDown();

private:
    Dictionary* dict;
    void testCatching();
};

#endif	/* PROCESSORTEST_H */

