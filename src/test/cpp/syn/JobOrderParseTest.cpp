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
 * File:   JobOrderParseTest.cpp
 * Author: thomass
 * 
 * Created on November 15, 2010, 4:21 PM
 */

#include "JobOrderParseTest.h"
#include <xercesc/util/PlatformUtils.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(JobOrderParseTest);

JobOrderParseTest::JobOrderParseTest() {
}

JobOrderParseTest::~JobOrderParseTest() {
}

JobOrderParser parser = JobOrderParser("/mnt/hgfs/S3L2PP/src/test/resources/syn/JobOrder.Test_1.xml");

void JobOrderParseTest::setUp() {
    initializeXerces();
}

void JobOrderParseTest::tearDown() {
    parser.~JobOrderParser();
    terminateXerces();
}

void JobOrderParseTest::testParsing() {
    parser.readXml();
    parser.readConfig();
    CPPUNIT_ASSERT(true);
}

void JobOrderParseTest::initializeXerces() {
    try {
        xercesc_3_1::XMLPlatformUtils::Initialize();
    } catch (const XMLException& toCatch) {
        std::cout << "\n" << toCatch.getMessage() << "\n";
    }
}

void JobOrderParseTest::terminateXerces() {
    xercesc_3_1::XMLPlatformUtils::Terminate();
}