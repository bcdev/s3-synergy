/*
 * Configuration.cpp
 *
 *  Created on: Nov 9, 2010
 *      Author: thomass
 */

#include <iostream>

#include "Boost.h"
#include "IpfConfiguration.h"
#include "Logging.h"

using std::cout;
using std::endl;
using std::string;

IpfConfiguration::IpfConfiguration() {
    setTest(false);
    setBreakpointEnable(false);
    setStandardLogLevel("INFO");
    setErrorLogLevel("INFO");
}

IpfConfiguration::~IpfConfiguration() {
}

string IpfConfiguration::getProcessorName() const {
    return processorName;
}

void IpfConfiguration::setProcessorName(const string& processorName) {
    this->processorName = processorName;
}

void IpfConfiguration::log(Logging& logging) const {
    logging.debug("order_id = " + getOrderId(), "JobOrder");
    logging.debug("processorName = " + getProcessorName(), "JobOrder");
    logging.debug("version = " + getVersion(), "JobOrder");
    logging.debug("stdLogLevel = " + getStandardLogLevel(), "JobOrder");
    logging.debug("errorLogLevel = " + getErrorLogLevel(), "JobOrder");
    logging.debug("Test = " + boolToString(isTest()), "JobOrder");
    logging.debug("breakpoint enabled = " + boolToString(isBreakpointEnable()), "JobOrder");
    logging.debug("acquisition station = " + getAcquisitionStation(), "JobOrder");
    logging.debug("processing station = " + getProcessingStation(), "JobOrder");
    logging.debug("start time = " + getSensingTimeStart(), "JobOrder");
    logging.debug("stop time = " + getSensingTimeStop(), "JobOrder");
    for (size_t i = 0; i < configFileNames.size(); i++) {
        logging.debug("configuration file " + boost::lexical_cast<string>(i + 1) + ": " + configFileNames[i], "JobOrder");
    }
    for (size_t i = 0; i < processingParameters.size(); i++) {
        processingParameters[i].log(logging);
    }
}

void IpfConfiguration::setVersion(string version) {
    this->version = version;
}

string IpfConfiguration::getVersion() const {
    return version;
}

void IpfConfiguration::setConfigFileNames(const vector<string>& configFileNames) {
    this->configFileNames = configFileNames;
}

vector<string> IpfConfiguration::getConfigFiles() const {
    return configFileNames;
}

void IpfConfiguration::setSensingTimeStop(const string& sensingTimeStop) {
    this->sensingTimeStop = sensingTimeStop;
}

string IpfConfiguration::getSensingTimeStop() const {
    return sensingTimeStop;
}

void IpfConfiguration::setSensingTimeStart(const string& sensingTimeStart) {
    this->sensingTimeStart = sensingTimeStart;
}

string IpfConfiguration::getSensingTimeStart() const {
    return sensingTimeStart;
}

void IpfConfiguration::setProcessingStation(const string& processingStation) {
    this->processingStation = processingStation;
}

string IpfConfiguration::getProcessingStation() const {
    return processingStation;
}

void IpfConfiguration::setAcquisitionStation(const string& acquisitionStation) {
    this->acquisitionStation = acquisitionStation;
}

string IpfConfiguration::getAcquisitionStation() const {
    return acquisitionStation;
}

void IpfConfiguration::setBreakpointEnable(bool breakpointEnable) {
    this->breakpointEnable = breakpointEnable;
}

bool IpfConfiguration::isBreakpointEnable() const {
    return breakpointEnable;
}

void IpfConfiguration::setTest(bool test) {
    this->test = test;
}

bool IpfConfiguration::isTest() const {
    return test;
}

void IpfConfiguration::setErrorLogLevel(const string& errorLogLevel) {
    this->errorLogLevel = errorLogLevel;
}

string IpfConfiguration::getErrorLogLevel() const {
    return errorLogLevel;
}

void IpfConfiguration::setStandardLogLevel(const string& standardLogLevel) {
    this->standardLogLevel = standardLogLevel;
}

string IpfConfiguration::getStandardLogLevel() const {
    return standardLogLevel;
}

void IpfConfiguration::setDynamicProcessingParameters(const vector<ProcessingParameter>& processingParameters) {
    this->processingParameters = processingParameters;
}

vector<ProcessingParameter> IpfConfiguration::getDynamicProcessingParameters() const {
    return processingParameters;
}

string IpfConfiguration::getDynamicProcessingParameter(const string& name) const {
    for (int i = 0; i < processingParameters.size(); i++) {
    	if (processingParameters[i].getName().compare(name) == 0) {
    		return processingParameters[i].getValue();
    	}
    }
    return "";
}

void IpfConfiguration::setOrderId(const string& orderId) {
    this->orderId = orderId;
}

string IpfConfiguration::getOrderId() const {
    return orderId;
}

string IpfConfiguration::boolToString(bool input) const {
    if (input) {
        return "true";
    } else {
        return "false";
    }
}

ProcessingParameter::ProcessingParameter(const string& name, const string& value) {
    this->name = name;
    this->value = value;
}

ProcessingParameter::~ProcessingParameter() {

}

void ProcessingParameter::setValue(const string& value) {
    this->value = value;
}

string ProcessingParameter::getValue() const {
    return value;
}

void ProcessingParameter::setName(const string& name) {
    this->name = name;
}

string ProcessingParameter::getName() const {
    return name;
}

void ProcessingParameter::log(Logging& logging) const {
    logging.debug(getName() + " = " + getValue(), "JobOrder");
}
