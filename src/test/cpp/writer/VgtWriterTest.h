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

#ifndef VGTWRITERTEST_H_
#define VGTWRITERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"

class VgtWriterTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(VgtWriterTest);
    CPPUNIT_TEST(testWriter);
    CPPUNIT_TEST_SUITE_END();

public:
    VgtWriterTest();
    virtual ~VgtWriterTest();
    void setUp();
    void tearDown();

protected:
    void testWriter();
    void prepareContext();
    shared_ptr<Context> context;
};

#endif /* VGTWRITERTEST_H_ */
