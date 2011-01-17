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
#include <vector>

#include "DictionaryParserTest.h"
#include "../../../main/cpp/core/Constants.h"

using std::vector;

CPPUNIT_TEST_SUITE_REGISTRATION(DictionaryParserTest);

DictionaryParserTest::DictionaryParserTest() {

}

DictionaryParserTest::~DictionaryParserTest() {
}

void DictionaryParserTest::setUp() {
    // Initialize Xerces and XPath
    XPathInitializer init;
    dict = new Dictionary("/mnt/hgfs/S3L2PP/src/test/resources/syn/config/config.xml");
}

void DictionaryParserTest::tearDown() {
    delete dict;
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
    string symbolicName5 = "L_23";
    string symbolicName6 = "L_28";
    string symbolicName7 = "lat";
    string symbolicName8 = "time";
    string symbolicName9 = "OLC_flags";
    string symbolicName10 = "OLC_VAA";
    string symbolicName11 = "air_pressure";
    string symbolicName12 = "SLO_TP_lon";

    string symbolicName13 = "SDR_1";

    CPPUNIT_ASSERT(dict->hasProductDescriptor(Constants::SYMBOLIC_NAME_L1C));
    CPPUNIT_ASSERT(dict->hasProductDescriptor(Constants::SYMBOLIC_NAME_L2));
    ProductDescriptor& l1cProductDescriptor = dict->getProductDescriptor(Constants::SYMBOLIC_NAME_L1C);
    ProductDescriptor& l2ProductDescriptor = dict->getProductDescriptor(Constants::SYMBOLIC_NAME_L2);

    int segmentDescriptorsCount = l1cProductDescriptor.getSegmentDescriptors().size();
    CPPUNIT_ASSERT(segmentDescriptorsCount > 0);

    foreach(SegmentDescriptor* s, l1cProductDescriptor.getSegmentDescriptors()) {
        std::cout << s->getName() << "\n";
    }
    //    CPPUNIT_ASSERT(l1cProductDescriptor.hasSegmentDescriptor("SYN_COLLOCATION"));

    string varName2 = getVariableDescriptor(symbolicName2, l1cProductDescriptor.getVariables())->getName();
    string varName3 = getVariableDescriptor(symbolicName3, l1cProductDescriptor.getVariables())->getName();
    string varName4 = getVariableDescriptor(symbolicName4, l1cProductDescriptor.getVariables())->getName();
    string varName5 = getVariableDescriptor(symbolicName5, l1cProductDescriptor.getVariables())->getName();
    string varName6 = getVariableDescriptor(symbolicName6, l1cProductDescriptor.getVariables())->getName();
    string varName7 = getVariableDescriptor(symbolicName7, l1cProductDescriptor.getVariables())->getName();
    string varName8 = getVariableDescriptor(symbolicName8, l1cProductDescriptor.getVariables())->getName();
    string varName9 = getVariableDescriptor(symbolicName9, l1cProductDescriptor.getVariables())->getName();
    string varName10 = getVariableDescriptor(symbolicName10, l1cProductDescriptor.getVariables())->getName();
    string varName11 = getVariableDescriptor(symbolicName11, l1cProductDescriptor.getVariables())->getName();
    string varName12 = getVariableDescriptor(symbolicName12, l1cProductDescriptor.getVariables())->getName();

    string varName13 = getVariableDescriptor(symbolicName13, l2ProductDescriptor.getVariables())->getName();

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

    CPPUNIT_ASSERT(varName13.compare("SDR_1") == 0);

    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName3, l1cProductDescriptor.getVariables())->getNcFileName().compare("OLC_RADIANCE_O16") == 0);
    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName4, l1cProductDescriptor.getVariables())->getNcFileName().compare("OLC_RADIANCE_O21") == 0);
    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName5, l1cProductDescriptor.getVariables())->getNcFileName().compare("SLST_NAD_RADIANCE_S5") == 0);
    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName6, l1cProductDescriptor.getVariables())->getNcFileName().compare("SLST_ALT_RADIANCE_S4") == 0);
    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName7, l1cProductDescriptor.getVariables())->getNcFileName().compare("GEOLOCATION_REF") == 0);
    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName8, l1cProductDescriptor.getVariables())->getNcFileName().compare("L2_SYN_time_stamps") == 0);
    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName9, l1cProductDescriptor.getVariables())->getNcFileName().compare("PIX_ANNOT_OLC") == 0);
    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName10, l1cProductDescriptor.getVariables())->getNcFileName().compare("SUBS_ANNOT_GEOM_OLC") == 0);
    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName11, l1cProductDescriptor.getVariables())->getNcFileName().compare("L2_SYN_geophysical_atmospheric_data") == 0);
    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName12, l1cProductDescriptor.getVariables())->getNcFileName().compare("SUBS_ANNOT_SLST_ALT") == 0);

    CPPUNIT_ASSERT(getVariableDescriptor(symbolicName13, l2ProductDescriptor.getVariables())->getNcFileName().compare("L2_SYN_surface_directional_reflectance_1") == 0);
}

