/*
 * Logging.cpp
 *
 *  Created on: Aug 16, 2011
 *      Author: ralf
 */

#include <ctime>
#include <iostream>

#include "../core/Constants.h"
#include "Logging.h"

using std::cerr;
using std::cout;

const string Logging::PROCESSOR_VERSION = Constants::PROCESSOR_VERSION;

Logging::Logging() {
}

void Logging::openLogFile(const string& logFileName) {
    logFile.open(logFileName.c_str());
}

string Logging::createMessageHeader(const string& moduleName, const string& moduleVersion) const {
    char nodeName[80];
    gethostname(nodeName, 80);

    string header = getTimeString();
    header.append(" ");
    header.append(nodeName);
    header.append(" ");
    header.append(moduleName);
    header.append(" ");
    header.append(moduleVersion);
    header.append(" [");
    header.append(lexical_cast<string>(getpid()));
    header.append("]: ");

    return header;
}

void Logging::logToError(const string& message, const string& moduleName, const string& moduleVersion) {
    string logMessage;
    logMessage.append(createMessageHeader(moduleName, moduleVersion));
    logMessage.append("[E] ");
    logMessage.append(message);

    logFile << logMessage << "\n";
    cerr << logMessage << "\n";
}

void Logging::logToStdout(const string& message, const string& moduleName, const string& moduleVersion, const string& logType) {
    string logMessage;
    logMessage.append(createMessageHeader(moduleName, moduleVersion));
    logMessage.append(logType);
    logMessage.append(" ");
    logMessage.append(message);

    logFile << logMessage << "\n";
    cout << logMessage << "\n";
}

string Logging::getTimeString() const {
    using std::gmtime;
    using std::time;
    using std::strftime;

    time_t t;
    time(&t);
    char utcString[80];
    strftime(utcString, 80, "%Y-%m-%dT%H:%M:%S.000000", gmtime(&t));

    return string(utcString);
}
