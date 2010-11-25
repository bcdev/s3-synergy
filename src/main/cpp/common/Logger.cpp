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

#include <fstream>
#include <iostream>
#include <time.h>
#include <unistd.h>

#include "Logger.h"
#include "StringUtils.h"

using std::cout;
using std::cerr;
using std::ofstream;

Logger::Logger(string outLogLevel, string errLogLevel) : messageBuffer() {
    this->outLogLevel = outLogLevel;
    this->errLogLevel = errLogLevel;
}

void Logger::logDebug(string message, string moduleName, string moduleVersion) {
    if (this->outLogLevel.compare("DEBUG") == 0) {
        logToStdout(message, moduleName, moduleVersion, "[D]");
    }
}

void Logger::logInfo(string message, string moduleName, string moduleVersion) {
    if (this->outLogLevel.compare("DEBUG") == 0 ||
            this->outLogLevel.compare("INFO") == 0) {
        logToStdout(message, moduleName, moduleVersion, "[I]");
    }
}

void Logger::logProgress(string message, string moduleName, string moduleVersion) {
    if (this->outLogLevel.compare("DEBUG") == 0 ||
            this->outLogLevel.compare("INFO") == 0 ||
            this->outLogLevel.compare("PROGRESS") == 0) {
    logToStdout(message, moduleName, moduleVersion, "[P]");
    }
}

void Logger::logWarning(string message, string moduleName, string moduleVersion) {
    if (this->outLogLevel.compare("DEBUG") == 0 ||
            this->outLogLevel.compare("INFO") == 0 ||
            this->outLogLevel.compare("PROGRESS") == 0 ||
            this->outLogLevel.compare("WARNING") == 0) {
    logToStdout(message, moduleName, moduleVersion, "[W]");
    }
}

void Logger::logError(string message, string moduleName, string moduleVersion) {
    logToError(message, moduleName, moduleVersion);
}

void Logger::writeLogFile(string orderId) {
    ofstream logFile;
    string fileName = "LOG.";
    fileName.append(orderId);
    logFile.open(fileName.c_str());
    for( size_t i = 0; i < messageBuffer.size(); i++ ) {
        logFile << *(messageBuffer.at(i)) << "\n";
    }
    logFile.close();
}
vector<string*> Logger::getMessageBuffer() const {
    return messageBuffer;
}

string Logger::createMessageHeader(string moduleName, string moduleVersion) {
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
    header.append(StringUtils::intToString((int) getpid()));
    header.append("]: ");
    return header;
}

void Logger::logToError(string message, string moduleName, string moduleVersion) {
    string outputMessage = createMessageHeader(moduleName, moduleVersion);
    outputMessage.append("[E] ");
    outputMessage.append(message);

    messageBuffer.push_back(&outputMessage);
    cerr << outputMessage << "\n";
}

void Logger::logToStdout(string message, string moduleName, string moduleVersion, string logType) {
    string* logMessage = new string();
    logMessage->append( createMessageHeader(moduleName, moduleVersion) );
    logMessage->append(logType);
    logMessage->append(" ");
    logMessage->append(message);

    messageBuffer.push_back(logMessage);
    cout << *logMessage << "\n";
}

string Logger::getTimeString() {
    time_t rawtime;
    tm* timer;
    char timeBuffer [80];
    time(&rawtime);
    timer = localtime(&rawtime);
    strftime(timeBuffer, 80, "%Y-%m-%dT%H:%M:%S.000000", timer);
    return timeBuffer;
}