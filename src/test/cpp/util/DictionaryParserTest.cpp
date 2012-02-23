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

#include "../../../main/cpp/core/Constants.h"
#include "../../../main/cpp/util/DictionaryParser.h"

#include "DictionaryParserTest.h"

using std::getenv;

using boost::lexical_cast;

CPPUNIT_TEST_SUITE_REGISTRATION(DictionaryParserTest);

DictionaryParserTest::DictionaryParserTest() {
}

DictionaryParserTest::~DictionaryParserTest() {
}

void DictionaryParserTest::setUp() {
	XPathInitializer init;
	dictionary = DictionaryParser().parse(Constants::S3_SYNERGY_HOME + "/src/main/resources/dictionary");
}

void DictionaryParserTest::tearDown() {
}

void DictionaryParserTest::testSy1ProductDescriptor() {
    CPPUNIT_ASSERT(dictionary->hasProductDescriptor(Constants::PRODUCT_SY1));
    const ProductDescriptor& p = dictionary->getProductDescriptor(Constants::PRODUCT_SY1);

    CPPUNIT_ASSERT(p.hasSegmentDescriptor(Constants::SEGMENT_OLC));
    const SegmentDescriptor& olc = p.getSegmentDescriptor(Constants::SEGMENT_OLC);

    CPPUNIT_ASSERT(p.hasSegmentDescriptor(Constants::SEGMENT_GEO));
    const SegmentDescriptor& geo = p.getSegmentDescriptor(Constants::SEGMENT_GEO);

    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_1"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_2"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_3"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_4"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_5"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_6"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_7"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_8"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_9"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_10"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_11"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_12"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_13"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_14"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_15"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_16"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_17"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_18"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_19"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_20"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("L_21"));

    CPPUNIT_ASSERT(geo.hasVariableDescriptor("latitude"));
    CPPUNIT_ASSERT(geo.hasVariableDescriptor("longitude"));
    CPPUNIT_ASSERT(olc.hasVariableDescriptor("altitude"));

    CPPUNIT_ASSERT(p.hasSegmentDescriptor(Constants::SEGMENT_OLC_TIME));
    const SegmentDescriptor& olcTime = p.getSegmentDescriptor(
            Constants::SEGMENT_OLC_TIME);

    CPPUNIT_ASSERT(olcTime.hasVariableDescriptor("time"));

    CPPUNIT_ASSERT(p.hasSegmentDescriptor(Constants::SEGMENT_SLN));
    const SegmentDescriptor& sln = p.getSegmentDescriptor(
            Constants::SEGMENT_SLN);

    CPPUNIT_ASSERT(sln.hasVariableDescriptor("L_1"));
    CPPUNIT_ASSERT(sln.hasVariableDescriptor("L_2"));
    CPPUNIT_ASSERT(sln.hasVariableDescriptor("L_3"));
    CPPUNIT_ASSERT(sln.hasVariableDescriptor("L_4"));
    CPPUNIT_ASSERT(sln.hasVariableDescriptor("L_5"));
    CPPUNIT_ASSERT(sln.hasVariableDescriptor("L_6"));

    CPPUNIT_ASSERT(p.hasSegmentDescriptor(Constants::SEGMENT_SLO));
    const SegmentDescriptor& slo = p.getSegmentDescriptor(
            Constants::SEGMENT_SLO);

    CPPUNIT_ASSERT(slo.hasVariableDescriptor("L_1"));
    CPPUNIT_ASSERT(slo.hasVariableDescriptor("L_2"));
    CPPUNIT_ASSERT(slo.hasVariableDescriptor("L_3"));
    CPPUNIT_ASSERT(slo.hasVariableDescriptor("L_4"));
    CPPUNIT_ASSERT(slo.hasVariableDescriptor("L_5"));
    CPPUNIT_ASSERT(slo.hasVariableDescriptor("L_6"));

    CPPUNIT_ASSERT(p.hasSegmentDescriptor(Constants::SEGMENT_OLC_TP));
    CPPUNIT_ASSERT(p.hasSegmentDescriptor(Constants::SEGMENT_SLN_TP));
    CPPUNIT_ASSERT(p.hasSegmentDescriptor(Constants::SEGMENT_SLO_TP));
    CPPUNIT_ASSERT(p.hasSegmentDescriptor(Constants::SEGMENT_OLC_INFO));
    CPPUNIT_ASSERT(p.hasSegmentDescriptor(Constants::SEGMENT_SLN_INFO));
    CPPUNIT_ASSERT(p.hasSegmentDescriptor(Constants::SEGMENT_SLO_INFO));
}

