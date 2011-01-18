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
 * File:   SynExceptionTest.cpp
 * Author: thomass
 * 
 * Created on January 18, 2011, 2:28 PM
 */


#include "../../../main/cpp/core/Boost.h"
#include "../../../main/cpp/util/Logger.h"
#include "../../../main/cpp/core/SynException.h"
#include "SynExceptionTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(SynExceptionTest);

SynExceptionTest::SynExceptionTest() {
}

SynExceptionTest::~SynExceptionTest() {
}

void SynExceptionTest::setUp() {
}

void SynExceptionTest::tearDown() {
}

void SynExceptionTest::testArguments() {
    try {
        throwException();
    } catch (SynException &e) {
        CPPUNIT_ASSERT(e.getMessage().compare("my message") == 0);
        CPPUNIT_ASSERT(e.getModuleId().compare("my module id") == 0);
    }
}

void SynExceptionTest::throwException() {
    SynException up("my message", "my module id");
    BOOST_THROW_EXCEPTION(up);
}
