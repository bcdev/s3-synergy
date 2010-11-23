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
 * File:   JobOrderParser.cpp
 * Author: thomass
 * 
 * Created on November 15, 2010, 4:31 PM
 */

#include <iostream>

#include "Configuration.h"
#include "JobOrderParser.h"

using std::string;
using std::cout;

JobOrderParser::JobOrderParser(string path) : XmlParser(path) {
}

JobOrderParser::~JobOrderParser() {
    delete this->parser;
}

Configuration JobOrderParser::parseConfiguration() {
    Configuration config;

    string query = "/Ipf_Job_Order/Ipf_Conf/Processor_Name";
    string value = evaluateToString(query);
    config.setProcessorName(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Version";
    value = evaluateToString(query);
    config.setVersion(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Stdout_Log_Level";
    value = evaluateToString(query);
    config.setStandardLogLevel(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Stderr_Log_Level";
    value = evaluateToString(query);
    config.setVersion(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Test";
    value = evaluateToString(query);
    config.setTest(stringToBool(value));

    query = "/Ipf_Job_Order/Ipf_Conf/Acquisition_Station";
    value = evaluateToString(query);
    config.setAcquisitionStation(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Processing_Station";
    value = evaluateToString(query);
    config.setProcessingStation(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Sensing_Time/Start";
    value = evaluateToString(query);
    config.setSensingTimeStart(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Sensing_Time/Stop";
    value = evaluateToString(query);
    config.setSensingTimeStop(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Dynamic_Processing_Parameters/Processing_Parameter/Name/child::text()";
    string query2 = "/Ipf_Job_Order/Ipf_Conf/Dynamic_Processing_Parameters/Processing_Parameter/Value/child::text()";
    vector<string> keys = evaluateToStringList(query);
    vector<string> values = evaluateToStringList(query2);
    vector<ProcessingParameter> parameters;
    for (int i = 0; i < keys.size(); i++) {
        parameters.push_back( ProcessingParameter(keys.at(i), values.at(i)) );
    }
    config.setProcessingParameters(parameters);

    query = "/Ipf_Job_Order/Ipf_Conf/Config_Files/Conf_File_Name/child::text()";
    vector<string> strings = evaluateToStringList(query);
    config.setConfigFileNames(strings);

    return config;
}

bool JobOrderParser::stringToBool(string in) {
    if (in.compare("true") == 0 || in.compare("True") == 0
            || in.compare("TRUE")) {
        return true;
    }
    return false;
}