void DictionaryParserTest::testDictionaryParsing() {
    ProductDescriptor productDescriptor = dict->getProductDescriptor(Constants::SYMBOLIC_NAME_L2);
    testSDRVariable(*productDescriptor.getVariable("SDR_1"));
    testA550Variable(*productDescriptor.getVariable("A550"));
    testT550Variable(*productDescriptor.getVariable("T550"));
    testAMINVariable(*productDescriptor.getVariable("AMIN"));
    testAir_pressureVariable(*productDescriptor.getVariable("air_pressure"));
    testLatitudeVariable(*productDescriptor.getVariable("latitude"));
    testSLN_flagsVariable(*productDescriptor.getVariable("SLN_flags"));
}

void DictionaryParserTest::testSDRVariable(VariableDescriptor& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.0001 == var.getAttribute(attributeName).getDouble());

    attributeName = "channel";
    CPPUNIT_ASSERT(1 == var.getAttribute(attributeName).getInt());

    string expected = "L2_SYN_surface_directional_reflectance_1";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileName()) == 0);
}

void DictionaryParserTest::testT550Variable(VariableDescriptor& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.0001 == var.getAttribute(attributeName).getDouble());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(32767 == var.getAttribute(attributeName).getInt());

    string expected = "L2_SYN_aerosol_optical_thickness";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileName()) == 0);
}

void DictionaryParserTest::testA550Variable(VariableDescriptor& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.015 == var.getAttribute(attributeName).getDouble());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(255 == var.getAttribute(attributeName).getShort());
}

void DictionaryParserTest::testAMINVariable(VariableDescriptor& var) {
    string attributeName = "_FillValue";
    CPPUNIT_ASSERT(0 == var.getAttribute(attributeName).getShort());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(40 == var.getAttribute(attributeName).getShort());
}

void DictionaryParserTest::testSLN_flagsVariable(VariableDescriptor& var) {
    string attributeName = "_FillValue";
    CPPUNIT_ASSERT(0 == var.getAttribute(attributeName).getShort());

    string expected = "L2_SYN_status_flags";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileName()) == 0);
}

void DictionaryParserTest::testLatitudeVariable(VariableDescriptor& var) {
    string attributeName = "units";
    CPPUNIT_ASSERT("degrees_north" == var.getAttribute(attributeName).getValue());

    attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.000001 == var.getAttribute(attributeName).getDouble());

    attributeName = "valid_min";
    CPPUNIT_ASSERT(-90000000 == var.getAttribute(attributeName).getInt());
}

void DictionaryParserTest::testTimeVariable(VariableDescriptor& var) {
    string attributeName = "units";
    CPPUNIT_ASSERT("microseconds since 2000-1-1 0:0:0" == var.getAttribute(attributeName).getValue());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(4294967295.0 == var.getAttribute(attributeName).getDouble());
}

void DictionaryParserTest::testSLN_VAAVariable(VariableDescriptor& var) {
    string attributeName = "units";
    CPPUNIT_ASSERT("degrees" == var.getAttribute(attributeName).getValue());

    attributeName = "valid_min";
    CPPUNIT_ASSERT(-180.0 == var.getAttribute(attributeName).getFloat());
}

void DictionaryParserTest::testAir_pressureVariable(VariableDescriptor& var) {
    string attributeName = "units";
    CPPUNIT_ASSERT("hPa" == var.getAttribute(attributeName).getValue());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(1100.0 == var.getAttribute(attributeName).getFloat());

    string expected = "L2_SYN_geophysical_atmospheric_data";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileName()) == 0);
}

