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

#include <string>
#include <vector>

using std::string;
using std::vector;

class Logger {
public:
    static Logger* get();
    void logDebug(string message, string moduleName, string moduleVersion);
    void logInfo(string message, string moduleName, string moduleVersion);
    void logProgress(string message, string moduleName, string moduleVersion);
    void logWarning(string message, string moduleName, string moduleVersion);
    void logError(string message, string moduleName, string moduleVersion);
    void writeLogFile(string orderId);
    vector<string*> getMessageBuffer() const;
    void setOutLogLevel(string outLogLevel);
    void setErrLogLevel(string errLogLevel);
protected:
    Logger();
private:
    string createMessageHeader(string moduleName, string moduleVersion);
    void logToError(string message, string moduleName, string moduleVersion);
    void logToStdout(string message, string moduleName, string moduleVersion, string logType);
    string getTimeString();
    vector<string*> messageBuffer;
    static Logger *instanz;
    string outLogLevel;
    string errLogLevel;
    Logger(const Logger&);
};

#endif	/* LOGGER_H */

