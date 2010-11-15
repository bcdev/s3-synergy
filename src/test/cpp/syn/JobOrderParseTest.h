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
 * File:   JobOrderParseTest.h
 * Author: thomass
 *
 * Created on November 15, 2010, 4:21 PM
 */

#ifndef JOBORDERPARSETEST_H
#define	JOBORDERPARSETEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "../../../main/cpp/common/JobOrderParser.h"

class JobOrderParseTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(JobOrderParseTest);
    CPPUNIT_TEST(testParsing);
    CPPUNIT_TEST_SUITE_END();

public:
    JobOrderParseTest();
    virtual ~JobOrderParseTest();
    void setUp();
    void tearDown();

private:
    void testParsing();
    void initializeXerces();
    void terminateXerces();
};

#endif	/* JOBORDERPARSETEST_H */

