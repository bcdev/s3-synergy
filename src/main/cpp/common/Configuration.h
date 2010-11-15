/*
 * Configuration.h
 *
 *  Created on: Nov 9, 2010
 *      Author: thomass
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <iostream>

using namespace std;

class Configuration {
private:
    string id;
    string debugLevel;
    string inputFileLocation;
    string synFileLocation;
    string vgtFileLocation;
    bool exportSyn;
    bool exportVgt;
    string boolToString(bool input);
public:

    Configuration() {
        this->exportSyn = false;
        this->exportVgt = false;
    }

    string getId() const;
    void setId(string id);
    bool getExportSyn() const;
    void setExportSyn(bool exportSyn);
    bool getExportVgt() const;
    void setExportVgt(bool exportVgt);
    string getInputFileLocation() const;
    void setInputFileLocation(string inputFileLocation);
    string getSynFileLocation() const;
    void setSynFileLocation(string synFileLocation);
    string getVgtFileLocation() const;
    void setVgtFileLocation(string vgtFileLocation);
    void print();
    string getDebugLevel() const;
    void setDebugLevel(string debugLevel);

};

#endif /* CONFIGURATION_H_ */
