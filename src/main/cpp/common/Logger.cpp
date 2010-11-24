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

#include "Logger.h"
#include "StringUtils.h"

using std::cout;
using std::cerr;

Logger::Logger(string outLogLevel, string errLogLevel) {
    this->outLogLevel = outLogLevel;
    this->errLogLevel = errLogLevel;
}

void Logger::logDebug(string message) {

}

void Logger::logInfo(string message) {

}

void Logger::logProgress(string message) {

}

void Logger::logWarning(string message) {

}

void Logger::logError(string message) {
    logToError(message);
}

void Logger::logToError(string message) {
    char nodeNameBuffer [80];
    gethostname(nodeNameBuffer, 80);

    string outputMessage = getTimeString();
    outputMessage.append(" ");
    outputMessage.append(nodeNameBuffer);
    outputMessage.append(" ");
    outputMessage.append("processor_name");
    outputMessage.append(" ");
    outputMessage.append("processor_version");
    outputMessage.append(" [");
    outputMessage.append( StringUtils::intToString( (int)getpid() ) );
    outputMessage.append("]: ");
    outputMessage.append("[E] ");
    outputMessage.append(message);

    cerr << outputMessage;
}

void Logger::logToStdout(string message) {
    cout << message;
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