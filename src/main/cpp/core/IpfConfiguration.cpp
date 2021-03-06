/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

#include <iostream>
#include <stdexcept>

#include "Boost.h"
#include "IpfConfiguration.h"
#include "Logging.h"

using std::cout;
using std::endl;
using std::logic_error;
using std::string;

IpfConfiguration::IpfConfiguration() {
	setTest(false);
	setBreakpointEnable(false);
	setStandardLogLevel("INFO");
	setErrorLogLevel("INFO");
}

IpfConfiguration::IpfConfiguration(const IpfConfiguration& configuration) :
		orderId(configuration.orderId),
		processorName(configuration.processorName),
		version(configuration.version),
		standardLogLevel(configuration.standardLogLevel),
		errorLogLevel(configuration.errorLogLevel),
		test(configuration.test),
		breakpointEnable(configuration.breakpointEnable),
		acquisitionStation(configuration.acquisitionStation),
		processingStation(configuration.processingStation),
		sensingTimeStart(configuration.sensingTimeStart),
		sensingTimeStop(configuration.sensingTimeStop),
		configFileNames(configuration.configFileNames),
		processingParameters(configuration.processingParameters),
		auxiliaryFileMap(configuration.auxiliaryFileMap) {
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
	logging.debug("breakpoint enabled = " + boolToString(isBreakpointEnable()),
			"JobOrder");
	logging.debug("acquisition station = " + getAcquisitionStation(),
			"JobOrder");
	logging.debug("processing station = " + getProcessingStation(), "JobOrder");
	logging.debug("start time = " + getSensingTimeStart(), "JobOrder");
	logging.debug("stop time = " + getSensingTimeStop(), "JobOrder");
	for (size_t i = 0; i < configFileNames.size(); i++) {
		logging.debug(
				"configuration file " + boost::lexical_cast<string>(i + 1)
						+ ": " + configFileNames[i], "JobOrder");
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

void IpfConfiguration::setConfigFileNames(
		const vector<string>& configFileNames) {
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

void IpfConfiguration::setDynamicProcessingParameters(
		const vector<ProcessingParameter>& processingParameters) {
	this->processingParameters = processingParameters;
}

vector<ProcessingParameter> IpfConfiguration::getDynamicProcessingParameters() const {
	return processingParameters;
}

string IpfConfiguration::getDynamicProcessingParameter(
		const string& name) const {
	for (size_t i = 0; i < processingParameters.size(); i++) {
		if (processingParameters[i].getName().compare(name) == 0) {
			return processingParameters[i].getValue();
		}
	}
	return "";
}

void IpfConfiguration::addAuxFileName(const string& id, const string& path) {
	if (hasAuxFileName(id)) {
		BOOST_THROW_EXCEPTION(
				logic_error("Auxiliary file ID '" + id + "' is already in use."));
	}
	auxiliaryFileMap[id] = path;
}

bool IpfConfiguration::hasAuxFileName(const string& id) const {
	return auxiliaryFileMap.find(id) != auxiliaryFileMap.end();
}

string IpfConfiguration::getAuxFileName(const string& id) const {
	if (!hasAuxFileName(id)) {
		BOOST_THROW_EXCEPTION(
				logic_error("No auxiliary file for ID '" + id + "'."));
	}
	return auxiliaryFileMap.at(id);
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

ProcessingParameter::ProcessingParameter(const string& name,
		const string& value) {
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
