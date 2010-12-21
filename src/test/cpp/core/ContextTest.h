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
 * File:   ContextTest.h
 * Author: ralf
 *
 * Created on December 21, 2010, 1:08 PM
 */

#ifndef CONTEXTTEST_H
#define	CONTEXTTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/Context.h"

class ContextTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(ContextTest);
    CPPUNIT_TEST(testInitialState);
    CPPUNIT_TEST(testAddModule);
    CPPUNIT_TEST_SUITE_END();

public:
    ContextTest();
    virtual ~ContextTest();
    void setUp();
    void tearDown();

private:
    Context* context;
    void testInitialState();
    void testAddModule();
};

#endif	/* CONTEXTTEST_H */

