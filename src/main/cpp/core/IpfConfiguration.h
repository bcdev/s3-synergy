/*
 * IpfConfiguration.h
 *
 *  Created on: Nov 9, 2010
 *      Author: thomass
 */

#ifndef IPFCONFIGURATION_H_
#define IPFCONFIGURATION_H_

#include <string>
#include <vector>

#include "../logging/Logging.h"

using std::string;
using std::vector;

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
