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
 * File:   Logger.h
 * Author: thomass
 *
 * Created on November 24, 2010, 4:08 PM
 */

#ifndef LOGGER_H
#define	LOGGER_H

#include <fstream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::ofstream;

class Logger {
public:
    ~Logger();
    static Logger* get();
    void debug(string message, string moduleName, string moduleVersion);
    void info(string message, string moduleName, string moduleVersion);
    void progress(string message, string moduleName, string moduleVersion);
    void warning(string message, string moduleName, string moduleVersion);
    void error(string message, string moduleName, string moduleVersion);
    void debug(string message, string moduleName);
    void info(string message, string moduleName);
    void progress(string message, string moduleName);
    void warning(string message, string moduleName);
    void error(string message, string moduleName);
    void writeLogFile(string orderId);
    vector<string*> getMessageBuffer() const;
    void setOutLogLevel(string outLogLevel);
    void setErrLogLevel(string errLogLevel);
    void init(string orderId);
protected:
    Logger();
private:
    string createMessageHeader(string moduleName, string moduleVersion);
    void logToError(string message, string moduleName, string moduleVersion);
    void logToStdout(string message, string moduleName, string moduleVersion, string logType);
    string getTimeString();
    static Logger *instance;
    string outLogLevel;
    string errLogLevel;
    string orderId;
    Logger(const Logger&);
    ofstream logFile;
};

#endif	/* LOGGER_H */

