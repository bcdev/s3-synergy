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
 * File:   DefaultLogging.cpp
 * Author: thomass
 * 
 * Created on November 24, 2010, 4:08 PM
 */

#include <iostream>
#include <ctime>
#include <unistd.h>
#include <boost/lexical_cast.hpp>

#include "DefaultLogging.h"

using std::cout;
using std::cerr;

DefaultLogging::DefaultLogging(const string& logFileName) :
		outLogLevel(LOG_LEVEL_INFO), errLogLevel(LOG_LEVEL_INFO), logFile() {
	logFile.open(logFileName.c_str());
}

DefaultLogging::~DefaultLogging() {
	if (logFile.is_open()) {
		logFile.close();
	}
}

void DefaultLogging::debug(const string& message, const string& moduleName) {
	if (this->outLogLevel.compare("DEBUG") == 0) {
		logToStdout(message, moduleName, PROCESSOR_VERSION, "[D]");
	}
}

void DefaultLogging::info(const string& message, const string& moduleName) {
	if (this->outLogLevel.compare("DEBUG") == 0
			|| this->outLogLevel.compare("INFO") == 0) {
		logToStdout(message, moduleName, PROCESSOR_VERSION, "[I]");
	}
}

void DefaultLogging::progress(const string& message, const string& moduleName) {
	if (this->outLogLevel.compare("DEBUG") == 0
			|| this->outLogLevel.compare("INFO") == 0
			|| this->outLogLevel.compare("PROGRESS") == 0) {
		logToStdout(message, moduleName, PROCESSOR_VERSION, "[P]");
	}
}

void DefaultLogging::warning(const string& message, const string& moduleName) {
	if (this->outLogLevel.compare("DEBUG") == 0
			|| this->outLogLevel.compare("INFO") == 0
			|| this->outLogLevel.compare("PROGRESS") == 0
			|| this->outLogLevel.compare("WARNING") == 0) {
		logToStdout(message, moduleName, PROCESSOR_VERSION, "[W]");
	}
}

void DefaultLogging::error(const string& message, const string& moduleName) {
	logToError(message, moduleName, PROCESSOR_VERSION);
}

void DefaultLogging::setOutLogLevel(const string& outLogLevel) {
	this->outLogLevel = outLogLevel;
}

void DefaultLogging::setErrLogLevel(const string& errLogLevel) {
	this->errLogLevel = errLogLevel;
}

string DefaultLogging::createMessageHeader(const string& moduleName,
		const string& moduleVersion) const {
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
	header.append(boost::lexical_cast<string>(getpid()));
	header.append("]: ");

	return header;
}

void DefaultLogging::logToError(const string& message, const string& moduleName,
		const string& moduleVersion) {
	string logMessage;
	logMessage.append(createMessageHeader(moduleName, moduleVersion));
	logMessage.append("[E] ");
	logMessage.append(message);

	logFile << logMessage << "\n";
	cerr << logMessage << "\n";
}

void DefaultLogging::logToStdout(const string& message,
		const string& moduleName, const string& moduleVersion,
		const string& logType) {
	string logMessage;
	logMessage.append(createMessageHeader(moduleName, moduleVersion));
	logMessage.append(logType);
	logMessage.append(" ");
	logMessage.append(message);

	logFile << logMessage << "\n";
	cout << logMessage << "\n";
}

string DefaultLogging::getTimeString() const {
	using std::gmtime;
	using std::time;
	using std::strftime;

	time_t t;
	time(&t);
	char utcString[80];
	strftime(utcString, 80, "%Y-%m-%dT%H:%M:%S.000000", gmtime(&t));

	return string(utcString);
}
