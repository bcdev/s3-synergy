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
 * File:   DefaultLogger.h
 * Author: thomass
 *
 * Created on November 24, 2010, 4:08 PM
 */

#ifndef DEFAULTLOGGING_H
#define	DEFAULTLOGGING_H

#include <fstream>
#include <string>
#include <vector>

#include "../core/Logging.h"

using std::vector;
using std::ofstream;

class DefaultLogging : public virtual Logging {
public:
    DefaultLogging(const string& logFileName);
    virtual ~DefaultLogging();
    
    void debug(const string& message, const string& moduleName,
            const string& processorVersion);
    void debug(const string& message, const string& moduleName);
    void info(const string& message, const string& moduleName,
            const string& processorVersion);
    void info(const string& message, const string& moduleName);
    void progress(const string& message, const string& moduleName,
            const string& processorVersion);
    void progress(const string& message, const string& moduleName);
    void warning(const string& message, const string& moduleName,
            const string& processorVersion);
    void warning(const string& message, const string& moduleName);
    void error(const string& message, const string& moduleName,
            const string& processorVersion);
    void error(const string& message, const string& moduleName);

    void setProcessorVersion(const string& processorVersion);
    void setOutLogLevel(const string& outLogLevel);
    void setErrLogLevel(const string& errLogLevel);

private:
    DefaultLogging(const DefaultLogging& logger);

    string createMessageHeader(const string& moduleName, const string& moduleVersion);
    void logToError(const string& message, const string& moduleName, const string& moduleVersion);
    void logToStdout(const string& message, const string& moduleName, const string& moduleVersion, const string& logType);
    string getTimeString();

    string processorVersion;
    string outLogLevel;
    string errLogLevel;

    ofstream logFile;
};

#endif	/* DEFAULTLOGGING_H */

