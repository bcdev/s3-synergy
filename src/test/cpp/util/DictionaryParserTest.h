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
 * File:   DictionaryParserTest.h
 * Author: thomass
 *
 * Created on December 23, 2010, 11:11 AM
 */

#ifndef DICTIONARYPARSERTEST_H
#define	DICTIONARYPARSERTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/core/Dictionary.h"

class DictionaryParserTest: public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(DictionaryParserTest);
    CPPUNIT_TEST(testSy1ProductDescriptor);
    CPPUNIT_TEST(testSy2ProductDescriptor);
    CPPUNIT_TEST_SUITE_END();

public:
    DictionaryParserTest();
    virtual ~DictionaryParserTest();
    void setUp();
    void tearDown();

private:
    void testSy1ProductDescriptor();
    void testSy2ProductDescriptor();
    void testBreakpointProductDescriptor();
    void checkSDRVariable(VariableDescriptor & v);
    void checkT550Variable(VariableDescriptor & v);
    void checkA550Variable(VariableDescriptor & v);
    void ckeckAMINVariable(VariableDescriptor & v);
    void checkLatitudeVariable(VariableDescriptor & v);
    void checkLongitudeVariable(VariableDescriptor & v);
    void checkAirPressureVariable(VariableDescriptor & v);
    void testL1Data(const string& productDescriptorId);
    void testL2Data(const string& productDescriptorId);

    shared_ptr<Dictionary> dictionary;
}
;

#endif	/* DICTIONARYPARSERTEST_H */
