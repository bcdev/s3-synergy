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

#ifndef AERTEST_H
#define	AERTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"

class AerTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(AerTest);
    //CPPUNIT_TEST(testAer_s);
    //CPPUNIT_TEST(testReadAuxdata);
    CPPUNIT_TEST(testAer);
    CPPUNIT_TEST_SUITE_END();

public:
    AerTest();
    virtual ~AerTest();
    void setUp();
    void tearDown();

private:
    static const double EPSILON;
    void prepareContext();
    void testReadAuxdata();
    void testAer_s();
    void testAer();
    void testNdv();
    void testAotStandardError();
    void testErrorMetric();
    shared_ptr<Context> context;
    shared_ptr<Aer> aer;
};

#endif	/* AERTEST_H */

