/* 
 * Copyright (C) 2010 by Brockmann Consult (info@brockmann-consult.de)
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * File:   Logger.cpp
 * Author: thomass
 * 
 * Created on November 24, 2010, 4:08 PM
 */

#include <iostream>
#include <time.h>
#include <unistd.h>
#include <boost/lexical_cast.hpp>

#include "Logger.h"

using std::cout;
using std::cerr;

Logger::Logger(const string& logFileName) : logFile() {
    logFile.open(logFileName.c_str());
}

Logger::Logger(const Logger& logger) {
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::debug(const string& message, const string& moduleName) {
    debug(message, moduleName, processorVersion);
}

void Logger::debug(const string& message, const string& moduleName,
        const string& processorVersion) {
    if (this->outLogLevel.compare("DEBUG") == 0) {
        logToStdout(message, moduleName, processorVersion, "[D]");
    }
}

void Logger::info(const string& message, const string& moduleName) {
    info(message, moduleName, processorVersion);
}

void Logger::info(const string& message, const string& moduleName,
        const string& processorVersion) {
    if (this->outLogLevel.compare("DEBUG") == 0 ||
            this->outLogLevel.compare("INFO") == 0) {
        logToStdout(message, moduleName, processorVersion, "[I]");
    }
}

void Logger::progress(const string& message, const string& moduleName) {
    progress(message, moduleName, processorVersion);
}

void Logger::progress(const string& message, const string& moduleName,
        const string& processorVersion) {
    if (this->outLogLevel.compare("DEBUG") == 0 ||
            this->outLogLevel.compare("INFO") == 0 ||
            this->outLogLevel.compare("PROGRESS") == 0) {
        logToStdout(message, moduleName, processorVersion, "[P]");
    }
}

void Logger::warning(const string& message, const string& moduleName) {
    warning(message, moduleName, processorVersion);
}

void Logger::warning(const string& message, const string& moduleName,
        const string& processorVersion) {
    if (this->outLogLevel.compare("DEBUG") == 0 ||
            this->outLogLevel.compare("INFO") == 0 ||
            this->outLogLevel.compare("PROGRESS") == 0 ||
            this->outLogLevel.compare("WARNING") == 0) {
        logToStdout(message, moduleName, processorVersion, "[W]");
    }
}

void Logger::error(const string& message, const string& moduleName) {
    error(message, moduleName, processorVersion);
}

void Logger::error(const string& message, const string& moduleName,
        const string& processorVersion) {
    logToError(message, moduleName, processorVersion);
}

void Logger::setProcessorVersion(const string& processorVersion) {

}

void Logger::setOutLogLevel(const string& outLogLevel) {
    this->outLogLevel = outLogLevel;
}

void Logger::setErrLogLevel(const string& errLogLevel) {
    this->errLogLevel = errLogLevel;
}

string Logger::createMessageHeader(const string& moduleName, const string& moduleVersion) {
    char nodeNameBuffer [80];
    gethostname(nodeNameBuffer, 80);
    string header = getTimeString();
    header.append(" ");
    header.append(nodeNameBuffer);
    header.append(" ");
    header.append(moduleName);
    header.append(" ");
    header.append(moduleVersion);
    header.append(" [");
    header.append(boost::lexical_cast<string > ((int) getpid()));
    header.append("]: ");
    return header;
}

void Logger::logToError(const string& message, const string& moduleName, const string& moduleVersion) {
    string logMessage;
    logMessage.append(createMessageHeader(moduleName, moduleVersion));
    logMessage.append("[E] ");
    logMessage.append(message);

    logFile << logMessage << "\n";
    cerr << logMessage << "\n";
}

void Logger::logToStdout(const string& message, const string& moduleName, const string& moduleVersion, const string& logType) {
    string logMessage;
    logMessage.append(createMessageHeader(moduleName, moduleVersion));
    logMessage.append(logType);
    logMessage.append(" ");
    logMessage.append(message);

    logFile << logMessage << "\n";
    cout << logMessage << "\n";
}

string Logger::getTimeString() {
    time_t rawtime;
    tm* timer;
    time(&rawtime);
    timer = localtime(&rawtime);
    char timeBuffer [80];
    strftime(timeBuffer, 80, "%Y-%m-%dT%H:%M:%S.000000", timer);
    return timeBuffer;
}
