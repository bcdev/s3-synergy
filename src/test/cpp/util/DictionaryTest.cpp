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
    dict.parse();
//    set<Variable*> vars = dict.getVariables();
//    for (set<Variable*>::iterator iter = vars.begin(); iter != vars.end(); iter++) {
//        std::cout << "\n" << (*iter)->toString() << "\n";
//    }
}

void DictionaryTest::tearDown() {

}

void DictionaryTest::testGetNcVarName() {
    string symbolicName = "SDR_1";
    string symbolicName2 = "L1c_OLC_RADIANCE_O1_TOA_Radiance_Meas";
    string fileNameExt = "L1c_OLC_RADIANCE_O1.nc";
    string fileName = "L1c_OLC_RADIANCE_O1";
    
    string varName1 = dict.getNcVarName(symbolicName, fileNameExt);
    string varName2 = dict.getNcVarName(symbolicName, fileName);
    string varName3 = dict.getNcVarName(symbolicName2, fileNameExt);
    string varName4 = dict.getNcVarName(symbolicName2, fileName);

    CPPUNIT_ASSERT( varName1.compare("SDR_1") == 0 );
    CPPUNIT_ASSERT( varName2.compare("SDR_1") == 0 );
    CPPUNIT_ASSERT( varName3.compare("TOA_Radiance_Meas") == 0 );
    CPPUNIT_ASSERT( varName4.compare("TOA_Radiance_Meas") == 0 );
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
    testAGVariable(dict.getVariable("AG"));
    testB0_PVariable(dict.getVariable("B0_P"));
    testB0_SVariable(dict.getVariable("B0_S"));
    testLatVariable(dict.getVariable("lat"));
    testMIR_PVariable(dict.getVariable("MIR_P"));
    testMIR_SVariable(dict.getVariable("MIR_S"));
    testNDVIVariable(dict.getVariable("NDVI"));
    testOGVariable(dict.getVariable("OG"));
    testSAA_PVariable(dict.getVariable("SAA_P"));
    testSAA_SVariable(dict.getVariable("SAA_S"));
    testTGVariable(dict.getVariable("TG"));
    testWVGVariable(dict.getVariable("WVG"));
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
    CPPUNIT_ASSERT(ncShort == var.getType());

    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.015 == boost::lexical_cast<double>(var.getAttribute(attributeName).getValue()));

    attributeName = "valid_max";
    CPPUNIT_ASSERT(255 == boost::lexical_cast<short>(var.getAttribute(attributeName).getValue()));
}

void DictionaryTest::testAMINVariable(Variable& var) {
    testStandardDims(var);
    CPPUNIT_ASSERT(ncShort == var.getType());

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

void DictionaryTest::testLatVariable(Variable& var) {
    string expectedDim = "lat";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[0]->getName()) == 0);

    CPPUNIT_ASSERT(ncDouble == var.getType());

    string attributeName = "units";
    CPPUNIT_ASSERT("degrees_north" == var.getAttribute(attributeName).getValue());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(180.0 == boost::lexical_cast<float>(var.getAttribute(attributeName).getValue()));
}

