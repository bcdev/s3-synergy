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
#include <vector>

#include "DictionaryTest.h"

using std::vector;

CPPUNIT_TEST_SUITE_REGISTRATION(DictionaryTest);

DictionaryTest::DictionaryTest() {

}

DictionaryTest::~DictionaryTest() {
}

void DictionaryTest::setUp() {
    // Initialize Xerces and XPath
    XPathInitializer init;
    dict = new Dictionary("/mnt/hgfs/S3L2PP/src/test/resources/syn/config/config.xml");
}

void DictionaryTest::tearDown() {
    delete dict;
}

void DictionaryTest::testGetNcVarNameAndGetFileName() {
    string symbolicName2 = "L_1";
    string symbolicName3 = "L_14";
    string symbolicName4 = "L_18";
    string symbolicName5 = "L_23";
    string symbolicName6 = "L_28";
    string symbolicName7 = "lat";
    string symbolicName8 = "time";
    string symbolicName9 = "OLC_flags";
    string symbolicName10 = "OLC_VAA";
    string symbolicName11 = "air_pressure";
    string symbolicName12 = "SLO_TP_lon";
    //    string symbolicName12 = "water_vapour";

    string varName2 = dict->getL1cNcVarName(symbolicName2);
    string varName3 = dict->getL1cNcVarName(symbolicName3);
    string varName4 = dict->getL1cNcVarName(symbolicName4);
    string varName5 = dict->getL1cNcVarName(symbolicName5);
    string varName6 = dict->getL1cNcVarName(symbolicName6);
    string varName7 = dict->getL1cNcVarName(symbolicName7);
    string varName8 = dict->getL1cNcVarName(symbolicName8);
    string varName9 = dict->getL1cNcVarName(symbolicName9);
    string varName10 = dict->getL1cNcVarName(symbolicName10);
    string varName11 = dict->getL1cNcVarName(symbolicName11);
    string varName12 = dict->getL1cNcVarName(symbolicName12);

    CPPUNIT_ASSERT(varName2.compare("TOA_Radiance_Meas") == 0);
    CPPUNIT_ASSERT(varName3.compare("TOA_Radiance_Meas") == 0);
    CPPUNIT_ASSERT(varName4.compare("TOA_Radiance_Meas") == 0);
    CPPUNIT_ASSERT(varName5.compare("TOA_Radiance_Meas") == 0);
    CPPUNIT_ASSERT(varName6.compare("TOA_Radiance_Meas") == 0);
    CPPUNIT_ASSERT(varName7.compare("Geodetic_Latitude") == 0);
    CPPUNIT_ASSERT(varName8.compare("OLCI_time_stamps") == 0);
    CPPUNIT_ASSERT(varName9.compare("OLCI_QC_flags") == 0);
    CPPUNIT_ASSERT(varName10.compare("OAA") == 0);
    CPPUNIT_ASSERT(varName11.compare("sea_level_pressure") == 0);
    CPPUNIT_ASSERT(varName12.compare("SLST_L1b_TP_Longitude") == 0);

    CPPUNIT_ASSERT(dict->getL1cVariable(symbolicName3).getFileName().compare("OLC_RADIANCE_O16") == 0);
    CPPUNIT_ASSERT(dict->getL1cVariable(symbolicName4).getFileName().compare("OLC_RADIANCE_O21") == 0);
    CPPUNIT_ASSERT(dict->getL1cVariable(symbolicName5).getFileName().compare("SLST_NAD_RADIANCE_S5") == 0);
    CPPUNIT_ASSERT(dict->getL1cVariable(symbolicName6).getFileName().compare("SLST_ALT_RADIANCE_S4") == 0);
    CPPUNIT_ASSERT(dict->getL1cVariable(symbolicName7).getFileName().compare("GEOLOCATION_REF") == 0);
    CPPUNIT_ASSERT(dict->getL2Variable(symbolicName8).getFileName().compare("L2_SYN_time_stamps") == 0);
    CPPUNIT_ASSERT(dict->getL1cVariable(symbolicName9).getFileName().compare("PIX_ANNOT_OLC") == 0);
    CPPUNIT_ASSERT(dict->getL1cVariable(symbolicName10).getFileName().compare("SUBS_ANNOT_GEOM_OLC") == 0);
    CPPUNIT_ASSERT(dict->getL2Variable(symbolicName11).getFileName().compare("L2_SYN_geophysical_atmospheric_data") == 0);
    CPPUNIT_ASSERT(dict->getL1cVariable(symbolicName12).getFileName().compare("SUBS_ANNOT_SLST_ALT") == 0);
}

