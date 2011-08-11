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
 * File:   DictionaryParserTest.cpp
 * Author: thomass
 *
 * Created on December 23, 2010, 11:11 AM
 */

#include <boost/lexical_cast.hpp>
#include <cstdlib>

#include "../../../main/cpp/util/DictionaryParser.h"
#include "DictionaryParserTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(DictionaryParserTest);

DictionaryParserTest::DictionaryParserTest() {
}

DictionaryParserTest::~DictionaryParserTest() {
}

const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");

void DictionaryParserTest::setUp() {
    XPathInitializer init;
    dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");
}

void DictionaryParserTest::tearDown() {
    delete dictionary;
}

static VariableDescriptor* getVariableDescriptor(const string& symbolicName, const vector<VariableDescriptor*>& descriptors) {

    foreach(VariableDescriptor* descriptor, descriptors) {
        if (descriptor->getName().compare(symbolicName) == 0) {
            return descriptor;
        }
    }
    throw std::invalid_argument("no variable descriptor for symbolic name '" + symbolicName + "'.");
}

void DictionaryParserTest::testGetNcVarNameAndGetFileName() {
    string symbolicName2 = "L_1";
    string symbolicName3 = "L_14";
    string symbolicName4 = "L_18";
    string symbolicName7 = "lat";
    string symbolicName8 = "time";
    string symbolicName9 = "OLC_flags";
    string symbolicName10 = "OLC_VAA";
    string symbolicName11 = "air_pressure";
    string symbolicName12 = "SLO_TP_lon";

    string symbolicName13 = "SDR_1";

    CPPUNIT_ASSERT(dictionary->hasProductDescriptor(Constants::PRODUCT_L1C));
    CPPUNIT_ASSERT(dictionary->hasProductDescriptor(Constants::PRODUCT_SYL2));
    ProductDescriptor& l1cProductDescriptor = dictionary->getProductDescriptor(Constants::PRODUCT_L1C);
    ProductDescriptor& l2ProductDescriptor = dictionary->getProductDescriptor(Constants::PRODUCT_SYL2);

    CPPUNIT_ASSERT(l1cProductDescriptor.hasSegmentDescriptor("OLC"));

    const vector<VariableDescriptor*>& l1cVariables = l1cProductDescriptor.getVariableDescriptors();

    string varName2 = getVariableDescriptor(symbolicName2, l1cVariables)->getNcVarName();
    string varName3 = getVariableDescriptor(symbolicName3, l1cVariables)->getNcVarName();
    string varName4 = getVariableDescriptor(symbolicName4, l1cVariables)->getNcVarName();
    string varName7 = getVariableDescriptor(symbolicName7, l1cVariables)->getNcVarName();
    string varName8 = getVariableDescriptor(symbolicName8, l1cVariables)->getNcVarName();
    string varName9 = getVariableDescriptor(symbolicName9, l1cVariables)->getNcVarName();
    string varName10 = getVariableDescriptor(symbolicName10, l1cVariables)->getNcVarName();
    string varName11 = getVariableDescriptor(symbolicName11, l1cVariables)->getNcVarName();
    string varName12 = getVariableDescriptor(symbolicName12, l1cVariables)->getNcVarName();

    string varName13 = getVariableDescriptor(symbolicName13, l2ProductDescriptor.getVariableDescriptors())->getNcVarName();

    CPPUNIT_ASSERT(varName2.compare("TOA_Radiance_Meas") == 0);
    CPPUNIT_ASSERT(varName3.compare("TOA_Radiance_Meas") == 0);
    CPPUNIT_ASSERT(varName4.compare("TOA_Radiance_Meas") == 0);
    CPPUNIT_ASSERT(varName7.compare("latitude") == 0);
    CPPUNIT_ASSERT(varName8.compare("OLCI_time_stamps") == 0);
    CPPUNIT_ASSERT(varName9.compare("OLCI_QC_flags") == 0);
    CPPUNIT_ASSERT(varName10.compare("OAA") == 0);
    CPPUNIT_ASSERT(varName11.compare("sea_level_pressure") == 0);
    CPPUNIT_ASSERT(varName12.compare("SLST_L1b_TP_Longitude") == 0);

    CPPUNIT_ASSERT(varName13.compare("SDR_1") == 0);

    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName3, l1cVariables)->getNcFileBasename().compare("OLC_RADIANCE_O14") == 0);
    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName4, l1cVariables)->getNcFileBasename().compare("OLC_RADIANCE_O18") == 0);
    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName7, l1cVariables)->getNcFileBasename().compare("GEOLOCATION_REF") == 0);
    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName8, l1cVariables)->getNcFileBasename().compare("TIME_STAMP_OLC") == 0);
    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName9, l1cVariables)->getNcFileBasename().compare("PIX_ANNOT_OLC") == 0);
    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName10, l1cVariables)->getNcFileBasename().compare("SUBS_ANNOT_GEOM_OLC") == 0);
    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName11, l1cVariables)->getNcFileBasename().compare("SUBS_ANNOT_METEO_OLC") == 0);
    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName12, l1cVariables)->getNcFileBasename().compare("SUBS_ANNOT_SLST_ALT") == 0);

    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName13, l2ProductDescriptor.getVariableDescriptors())->getNcFileBasename().compare("r0400") == 0);
}

