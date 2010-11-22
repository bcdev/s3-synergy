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
    string value = getStringFromNode( evaluateXPathQuery(query) );
    config.setProcessorName(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Version";
    value = getStringFromNode(evaluateXPathQuery(query));
    config.setVersion(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Stdout_Log_Level";
    value = getStringFromNode(evaluateXPathQuery(query));
    config.setStandardLogLevel(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Stderr_Log_Level";
    value = getStringFromNode(evaluateXPathQuery(query));
    config.setVersion(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Test";
    value = getStringFromNode(evaluateXPathQuery(query));
    config.setTest(stringToBool(value));

    query = "/Ipf_Job_Order/Ipf_Conf/Acquisition_Station";
    value = getStringFromNode(evaluateXPathQuery(query));
    config.setAcquisitionStation(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Processing_Station";
    value = getStringFromNode(evaluateXPathQuery(query));
    config.setProcessingStation(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Sensing_Time/Start";
    value = getStringFromNode(evaluateXPathQuery(query));
    config.setSensingTimeStart(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Sensing_Time/Stop";
    value = getStringFromNode(evaluateXPathQuery(query));
    config.setSensingTimeStop(value);

    return config;
}

bool JobOrderParser::stringToBool(string in) {
    if (in.compare("true") == 0 || in.compare("True") == 0
            || in.compare("TRUE")) {
        return true;
    }
    return false;
}

string JobOrderParser::getStringFromNode(const XObject* result) {
    const xalanc::XalanDOMString& resultString = result->str();
    return XMLString::transcode(resultString.data());
}
