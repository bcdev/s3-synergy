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
 * File:   AveragingTest.cpp
 * Author: thomass
 * 
 * Created on November 10, 2010, 4:09 PM
 */

#include "AveragingTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(AveragingTest);

AveragingTest::AveragingTest() {
}

AveragingTest::~AveragingTest() {
}

void AveragingTest::setUp() {
    this->example = new int(1);
}

void AveragingTest::tearDown() {
    delete this->example;
}

void AveragingTest::testMethod() {
    CPPUNIT_ASSERT(*example == 1);
}

void AveragingTest::testFailedMethod() {
    CPPUNIT_ASSERT(++*example == 2);
}
