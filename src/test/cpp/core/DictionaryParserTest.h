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

class DictionaryParserTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(DictionaryParserTest);
    CPPUNIT_TEST(testDictionaryParsing);
    CPPUNIT_TEST(testGetNcVarNameAndGetFileName);
    CPPUNIT_TEST_SUITE_END();
    
public:
    DictionaryParserTest();
    virtual ~DictionaryParserTest();
    void setUp();
    void tearDown();
private:

    void testDictionaryParsing();
    void testGetNcVarNameAndGetFileName();
    void testSDRVariable(VariableDescriptor& var);
    void testT550Variable(VariableDescriptor& var);
    void testA550Variable(VariableDescriptor& var);
    void testAMINVariable(VariableDescriptor& var);
    void testSLN_flagsVariable(VariableDescriptor& var);
    void testLatitudeVariable(VariableDescriptor& var);
    void testTimeVariable(VariableDescriptor& var);
    void testAir_pressureVariable(VariableDescriptor& var);
    void testSLN_VAAVariable(VariableDescriptor& var);
    void testLatVariable(VariableDescriptor& var);
    void testB0_PVariable(VariableDescriptor& var);
    void testB0_SVariable(VariableDescriptor& var);
    void testMIR_PVariable(VariableDescriptor& var);
    void testMIR_SVariable(VariableDescriptor& var);
    void testSAA_PVariable(VariableDescriptor& var);
    void testSAA_SVariable(VariableDescriptor& var);
    void testAGVariable(VariableDescriptor& var);
    void testOGVariable(VariableDescriptor& var);
    void testWVGVariable(VariableDescriptor& var);
    void testNDVIVariable(VariableDescriptor& var);
    void testTGVariable(VariableDescriptor& var);
    Dictionary* dict;
};

#endif	/* DICTIONARYPARSERTEST_H */