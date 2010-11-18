/*
 * Configuration.h
 *
 *  Created on: Nov 9, 2010
 *      Author: thomass
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string>

using std::string;

class Configuration {
private:
    string processorName;
    string inputFileLocation;
    string boolToString(bool input);
public:

    Configuration() {
    }

    string getProcessorName();
    void setProcessorName(string processorName);
    string getInputFileLocation() const;
    void setInputFileLocation(string inputFileLocation);
    void print();
};

#endif /* CONFIGURATION_H_ */
