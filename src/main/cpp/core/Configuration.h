/*
 * Configuration.h
 *
 *  Created on: Nov 9, 2010
 *      Author: thomass
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string>
#include <vector>

#include "Logging.h"

using std::string;
using std::vector;

class ProcessingParameter {
public:
    ProcessingParameter(string name, string value);
    ~ProcessingParameter();
    void setValue(string value);
    string getValue() const;
    void setName(string name);
    string getName() const;
    void log(Logging& logging) const;

private:
    string name;
    string value;
};

class Configuration {
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
    vector<ProcessingParameter*> processingParameters;

    string boolToString(bool input) const;
public:

    Configuration();
    ~Configuration();

    string getProcessorName() const;
    void setProcessorName(string processorName);

    void setOrderId(string orderId);
    string getOrderId() const;
    void log(Logging& logging) const;
    void setVersion(string version);
    string getVersion() const;
    void setConfigFileNames(vector<string> configFileNames);
    vector<string> getConfigFileNames() const;
    void setSensingTimeStop(string sensingTimeStop);
    string getSensingTimeStop() const;
    void setSensingTimeStart(string sensingTimeStart);
    string getSensingTimeStart() const;
    void setProcessingStation(string processingStation);
    string getProcessingStation() const;
    void setAcquisitionStation(string acquisitionStation);
    string getAcquisitionStation() const;
    void setBreakpointEnable(bool breakpointEnable);
    bool isBreakpointEnable() const;
    void setTest(bool test);
    bool isTest() const;
    void setErrorLogLevel(string errorLogLevel);
    string getErrorLogLevel() const;
    void setStandardLogLevel(string standardLogLevel);
    string getStandardLogLevel() const;
    void setProcessingParameters(vector<ProcessingParameter*> processingParameters);
    vector<ProcessingParameter*> getProcessingParameters() const;
};

#endif /* CONFIGURATION_H_ */