void DictionaryParserTest::testLatVariable(VariableDescriptor& var) {
    string attributeName = "units";
    CPPUNIT_ASSERT("degrees_north" == var.getAttribute(attributeName).getValue());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(180.0 == var.getAttribute(attributeName).getFloat());
}

void DictionaryParserTest::testB0_PVariable(VariableDescriptor& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.001 == var.getAttribute(attributeName).getDouble());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(10000.0 == var.getAttribute(attributeName).getFloat());

    string expected = "VGT_P_B0_TOA_reflectance";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileName()) == 0);
}

void DictionaryParserTest::testB0_SVariable(VariableDescriptor& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.001f == var.getAttribute(attributeName).getFloat());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(10000.0 == var.getAttribute(attributeName).getFloat());

    string expected = "VGT_S_B0_surface_reflectance";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileName()) == 0);
}

void DictionaryParserTest::testMIR_PVariable(VariableDescriptor& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.001f == var.getAttribute(attributeName).getFloat());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(10000.0 == var.getAttribute(attributeName).getFloat());

    string expected = "VGT_P_MIR_TOA_reflectance";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileName()) == 0);
}

void DictionaryParserTest::testMIR_SVariable(VariableDescriptor& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.001f == var.getAttribute(attributeName).getFloat());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(10000.0 == var.getAttribute(attributeName).getFloat());

    string expected = "VGT_S_MIR_surface_reflectance";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileName()) == 0);
}

void DictionaryParserTest::testSAA_PVariable(VariableDescriptor& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(1.5 == var.getAttribute(attributeName).getFloat());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(120 == var.getAttribute(attributeName).getShort());

    string expected = "VGT_P_solar_azimuth_angle";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileName()) == 0);
}

void DictionaryParserTest::testSAA_SVariable(VariableDescriptor& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(1.5 == var.getAttribute(attributeName).getFloat());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(120 == var.getAttribute(attributeName).getShort());

    string expected = "VGT_S_solar_azimuth_angle";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileName()) == 0);
}

void DictionaryParserTest::testAGVariable(VariableDescriptor& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.004 == var.getAttribute(attributeName).getDouble());

    attributeName = "valid_max";
    CPPUNIT_ASSERT(127 == var.getAttribute(attributeName).getShort());

    string expected = "VGT_P_aerosol";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileName()) == 0);
}

void DictionaryParserTest::testOGVariable(VariableDescriptor& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.004 == var.getAttribute(attributeName).getDouble());

    attributeName = "units";
    CPPUNIT_ASSERT("atm cm" == var.getAttribute(attributeName).getValue());

    attributeName = "_FillValue";
    CPPUNIT_ASSERT(-1 == var.getAttribute(attributeName).getShort());

    string expected = "VGT_P_ozone";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileName()) == 0);
}

void DictionaryParserTest::testWVGVariable(VariableDescriptor& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.04 == var.getAttribute(attributeName).getDouble());

    attributeName = "units";
    CPPUNIT_ASSERT("g cm-2" == var.getAttribute(attributeName).getValue());

    attributeName = "_FillValue";
    CPPUNIT_ASSERT(-1 == var.getAttribute(attributeName).getShort());

    string expected = "VGT_P_water_vapour";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileName()) == 0);
}

void DictionaryParserTest::testNDVIVariable(VariableDescriptor& var) {
    string attributeName = "scale_factor";
    CPPUNIT_ASSERT(0.004 == var.getAttribute(attributeName).getDouble());

    attributeName = "standard_name";
    CPPUNIT_ASSERT("normalized_difference_vegetation_index" == var.getAttribute(attributeName).getValue());

    attributeName = "add_offset";
    CPPUNIT_ASSERT(-0.1 == var.getAttribute(attributeName).getDouble());

    string expected = "VGT_S_NDVI";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileName()) == 0);
}

void DictionaryParserTest::testTGVariable(VariableDescriptor& var) {
    string attributeName = "standard_name";
    CPPUNIT_ASSERT("time" == var.getAttribute(attributeName).getValue());

    attributeName = "_FillValue";
    CPPUNIT_ASSERT(-1 == var.getAttribute(attributeName).getShort());

    string expected = "VGT_S_synthesis_time";
    CPPUNIT_ASSERT(expected.compare(var.getNcFileName()) == 0);
}