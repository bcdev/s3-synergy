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
 * File:   PixelClassificationTest.cpp
 * Author: thomass
 * 
 * Created on November 10, 2010, 3:45 PM
 */

#include "PixelClassificationTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(PixelClassificationTest);

PixelClassificationTest::PixelClassificationTest() {
}

PixelClassificationTest::~PixelClassificationTest() {
}

void PixelClassificationTest::setUp() {
    this->pixel = new Pixel();
}

void PixelClassificationTest::tearDown() {
    delete this->pixel;
}

void PixelClassificationTest::testPixelClassification() {
    const PixelClassification pixelClassification;
    pixelClassification.classify(*pixel);
    const bool land = pixel->isRaised("SYN_L2_Flags", 0x0010);
    CPPUNIT_ASSERT(land);
}
