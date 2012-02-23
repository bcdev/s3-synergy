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

#ifndef IPFCONFIGURATION_H_
#define IPFCONFIGURATION_H_

#include <string>
#include <vector>

#include "Logging.h"

using std::string;
using std::vector;

/**
 * Represents a processing parameter in an IPF Job Order.
 */
class ProcessingParameter {
public:
	ProcessingParameter(const string& name, const string& value);
	~ProcessingParameter();

	void setValue(const string& value);
	string getValue() const;

	void setName(const string& name);
	string getName() const;

	void log(Logging& logging) const;

private:
	string name;
	string value;
};

/**
 * Represents the configuration element in an IPF Job Order.
 */
class IpfConfiguration {
private:
	string orderId;
	string processorName;
	string version;
	string standardLogLevel;
	string errorLogLevel;
	bool test;
	bool breakpointEnable;
	string acquisitionStation;
	string processingStation;
	string sensingTimeStart;
	string sensingTimeStop;
	vector<string> configFileNames;
	vector<ProcessingParameter> processingParameters;

	string boolToString(bool input) const;
public:

	IpfConfiguration();
	~IpfConfiguration();

	string getProcessorName() const;
	void setProcessorName(const string& processorName);

	void setOrderId(const string& orderId);
	string getOrderId() const;
	void log(Logging& logging) const;
	void setVersion(string version);
	string getVersion() const;
	void setConfigFileNames(const vector<string>& configFileNames);
	vector<string> getConfigFiles() const;
	void setSensingTimeStop(const string& sensingTimeStop);
	string getSensingTimeStop() const;
	void setSensingTimeStart(const string& sensingTimeStart);
	string getSensingTimeStart() const;
	void setProcessingStation(const string& processingStation);
	string getProcessingStation() const;
	void setAcquisitionStation(const string& acquisitionStation);
	string getAcquisitionStation() const;
	void setBreakpointEnable(bool breakpointEnable);
	bool isBreakpointEnable() const;
	void setTest(bool test);
	bool isTest() const;
	void setErrorLogLevel(const string& errorLogLevel);
	string getErrorLogLevel() const;
	void setStandardLogLevel(const string& standardLogLevel);
	string getStandardLogLevel() const;
	void setDynamicProcessingParameters(const vector<ProcessingParameter>& processingParameters);
	vector<ProcessingParameter> getDynamicProcessingParameters() const;
	string getDynamicProcessingParameter(const string& name) const;
};

#endif /* IPFCONFIGURATION_H_ */
