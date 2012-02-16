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

#ifndef DIRECTLOCATORTEST_H_
#define DIRECTLOCATORTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class DirectLocatorTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(DirectLocatorTest);
    CPPUNIT_TEST(testRotation_Identity);
    CPPUNIT_TEST(testRotation_Antipode);
    CPPUNIT_TEST(testLocation);
    CPPUNIT_TEST_SUITE_END();

public:
    DirectLocatorTest();
    virtual ~DirectLocatorTest();
    void setUp();
    void tearDown();

private:
    void testRotation_Identity();
    void testRotation_Antipode();
    void testLocation();
};

#endif /* DIRECTLOCATORTEST_H_ */
