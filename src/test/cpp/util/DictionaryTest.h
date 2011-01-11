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
 * File:   DictionaryTest.h
 * Author: thomass
 *
 * Created on December 23, 2010, 11:11 AM
 */

#ifndef DICTIONARYTEST_H
#define	DICTIONARYTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../main/cpp/util/Dictionary.h"
#include "../../../main/cpp/util/StringUtils.h"

class DictionaryTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(DictionaryTest);
    CPPUNIT_TEST(testDictionaryParsing);
    CPPUNIT_TEST(testGetNcVarNameAndGetFileName);
    CPPUNIT_TEST_SUITE_END();
    
public:
    DictionaryTest();
    virtual ~DictionaryTest();
    void setUp();
    void tearDown();
private:

    void testDictionaryParsing();
    void testGetNcVarNameAndGetFileName();
    void testSDRVariable(Variable& var);
    void testT550Variable(Variable& var);
    void testA550Variable(Variable& var);
    void testAMINVariable(Variable& var);
    void testSLN_flagsVariable(Variable& var);
    void testLatitudeVariable(Variable& var);
    void testTimeVariable(Variable& var);
    void testAir_pressureVariable(Variable& var);
    void testSLN_VAAVariable(Variable& var);
    void testLatVariable(Variable& var);
    void testB0_PVariable(Variable& var);
    void testB0_SVariable(Variable& var);
    void testMIR_PVariable(Variable& var);
    void testMIR_SVariable(Variable& var);
    void testSAA_PVariable(Variable& var);
    void testSAA_SVariable(Variable& var);
    void testAGVariable(Variable& var);
    void testOGVariable(Variable& var);
    void testWVGVariable(Variable& var);
    void testNDVIVariable(Variable& var);
    void testTGVariable(Variable& var);
    Dictionary* dict;

};

#endif	/* DICTIONARYTEST_H */