void DictionaryParserTest::testDictionaryParsing() {
    ProductDescriptor& productDescriptor = dictionary->getProductDescriptor(Constants::PRODUCT_SYL2);
    testSDRVariable(*productDescriptor.getVariableDescriptor("SDR_1"));
    testA550Variable(*productDescriptor.getVariableDescriptor("A550"));
    testT550Variable(*productDescriptor.getVariableDescriptor("T550"));
    testAMINVariable(*productDescriptor.getVariableDescriptor("AMIN"));
    testAir_pressureVariable(*productDescriptor.getVariableDescriptor("air_pressure"));
    testLatitudeVariable(*productDescriptor.getVariableDescriptor("latitude"));
}

void DictionaryParserTest::testSDRVariable(VariableDescriptor& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.0001 == var.getAttribute(attributeName).getDoubles()[0]);

    attributeName = "channel";
    CPPUNIT_ASSERT(1 == var.getAttribute(attributeName).getInts()[0]);

    string expected = "r0400";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileBasename()) == 0);
}

void DictionaryParserTest::testT550Variable(VariableDescriptor& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.0001 == var.getAttribute(attributeName).getDoubles()[0]);

    attributeName = "valid_max";
    CPPUNIT_ASSERT(32767 == var.getAttribute(attributeName).getInts()[0]);

    string expected = "t550";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileBasename()) == 0);
}

void DictionaryParserTest::testA550Variable(VariableDescriptor& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.015 == var.getAttribute(attributeName).getDoubles()[0]);

    attributeName = "valid_max";
    CPPUNIT_ASSERT(255 == var.getAttribute(attributeName).getShorts()[0]);
}

void DictionaryParserTest::testAMINVariable(VariableDescriptor& var) {
    string attributeName = "_FillValue";
    CPPUNIT_ASSERT(0 == var.getAttribute(attributeName).getShorts()[0]);

    attributeName = "valid_max";
    CPPUNIT_ASSERT(40 == var.getAttribute(attributeName).getShorts()[0]);
}

void DictionaryParserTest::testLatitudeVariable(VariableDescriptor& var) {
    string attributeName = "units";
    CPPUNIT_ASSERT("degrees_north" == var.getAttribute(attributeName).getValue());

    attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.000001 == var.getAttribute(attributeName).getDoubles()[0]);

    attributeName = "valid_min";
    CPPUNIT_ASSERT(-90000000 == var.getAttribute(attributeName).getInts()[0]);
}

void DictionaryParserTest::testAir_pressureVariable(VariableDescriptor& var) {
    string attributeName = "units";
    CPPUNIT_ASSERT("hPa" == var.getAttribute(attributeName).getValue());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(1100.0 == var.getAttribute(attributeName).getFloats()[0]);

    string expected = "tiepoints_meteo";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileBasename()) == 0);
}