void DictionaryParserTest::testSy2ProductDescriptor() {
    CPPUNIT_ASSERT(dictionary->hasProductDescriptor(Constants::PRODUCT_SY2));
    const ProductDescriptor & p = dictionary->getProductDescriptor(Constants::PRODUCT_SY2);

    CPPUNIT_ASSERT(p.hasSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED));
    const SegmentDescriptor & s = p.getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);

    CPPUNIT_ASSERT(p.hasSegmentDescriptor(Constants::SEGMENT_GEO));
    const SegmentDescriptor& g = p.getSegmentDescriptor(Constants::SEGMENT_GEO);

    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_1"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_2"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_3"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_4"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_5"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_6"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_7"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_8"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_9"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_10"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_11"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_12"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_13"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_14"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_15"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_16"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_17"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_18"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_19"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_20"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_21"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_22"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_23"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_24"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_25"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_26"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_27"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_28"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_29"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SDR_30"));

    CPPUNIT_ASSERT(s.hasVariableDescriptor("A550"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("T550"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("AMIN"));

    CPPUNIT_ASSERT(g.hasVariableDescriptor("longitude"));
    CPPUNIT_ASSERT(g.hasVariableDescriptor("latitude"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("altitude"));

    checkSDRVariable(s.getVariableDescriptor("SDR_1"));
    checkA550Variable(s.getVariableDescriptor("A550"));
    checkT550Variable(s.getVariableDescriptor("T550"));
    ckeckAMINVariable(s.getVariableDescriptor("AMIN"));

    checkLatitudeVariable(g.getVariableDescriptor("latitude"));
    checkLongitudeVariable(g.getVariableDescriptor("longitude"));

    string expected;
    expected = "CF-1.4";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("Conventions").getValue()) == 0);

    expected = "TBD";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("institution").getValue()) == 0);

    expected = "Sentinel-3A SY";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("source").getValue()) == 0);

    expected = "ATBD: S3-L2-SD-03-S2-SU-ATBD, IODD: S3-L2-SD-08-S-BC-IODD";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("references").getValue()) == 0);

    expected = "info@brockmann-consult.de";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("contact").getValue()) == 0);

    expected = "netCDF_4";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("netCDF_version").getValue()) == 0);

    expected = "test";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("dataset_type").getValue()) == 0);

    expected = "2";
    CPPUNIT_ASSERT(lexical_cast<uint16_t>(expected) == lexical_cast<uint16_t>(p.getAttribute("absolute_orbit_number").getValue()));

    expected = "UTC=2013-06-21T10:09:20.655099";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("start_time").getValue()) == 0);

    expected = "UTC=2013-06-21T10:14:13.783099";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("stop_time").getValue()) == 0);
}