void DictionaryTest::testDictionaryParsing() {
    testSDRVariable(dict->getL2Variable("SDR_1"));
    testA550Variable(dict->getL2Variable("A550"));
    testT550Variable(dict->getL2Variable("T550"));
    testAMINVariable(dict->getL2Variable("AMIN"));
    testAir_pressureVariable(dict->getL2Variable("air_pressure"));
    testLatitudeVariable(dict->getL2Variable("latitude"));
    testSLN_flagsVariable(dict->getL2Variable("SLN_flags"));
}

void DictionaryTest::testSDRVariable(Variable& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.0001 == var.getAttribute(attributeName).getDouble());

    attributeName = "channel";
    CPPUNIT_ASSERT(1 == var.getAttribute(attributeName).getInt());

    string expected = "L2_SYN_surface_directional_reflectance_1";
    CPPUNIT_ASSERT(expected.compare(dict->getL2NcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testT550Variable(Variable& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.0001 == var.getAttribute(attributeName).getDouble());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(32767 == var.getAttribute(attributeName).getInt());

    string expected = "L2_SYN_aerosol_optical_thickness";
    CPPUNIT_ASSERT(expected.compare(dict->getL2NcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testA550Variable(Variable& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.015 == var.getAttribute(attributeName).getDouble());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(255 == var.getAttribute(attributeName).getShort());
}

void DictionaryTest::testAMINVariable(Variable& var) {
    string attributeName = "_FillValue";
    CPPUNIT_ASSERT(0 == var.getAttribute(attributeName).getShort());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(40 == var.getAttribute(attributeName).getShort());
}

void DictionaryTest::testSLN_flagsVariable(Variable& var) {
    string attributeName = "_FillValue";
    CPPUNIT_ASSERT(0 == var.getAttribute(attributeName).getShort());

    string expected = "L2_SYN_status_flags";
    CPPUNIT_ASSERT(expected.compare(dict->getL2NcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testLatitudeVariable(Variable& var) {
    string attributeName = "units";
    CPPUNIT_ASSERT("degrees_north" == var.getAttribute(attributeName).getStrings()[0]);

    attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.000001 == var.getAttribute(attributeName).getDouble());

    attributeName = "valid_min";
    CPPUNIT_ASSERT(-90000000 == var.getAttribute(attributeName).getInt());
}

void DictionaryTest::testTimeVariable(Variable& var) {
    string attributeName = "units";
    CPPUNIT_ASSERT("microseconds since 2000-1-1 0:0:0" == var.getAttribute(attributeName).getStrings()[0]);

    attributeName = "valid_max";
    CPPUNIT_ASSERT(4294967295.0 == var.getAttribute(attributeName).getDouble());
}

void DictionaryTest::testSLN_VAAVariable(Variable& var) {
    string attributeName = "units";
    CPPUNIT_ASSERT("degrees" == var.getAttribute(attributeName).getStrings()[0]);

    attributeName = "valid_min";
    CPPUNIT_ASSERT(-180.0 == var.getAttribute(attributeName).getFloat());
}

void DictionaryTest::testAir_pressureVariable(Variable& var) {
    string attributeName = "units";
    CPPUNIT_ASSERT("hPa" == var.getAttribute(attributeName).getStrings()[0]);

    attributeName = "valid_max";
    CPPUNIT_ASSERT(1100.0 == var.getAttribute(attributeName).getFloat());

    string expected = "L2_SYN_geophysical_atmospheric_data";
    CPPUNIT_ASSERT(expected.compare(dict->getL2NcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testLatVariable(Variable& var) {
    string attributeName = "units";
    CPPUNIT_ASSERT("degrees_north" == var.getAttribute(attributeName).getStrings()[0]);

    attributeName = "valid_max";
    CPPUNIT_ASSERT(180.0 == var.getAttribute(attributeName).getFloat());
}

void DictionaryTest::testB0_PVariable(Variable& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.001 == var.getAttribute(attributeName).getDouble());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(10000.0 == var.getAttribute(attributeName).getFloat());

    string expected = "VGT_P_B0_TOA_reflectance";
    CPPUNIT_ASSERT(expected.compare(dict->getL2NcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testB0_SVariable(Variable& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.001f == var.getAttribute(attributeName).getFloat());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(10000.0 == var.getAttribute(attributeName).getFloat());

    string expected = "VGT_S_B0_surface_reflectance";
    CPPUNIT_ASSERT(expected.compare(dict->getL2NcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testMIR_PVariable(Variable& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.001f == var.getAttribute(attributeName).getFloat());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(10000.0 == var.getAttribute(attributeName).getFloat());

    string expected = "VGT_P_MIR_TOA_reflectance";
    CPPUNIT_ASSERT(expected.compare(dict->getL2NcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testMIR_SVariable(Variable& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.001f == var.getAttribute(attributeName).getFloat());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(10000.0 == var.getAttribute(attributeName).getFloat());

    string expected = "VGT_S_MIR_surface_reflectance";
    CPPUNIT_ASSERT(expected.compare(dict->getL2NcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testSAA_PVariable(Variable& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(1.5 == var.getAttribute(attributeName).getFloat());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(120 == var.getAttribute(attributeName).getShort());

    string expected = "VGT_P_solar_azimuth_angle";
    CPPUNIT_ASSERT(expected.compare(dict->getL2NcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testSAA_SVariable(Variable& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(1.5 == var.getAttribute(attributeName).getFloat());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(120 == var.getAttribute(attributeName).getShort());

    string expected = "VGT_S_solar_azimuth_angle";
    CPPUNIT_ASSERT(expected.compare(dict->getL2NcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testAGVariable(Variable& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.004 == var.getAttribute(attributeName).getDouble());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(127 == var.getAttribute(attributeName).getShort());

    string expected = "VGT_P_aerosol";
    CPPUNIT_ASSERT(expected.compare(dict->getL2NcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testOGVariable(Variable& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.004 == var.getAttribute(attributeName).getDouble());

    attributeName = "units";
    CPPUNIT_ASSERT("atm cm" == var.getAttribute(attributeName).getStrings()[0]);

    attributeName = "_FillValue";
    CPPUNIT_ASSERT(-1 == var.getAttribute(attributeName).getShort());

    string expected = "VGT_P_ozone";
    CPPUNIT_ASSERT(expected.compare(dict->getL2NcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testWVGVariable(Variable& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.04 == var.getAttribute(attributeName).getDouble());

    attributeName = "units";
    CPPUNIT_ASSERT("g cm-2" == var.getAttribute(attributeName).getStrings()[0]);

    attributeName = "_FillValue";
    CPPUNIT_ASSERT(-1 == var.getAttribute(attributeName).getShort());

    string expected = "VGT_P_water_vapour";
    CPPUNIT_ASSERT(expected.compare(dict->getL2NcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testNDVIVariable(Variable& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.004 == var.getAttribute(attributeName).getDouble());

    attributeName = "standard_name";
    CPPUNIT_ASSERT("normalized_difference_vegetation_index" == var.getAttribute(attributeName).getStrings()[0]);

    attributeName = "add_offset";
    CPPUNIT_ASSERT(-0.1 == var.getAttribute(attributeName).getDouble());

    string expected = "VGT_S_NDVI";
    CPPUNIT_ASSERT(expected.compare(dict->getL2NcFileName(var.getNcName())) == 0);
}

void DictionaryTest::testTGVariable(Variable& var) {
    string attributeName = "standard_name";
    CPPUNIT_ASSERT("time" == var.getAttribute(attributeName).getStrings()[0]);

    attributeName = "_FillValue";
    CPPUNIT_ASSERT(-1 == var.getAttribute(attributeName).getShort());

    string expected = "VGT_S_synthesis_time";
    CPPUNIT_ASSERT(expected.compare(dict->getL2NcFileName(var.getNcName())) == 0);
}