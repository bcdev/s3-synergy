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
 * File:   JobOrderParseTest.cpp
 * Author: thomass
 * 
 * Created on November 15, 2010, 4:21 PM
 */

#include "JobOrderParserTest.h"
#include "../../../main/cpp/core/Constants.h"

CPPUNIT_TEST_SUITE_REGISTRATION(JobOrderParserTest);

JobOrderParserTest::JobOrderParserTest() {
}

JobOrderParserTest::~JobOrderParserTest() {
}

void JobOrderParserTest::setUp() {
	parser = new JobOrderParser();
}

void JobOrderParserTest::tearDown() {
	delete parser;
}

void JobOrderParserTest::testJobOrderParsing() {
	XPathInitializer init;
	shared_ptr<JobOrder> jobOrder = parser->parse(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.0.xml");

	checkConfiguration(jobOrder);
	checkProcessorConfigurations(jobOrder);
}

void JobOrderParserTest::checkConfiguration(
		const shared_ptr<JobOrder> jobOrder) {
	const IpfConfiguration configuration = jobOrder->getIpfConfiguration();

	CPPUNIT_ASSERT(configuration.getOrderId().compare("0") == 0);
	CPPUNIT_ASSERT(configuration.getProcessorName().compare("S3L2PP-SYN") == 0);
	CPPUNIT_ASSERT(configuration.getVersion().compare("01.00") == 0);
	CPPUNIT_ASSERT(configuration.getStandardLogLevel().compare("DEBUG") == 0);
	CPPUNIT_ASSERT(configuration.getErrorLogLevel().compare("DEBUG") == 0);
	CPPUNIT_ASSERT(configuration.isTest());
	CPPUNIT_ASSERT(
			configuration.getAcquisitionStation().compare("unknown") == 0);
	CPPUNIT_ASSERT(
			configuration.getProcessingStation().compare("unknown") == 0);
	CPPUNIT_ASSERT(configuration.getConfigFiles().size() == 1);
	CPPUNIT_ASSERT(
			configuration.getConfigFiles().at(0).compare("src/test/resources/config/config.xml") == 0);
	CPPUNIT_ASSERT(
			configuration.getSensingTimeStart().compare("20101214_000000.0000") == 0);
	CPPUNIT_ASSERT(
			configuration.getSensingTimeStop().compare("20101214_235959.9999") == 0);
	CPPUNIT_ASSERT(configuration.getDynamicProcessingParameters().size() == 13);
	CPPUNIT_ASSERT(
			configuration.getDynamicProcessingParameters().at(0).getName().compare("Segment_Line_Count") == 0);
	CPPUNIT_ASSERT(
			configuration.getDynamicProcessingParameters().at(0).getValue().compare("2000") == 0);
	CPPUNIT_ASSERT(
			configuration.getDynamicProcessingParameters().at(1).getName().compare("VGT_Northernmost_Latitude") == 0);
	CPPUNIT_ASSERT(
			configuration.getDynamicProcessingParameters().at(1).getValue().compare("75.0") == 0);
	CPPUNIT_ASSERT(
			configuration.getDynamicProcessingParameters().at(2).getName().compare("VGT_Southernmost_Latitude") == 0);
	CPPUNIT_ASSERT(
			configuration.getDynamicProcessingParameters().at(2).getValue().compare("-56.0") == 0);
	CPPUNIT_ASSERT(
			configuration.getDynamicProcessingParameters().at(3).getName().compare("VGT_Westernmost_Longitude") == 0);
	CPPUNIT_ASSERT(
			configuration.getDynamicProcessingParameters().at(3).getValue().compare("-180.0") == 0);
	CPPUNIT_ASSERT(
			configuration.getDynamicProcessingParameters().at(4).getName().compare("VGT_Easternmost_Longitude") == 0);
	CPPUNIT_ASSERT(
			configuration.getDynamicProcessingParameters().at(4).getValue().compare("180.0") == 0);
	CPPUNIT_ASSERT(
			configuration.getDynamicProcessingParameters().at(5).getName().compare("VGT_Synthesis_Period") == 0);
	CPPUNIT_ASSERT(
			configuration.getDynamicProcessingParameters().at(5).getValue().compare("1") == 0);
}

void JobOrderParserTest::checkProcessorConfigurations(
		const shared_ptr<JobOrder> jobOrder) {
	const vector<IpfProcessor> processorConfigurations =
			jobOrder->getIpfProcessors();

	CPPUNIT_ASSERT(processorConfigurations.size() == 1);
	CPPUNIT_ASSERT(
			processorConfigurations.at(0).getTaskName().compare("SYL2") == 0);
	CPPUNIT_ASSERT(
			processorConfigurations.at(0).getTaskVersion().compare("01.00") == 0);
	CPPUNIT_ASSERT(processorConfigurations.at(0).getInputList().size() == 1);
	CPPUNIT_ASSERT(
			processorConfigurations.at(0).getInputList().at(0).getFileType().compare("SY1") == 0);
	CPPUNIT_ASSERT(
			processorConfigurations.at(0).getInputList().at(0).getFileNameType().compare("Physical") == 0);
	CPPUNIT_ASSERT(
			processorConfigurations.at(0).getInputList().at(0).getFileNames().size() == 1);
	CPPUNIT_ASSERT(
			processorConfigurations.at(0).getInputList().at(0).getFileNames().at(0).compare("testdata/SY1/S3A_SY_1_SYN________2013621T10920_2013621T101413_2_00293_2_DMS_TD04_00") == 0);

	CPPUNIT_ASSERT(processorConfigurations.at(0).getOutputList().size() == 1);
	CPPUNIT_ASSERT(
			processorConfigurations.at(0).getOutputList().at(0).getFileType().compare("SY2") == 0);
	CPPUNIT_ASSERT(
			processorConfigurations.at(0).getOutputList().at(0).getFileNameType().compare("Physical") == 0);
	CPPUNIT_ASSERT(
			processorConfigurations.at(0).getOutputList().at(0).getFileName().compare("testdata/SY2/test/some_file_name") == 0);

	CPPUNIT_ASSERT(
			processorConfigurations.at(0).getBreakpointFiles().size() == 0);
}