void DictionaryParserTest::testVgpProductDescriptor() {
    CPPUNIT_ASSERT(dictionary->hasProductDescriptor(Constants::PRODUCT_VGP));
    const ProductDescriptor & p = dictionary->getProductDescriptor(Constants::PRODUCT_VGP);

    CPPUNIT_ASSERT(p.hasSegmentDescriptor(Constants::SEGMENT_VGT));
    const SegmentDescriptor & s = p.getSegmentDescriptor(Constants::SEGMENT_VGT);

    CPPUNIT_ASSERT(s.hasVariableDescriptor("B0"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("B2"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("B3"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("MIR"));
    CPPUNIT_ASSERT(s.hasVariableDescriptor("SM"));

    CPPUNIT_ASSERT(p.hasSegmentDescriptor(Constants::SEGMENT_VGT_TP));
    const SegmentDescriptor& t = p.getSegmentDescriptor(Constants::SEGMENT_VGT_TP);

    CPPUNIT_ASSERT(t.hasVariableDescriptor("AG"));
    CPPUNIT_ASSERT(t.hasVariableDescriptor("OG"));
    CPPUNIT_ASSERT(t.hasVariableDescriptor("WVG"));

    CPPUNIT_ASSERT(t.hasVariableDescriptor("SAA"));
    CPPUNIT_ASSERT(t.hasVariableDescriptor("SZA"));
    CPPUNIT_ASSERT(t.hasVariableDescriptor("VAA"));
    CPPUNIT_ASSERT(t.hasVariableDescriptor("VZA"));

    string expected;
    expected = "CF-1.4";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("Conventions").getValue()) == 0);

    expected = "TBD";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("institution").getValue()) == 0);

    expected = "Sentinel-3A SY";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("source").getValue()) == 0);

    expected = "ATBD: S3-L2-SD-03-S2-SU-ATBD, IODD: S3-L2-SD-08-S-BC-IODD";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("references").getValue()) == 0);

    expected = "info@brockmann-consult.de";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("contact").getValue()) == 0);

    expected = "netCDF_4";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("netCDF_version").getValue()) == 0);

    expected = "test";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("dataset_type").getValue()) == 0);

    expected = "2";
    CPPUNIT_ASSERT(lexical_cast<uint16_t>(expected) == lexical_cast<uint16_t>(p.getAttribute("absolute_orbit_number").getValue()));

    expected = "UTC=2013-06-21T10:09:20.655099";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("start_time").getValue()) == 0);

    expected = "UTC=2013-06-21T10:14:13.783099";
    CPPUNIT_ASSERT(expected.compare(p.getAttribute("stop_time").getValue()) == 0);
}

void DictionaryParserTest::checkSDRVariable(VariableDescriptor& v) {
	CPPUNIT_ASSERT(0.0001 == v.getAttribute("scale_factor").getDoubles()[0]);
	CPPUNIT_ASSERT(1 == v.getAttribute("channel").getInts()[0]);

	const string& expected = "r0400";
	CPPUNIT_ASSERT(expected.compare(v.getNcFileBasename()) == 0);
}

void DictionaryParserTest::checkT550Variable(VariableDescriptor& v) {
	CPPUNIT_ASSERT(0.0001 == v.getAttribute("scale_factor").getDoubles()[0]);
	CPPUNIT_ASSERT(32767 == v.getAttribute("valid_max").getInts()[0]);

	const string& expected = "t550";
	CPPUNIT_ASSERT(expected.compare(v.getNcFileBasename()) == 0);
}

void DictionaryParserTest::checkA550Variable(VariableDescriptor& v) {
	CPPUNIT_ASSERT(0.015 == v.getAttribute("scale_factor").getDoubles()[0]);
	CPPUNIT_ASSERT(255 == v.getAttribute("valid_max").getShorts()[0]);
}

void DictionaryParserTest::ckeckAMINVariable(VariableDescriptor& v) {
	CPPUNIT_ASSERT(0 == v.getAttribute("_FillValue").getShorts()[0]);
	CPPUNIT_ASSERT(40 == v.getAttribute("valid_max").getShorts()[0]);
}

void DictionaryParserTest::checkLatitudeVariable(VariableDescriptor& v) {
	CPPUNIT_ASSERT("degrees_north" == v.getAttribute("units").getValue());
	CPPUNIT_ASSERT(0.000001 == v.getAttribute("scale_factor").getDoubles()[0]);
	CPPUNIT_ASSERT(-90000000 == v.getAttribute("valid_min").getInts()[0]);
	CPPUNIT_ASSERT(90000000 == v.getAttribute("valid_max").getInts()[0]);
}

void DictionaryParserTest::checkLongitudeVariable(VariableDescriptor& v) {
	CPPUNIT_ASSERT("degrees_east" == v.getAttribute("units").getValue());
	CPPUNIT_ASSERT(0.000001 == v.getAttribute("scale_factor").getDoubles()[0]);
	CPPUNIT_ASSERT(-180000000 == v.getAttribute("valid_min").getInts()[0]);
	CPPUNIT_ASSERT(180000000 == v.getAttribute("valid_max").getInts()[0]);
}

void DictionaryParserTest::checkAirPressureVariable(VariableDescriptor& v) {
	CPPUNIT_ASSERT("hPa" == v.getAttribute("units").getValue());
	CPPUNIT_ASSERT(1100.0 == v.getAttribute("valid_max").getFloats()[0]);

	const string& expected = "tiepoints_meteo";
	CPPUNIT_ASSERT(expected.compare(v.getNcFileBasename()) == 0);
}
