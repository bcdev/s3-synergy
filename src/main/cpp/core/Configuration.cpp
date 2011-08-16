/*
 * Configuration.cpp
 *
 *  Created on: Nov 9, 2010
 *      Author: thomass
 */

#include <iostream>

#include "Boost.h"
#include "Configuration.h"
#include "../util/Logger.h"

using std::cout;
using std::endl;
using std::string;

Configuration::Configuration() {
    setTest(false);
    setBreakpointEnable(false);
    setStandardLogLevel("INFO");
    setErrorLogLevel("INFO");
}

Configuration::~Configuration() {
}

string Configuration::getProcessorName() const {
    return processorName;
}

void Configuration::setProcessorName(const string& processorName) {
    this->processorName = processorName;
}

void Configuration::log(Logging& logging) const {
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

void Configuration::setVersion(string version) {
    this->version = version;
}

string Configuration::getVersion() const {
    return version;
}

void Configuration::setConfigFileNames(const vector<string>& configFileNames) {
    this->configFileNames = configFileNames;
}

vector<string> Configuration::getConfigFiles() const {
    return configFileNames;
}

void Configuration::setSensingTimeStop(const string& sensingTimeStop) {
    this->sensingTimeStop = sensingTimeStop;
}

string Configuration::getSensingTimeStop() const {
    return sensingTimeStop;
}

void Configuration::setSensingTimeStart(const string& sensingTimeStart) {
    this->sensingTimeStart = sensingTimeStart;
}

string Configuration::getSensingTimeStart() const {
    return sensingTimeStart;
}

void Configuration::setProcessingStation(const string& processingStation) {
    this->processingStation = processingStation;
}

string Configuration::getProcessingStation() const {
    return processingStation;
}

void Configuration::setAcquisitionStation(const string& acquisitionStation) {
    this->acquisitionStation = acquisitionStation;
}

string Configuration::getAcquisitionStation() const {
    return acquisitionStation;
}

void Configuration::setBreakpointEnable(bool breakpointEnable) {
    this->breakpointEnable = breakpointEnable;
}

bool Configuration::isBreakpointEnable() const {
    return breakpointEnable;
}

void Configuration::setTest(bool test) {
    this->test = test;
}

bool Configuration::isTest() const {
    return test;
}

void Configuration::setErrorLogLevel(const string& errorLogLevel) {
    this->errorLogLevel = errorLogLevel;
}

string Configuration::getErrorLogLevel() const {
    return errorLogLevel;
}

void Configuration::setStandardLogLevel(const string& standardLogLevel) {
    this->standardLogLevel = standardLogLevel;
}

string Configuration::getStandardLogLevel() const {
    return standardLogLevel;
}

void Configuration::setProcessingParameters(const vector<ProcessingParameter>& processingParameters) {
    this->processingParameters = processingParameters;
}

vector<ProcessingParameter> Configuration::getDynamicProcessingParameters() const {
    return processingParameters;
}

void Configuration::setOrderId(const string& orderId) {
    this->orderId = orderId;
}

string Configuration::getOrderId() const {
    return orderId;
}

string Configuration::boolToString(bool input) const {
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
