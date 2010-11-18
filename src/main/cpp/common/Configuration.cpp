/*
 * Configuration.cpp
 *
 *  Created on: Nov 9, 2010
 *      Author: thomass
 */

#include <iostream>

#include "Configuration.h"

using std::cout;
using std::endl;
using std::string;

string Configuration::getProcessorName() {
    return processorName;
}
void Configuration::setProcessorName(string processorName) {
    this->processorName = processorName;
}

string Configuration::getInputFileLocation() const {
    return inputFileLocation;
}

void Configuration::setInputFileLocation(string inputFileLocation) {
    this->inputFileLocation = inputFileLocation;
}

void Configuration::print() {

    cout << "processorName = " << getProcessorName() << "\n";
    cout << "inputFileLocation = " << getInputFileLocation() << "\n";
}

string Configuration::boolToString(bool input) {
    string output;
    if (input) {
        return "true";
    } else {
        return "false";
    }
}

