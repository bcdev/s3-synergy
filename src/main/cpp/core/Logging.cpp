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
