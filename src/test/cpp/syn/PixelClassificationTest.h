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
 * File:   PixelClassificationTest.h
 * Author: thomass
 *
 * Created on November 10, 2010, 3:45 PM
 */

#ifndef PCLTEST_H
#define	PCLTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "../../../main/cpp/common/PixelClassification.h"
#include "../../../main/cpp/common/SegmentImpl.h"

class PixelClassificationTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(PixelClassificationTest);
    CPPUNIT_TEST(testPixelClassification);
    CPPUNIT_TEST_SUITE_END();

public:
    PixelClassificationTest();
    virtual ~PixelClassificationTest();
    void setUp();
    void tearDown();

private:
    SegmentImpl segment;

    void testPixelClassification();
};

#endif	/* PCLTEST_H */

