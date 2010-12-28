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
 * File:   DictionaryTest.cpp
 * Author: thomass
 * 
 * Created on December 23, 2010, 11:11 AM
 */

#include <boost/lexical_cast.hpp>

#include "DictionaryTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(DictionaryTest);

DictionaryTest::~DictionaryTest() {
}

void DictionaryTest::setUp() {
    // Initialize Xerces and XPath
    XPathInitializer init;
    dict.parseInputFiles();
}

void DictionaryTest::tearDown() {

}

void DictionaryTest::testDictionaryParsing() {
    testSDRVariable(dict.getVariable("SDR_1"));
    testA550Variable(dict.getVariable("A550"));
    testT550Variable(dict.getVariable("T550"));
    testAMINVariable(dict.getVariable("AMIN"));
    testAir_pressureVariable(dict.getVariable("air_pressure"));
    testLatitudeVariable(dict.getVariable("latitude"));
    testSLN_flagsVariable(dict.getVariable("SLN_flags"));
    testTimeVariable(dict.getVariable("time"));
}

void DictionaryTest::testSDRVariable(Variable& var) {
    testStandardDims(var);
    CPPUNIT_ASSERT(ncShort == var.getType());

    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.0001 == boost::lexical_cast<double>(var.getAttribute(attributeName).getValue()));

    attributeName = "channel";
    CPPUNIT_ASSERT(1 == boost::lexical_cast<int>(var.getAttribute(attributeName).getValue()));

    string expected = "L2_SYN_surface_directional_reflectance_1";
    CPPUNIT_ASSERT(expected.compare(dict.getNcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testT550Variable(Variable& var) {
    testStandardDims(var);
    CPPUNIT_ASSERT(ncShort == var.getType());

    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.0001 == boost::lexical_cast<double>(var.getAttribute(attributeName).getValue()));

    attributeName = "valid_max";
    CPPUNIT_ASSERT(32767 == boost::lexical_cast<int>(var.getAttribute(attributeName).getValue()));

    string expected = "L2_SYN_aerosol_optical_thickness";
    CPPUNIT_ASSERT(expected.compare(dict.getNcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testA550Variable(Variable& var) {
    testStandardDims(var);
    CPPUNIT_ASSERT(ncChar == var.getType());

    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.015 == boost::lexical_cast<double>(var.getAttribute(attributeName).getValue()));

    attributeName = "valid_max";
    CPPUNIT_ASSERT(255 == boost::lexical_cast<short>(var.getAttribute(attributeName).getValue()));
}

void DictionaryTest::testAMINVariable(Variable& var) {
    testStandardDims(var);
    CPPUNIT_ASSERT(ncChar == var.getType());

    string attributeName = "_FillValue";
    CPPUNIT_ASSERT(0 == boost::lexical_cast<short>(var.getAttribute(attributeName).getValue()));

    attributeName = "valid_max";
    CPPUNIT_ASSERT(40 == boost::lexical_cast<short>(var.getAttribute(attributeName).getValue()));
}

void DictionaryTest::testSLN_flagsVariable(Variable& var) {
    testStandardDims(var);
    CPPUNIT_ASSERT(ncInt == var.getType());

    string attributeName = "_FillValue";
    CPPUNIT_ASSERT(0 == boost::lexical_cast<short>(var.getAttribute(attributeName).getValue()));

    string expected = "L2_SYN_status_flags";
    CPPUNIT_ASSERT(expected.compare(dict.getNcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testLatitudeVariable(Variable& var) {
    testStandardDims(var);
    CPPUNIT_ASSERT(ncInt == var.getType());

    string attributeName = "units";
    CPPUNIT_ASSERT("degrees_north" == var.getAttribute(attributeName).getValue());

    attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.000001 == boost::lexical_cast<double>(var.getAttribute(attributeName).getValue()));

    attributeName = "valid_min";
    CPPUNIT_ASSERT(-90000000 == boost::lexical_cast<int>(var.getAttribute(attributeName).getValue()));
}

void DictionaryTest::testTimeVariable(Variable& var) {
    CPPUNIT_ASSERT(ncDouble == var.getType());

    string attributeName = "units";
    CPPUNIT_ASSERT("microseconds since 2000-1-1 0:0:0" == var.getAttribute(attributeName).getValue());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(4294967295.0 == boost::lexical_cast<double>(var.getAttribute(attributeName).getValue()));
}

void DictionaryTest::testSLN_VAAVariable(Variable& var) {
    string expectedDim = "N_SLN_TP";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[0]->getName()) == 0);

    CPPUNIT_ASSERT(ncFloat == var.getType());

    string attributeName = "units";
    CPPUNIT_ASSERT("degrees" == var.getAttribute(attributeName).getValue());

    attributeName = "valid_min";
    CPPUNIT_ASSERT(-180.0 == boost::lexical_cast<float>(var.getAttribute(attributeName).getValue()));
}

void DictionaryTest::testAir_pressureVariable(Variable& var) {
    string expectedDim = "N_OLC_TP";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[0]->getName()) == 0);

    CPPUNIT_ASSERT(ncFloat == var.getType());

    string attributeName = "units";
    CPPUNIT_ASSERT("hPa" == var.getAttribute(attributeName).getValue());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(1100.0 == boost::lexical_cast<float>(var.getAttribute(attributeName).getValue()));

    string expected = "L2_SYN_geophysical_atmospheric_data";
    CPPUNIT_ASSERT(expected.compare(dict.getNcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testStandardDims(Variable& var) {
    string expected = "N_CAM";
    CPPUNIT_ASSERT(expected.compare(var.getDimensions()[0]->getName()) == 0);
    expected = "N_LINE_OLC";
    CPPUNIT_ASSERT(expected.compare(var.getDimensions()[1]->getName()) == 0);
    expected = "N_DET_CAM";
    CPPUNIT_ASSERT(expected.compare(var.getDimensions()[2]->getName()) == 0);

    CPPUNIT_ASSERT(5 == boost::lexical_cast<int>(var.getDimensions()[0]->getRange()));
    CPPUNIT_ASSERT(60000 == boost::lexical_cast<int>(var.getDimensions()[1]->getRange()));
    CPPUNIT_ASSERT(740 == boost::lexical_cast<int>(var.getDimensions()[2]->getRange()));
}