void DictionaryTest::testB0_PVariable(Variable& var) {
    CPPUNIT_ASSERT(var.getDimensions().size() == 2);

    string expectedDim = "lat";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[0]->getName()) == 0);
    expectedDim = "lon";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[1]->getName()) == 0);

    CPPUNIT_ASSERT(ncShort == var.getType());

    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.001 == boost::lexical_cast<double>(var.getAttribute(attributeName).getValue()));

    attributeName = "valid_max";
    CPPUNIT_ASSERT(10000.0 == boost::lexical_cast<float>(var.getAttribute(attributeName).getValue()));

    string expected = "VGT_P_B0_TOA_reflectance";
    CPPUNIT_ASSERT(expected.compare(dict.getNcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testB0_SVariable(Variable& var) {
    CPPUNIT_ASSERT(var.getDimensions().size() == 2);

    string expectedDim = "lat";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[0]->getName()) == 0);
    expectedDim = "lon";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[1]->getName()) == 0);

    CPPUNIT_ASSERT(ncShort == var.getType());

    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.001f == boost::lexical_cast<float>(var.getAttribute(attributeName).getValue()));

    attributeName = "valid_max";
    CPPUNIT_ASSERT(10000.0 == boost::lexical_cast<float>(var.getAttribute(attributeName).getValue()));

    string expected = "VGT_S_B0_surface_reflectance";
    CPPUNIT_ASSERT(expected.compare(dict.getNcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testMIR_PVariable(Variable& var) {
    CPPUNIT_ASSERT(var.getDimensions().size() == 2);

    string expectedDim = "lat";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[0]->getName()) == 0);
    expectedDim = "lon";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[1]->getName()) == 0);

    CPPUNIT_ASSERT(ncShort == var.getType());

    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.001f == boost::lexical_cast<float>(var.getAttribute(attributeName).getValue()));

    attributeName = "valid_max";
    CPPUNIT_ASSERT(10000.0 == boost::lexical_cast<float>(var.getAttribute(attributeName).getValue()));

    string expected = "VGT_P_MIR_TOA_reflectance";
    CPPUNIT_ASSERT(expected.compare(dict.getNcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testMIR_SVariable(Variable& var) {
    CPPUNIT_ASSERT(var.getDimensions().size() == 2);

    string expectedDim = "lat";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[0]->getName()) == 0);
    expectedDim = "lon";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[1]->getName()) == 0);

    CPPUNIT_ASSERT(ncShort == var.getType());

    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.001f == boost::lexical_cast<float>(var.getAttribute(attributeName).getValue()));

    attributeName = "valid_max";
    CPPUNIT_ASSERT(10000.0 == boost::lexical_cast<float>(var.getAttribute(attributeName).getValue()));

    string expected = "VGT_S_MIR_surface_reflectance";
    CPPUNIT_ASSERT(expected.compare(dict.getNcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testSAA_PVariable(Variable& var) {
    CPPUNIT_ASSERT(var.getDimensions().size() == 2);

    string expectedDim = "lat";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[0]->getName()) == 0);
    expectedDim = "lon";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[1]->getName()) == 0);

    CPPUNIT_ASSERT(ncByte == var.getType());

    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(1.5 == boost::lexical_cast<float>(var.getAttribute(attributeName).getValue()));

    attributeName = "valid_max";
    CPPUNIT_ASSERT(120 == boost::lexical_cast<short>(var.getAttribute(attributeName).getValue()));

    string expected = "VGT_P_solar_azimuth_angle";
    CPPUNIT_ASSERT(expected.compare(dict.getNcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testSAA_SVariable(Variable& var) {
    CPPUNIT_ASSERT(var.getDimensions().size() == 2);

    string expectedDim = "lat";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[0]->getName()) == 0);
    expectedDim = "lon";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[1]->getName()) == 0);

    CPPUNIT_ASSERT(ncByte == var.getType());

    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(1.5 == boost::lexical_cast<float>(var.getAttribute(attributeName).getValue()));

    attributeName = "valid_max";
    CPPUNIT_ASSERT(120 == boost::lexical_cast<short>(var.getAttribute(attributeName).getValue()));

    string expected = "VGT_S_solar_azimuth_angle";
    CPPUNIT_ASSERT(expected.compare(dict.getNcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testAGVariable(Variable& var) {
    CPPUNIT_ASSERT(var.getDimensions().size() == 2);

    string expectedDim = "lat";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[0]->getName()) == 0);
    expectedDim = "lon";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[1]->getName()) == 0);

    CPPUNIT_ASSERT(ncByte == var.getType());

    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.004 == boost::lexical_cast<double>(var.getAttribute(attributeName).getValue()));

    attributeName = "valid_max";
    CPPUNIT_ASSERT(127 == boost::lexical_cast<short>(var.getAttribute(attributeName).getValue()));

    string expected = "VGT_P_aerosol";
    CPPUNIT_ASSERT(expected.compare(dict.getNcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testOGVariable(Variable& var) {
    CPPUNIT_ASSERT(var.getDimensions().size() == 2);

    string expectedDim = "lat";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[0]->getName()) == 0);
    expectedDim = "lon";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[1]->getName()) == 0);

    CPPUNIT_ASSERT(ncByte == var.getType());

    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.004 == boost::lexical_cast<double>(var.getAttribute(attributeName).getValue()));

    attributeName = "units";
    CPPUNIT_ASSERT("atm cm" == var.getAttribute(attributeName).getValue());

    attributeName = "_FillValue";
    CPPUNIT_ASSERT(-1 == boost::lexical_cast<short>(var.getAttribute(attributeName).getValue()));

    string expected = "VGT_P_ozone";
    CPPUNIT_ASSERT(expected.compare(dict.getNcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testWVGVariable(Variable& var) {
    CPPUNIT_ASSERT(var.getDimensions().size() == 2);

    string expectedDim = "lat";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[0]->getName()) == 0);
    expectedDim = "lon";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[1]->getName()) == 0);

    CPPUNIT_ASSERT(ncShort == var.getType());

    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.04 == boost::lexical_cast<double>(var.getAttribute(attributeName).getValue()));

    attributeName = "units";
    CPPUNIT_ASSERT("g cm-2" == var.getAttribute(attributeName).getValue());

    attributeName = "_FillValue";
    CPPUNIT_ASSERT(-1 == boost::lexical_cast<short>(var.getAttribute(attributeName).getValue()));

    string expected = "VGT_P_water_vapour";
    CPPUNIT_ASSERT(expected.compare(dict.getNcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testNDVIVariable(Variable& var) {
    CPPUNIT_ASSERT(var.getDimensions().size() == 2);

    string expectedDim = "lat";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[0]->getName()) == 0);
    expectedDim = "lon";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[1]->getName()) == 0);

    CPPUNIT_ASSERT(ncByte == var.getType());

    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.004 == boost::lexical_cast<double>(var.getAttribute(attributeName).getValue()));

    attributeName = "standard_name";
    CPPUNIT_ASSERT("normalized_difference_vegetation_index" == var.getAttribute(attributeName).getValue());

    attributeName = "add_offset";
    CPPUNIT_ASSERT(-0.1 == boost::lexical_cast<double>(var.getAttribute(attributeName).getValue()));

    string expected = "VGT_S_NDVI";
    CPPUNIT_ASSERT(expected.compare(dict.getNcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testTGVariable(Variable& var) {
    CPPUNIT_ASSERT(var.getDimensions().size() == 2);

    string expectedDim = "lat";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[0]->getName()) == 0);
    expectedDim = "lon";
    CPPUNIT_ASSERT(expectedDim.compare(var.getDimensions()[1]->getName()) == 0);

    CPPUNIT_ASSERT(ncShort == var.getType());

    string attributeName = "standard_name";
    CPPUNIT_ASSERT("time" == var.getAttribute(attributeName).getValue());

    attributeName = "_FillValue";
    CPPUNIT_ASSERT(-1 == boost::lexical_cast<short>(var.getAttribute(attributeName).getValue()));

    string expected = "VGT_S_synthesis_time";
    CPPUNIT_ASSERT(expected.compare(dict.getNcFileName(var.getNcName())) == 0);
}