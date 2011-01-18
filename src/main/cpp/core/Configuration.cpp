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

string Configuration::getProcessorName() {
    return processorName;
}

void Configuration::setProcessorName(string processorName) {
    this->processorName = processorName;
}

// TODO - replace by toString() and remove Logger dependency

void Configuration::log() {
    Logger* logger = Logger::get();
    logger->debug("order_id = " + getOrderId(), "JobOrder");
    logger->debug("processorName = " + getProcessorName(), "JobOrder");
    logger->debug("version = " + getVersion(), "JobOrder");
    logger->debug("stdLogLevel = " + getStandardLogLevel(), "JobOrder");
    logger->debug("errorLogLevel = " + getErrorLogLevel(), "JobOrder");
    logger->debug("Test = " + boolToString(isTest()), "JobOrder");
    logger->debug("breakpoint enabled = " + boolToString(isBreakpointEnable()), "JobOrder");
    logger->debug("acquisition station = " + getAcquisitionStation(), "JobOrder");
    logger->debug("processing station = " + getProcessingStation(), "JobOrder");
    logger->debug("start time = " + getSensingTimeStart(), "JobOrder");
    logger->debug("stop time = " + getSensingTimeStop(), "JobOrder");
    for (size_t i = 0; i < getConfigFileNames().size(); i++) {
        logger->debug("config file " + boost::lexical_cast<string>(i + 1) + ": " + getConfigFileNames().at(i), "JobOrder");
    }
    for (size_t i = 0; i < getProcessingParameters().size(); i++) {
        getProcessingParameters().at(i)->log();
    }
}

void Configuration::setVersion(string version) {
    this->version = version;
}

string Configuration::getVersion() const {
    return version;
}

void Configuration::setConfigFileNames(vector<string> configFileNames) {
    this->configFileNames = configFileNames;
}

vector<string> Configuration::getConfigFileNames() const {
    return configFileNames;
}

void Configuration::setSensingTimeStop(string sensingTimeStop) {
    this->sensingTimeStop = sensingTimeStop;
}

string Configuration::getSensingTimeStop() const {
    return sensingTimeStop;
}

void Configuration::setSensingTimeStart(string sensingTimeStart) {
    this->sensingTimeStart = sensingTimeStart;
}

string Configuration::getSensingTimeStart() const {
    return sensingTimeStart;
}

void Configuration::setProcessingStation(string processingStation) {
    this->processingStation = processingStation;
}

string Configuration::getProcessingStation() const {
    return processingStation;
}

void Configuration::setAcquisitionStation(string acquisitionStation) {
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

void Configuration::setErrorLogLevel(string errorLogLevel) {
    this->errorLogLevel = errorLogLevel;
}

string Configuration::getErrorLogLevel() const {
    return errorLogLevel;
}

void Configuration::setStandardLogLevel(string standardLogLevel) {
    this->standardLogLevel = standardLogLevel;
}

string Configuration::getStandardLogLevel() const {
    return standardLogLevel;
}

void Configuration::setProcessingParameters(vector<ProcessingParameter*> processingParameters) {
    this->processingParameters = processingParameters;
}

vector<ProcessingParameter*> Configuration::getProcessingParameters() const {
    return processingParameters;
}

void Configuration::setOrderId(string orderId) {
    this->orderId = orderId;
}

string Configuration::getOrderId() const {
    return orderId;
}

string Configuration::boolToString(bool input) {
    if (input) {
        return "true";
    } else {
        return "false";
    }
}

ProcessingParameter::ProcessingParameter(string name, string value) {
    this->name = name;
    this->value = value;
}

ProcessingParameter::~ProcessingParameter() {

}

void ProcessingParameter::setValue(string value) {
    this->value = value;
}

string ProcessingParameter::getValue() const {
    return value;
}

void ProcessingParameter::setName(string name) {
    this->name = name;
}

string ProcessingParameter::getName() const {
    return name;
}

void ProcessingParameter::log() const {
    Logger::get()->debug(getName() + " = " + getValue(), "JobOrder");
}