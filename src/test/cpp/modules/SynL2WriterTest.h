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
 * File:   SynL2WriterTest.h
 * Author: ralf
 *
 * Created on January 12, 2011, 11:48 AM
 */

#ifndef SYNL2WRITERTEST_H
#define	SYNL2WRITERTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/modules/SynL2Writer.h"

class SynL2WriterTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(SynL2WriterTest);
    CPPUNIT_TEST(testWriter);
    CPPUNIT_TEST_SUITE_END();

public:
    SynL2WriterTest();
    virtual ~SynL2WriterTest();
    void setUp();
    void tearDown();

private:
    Module* writer;
    void testWriter();
};

#endif	/* SYNL2WRITERTEST_H */

