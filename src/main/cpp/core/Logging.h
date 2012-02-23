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

#ifndef LOGGING_H
#define	LOGGING_H

#include <fstream>
#include <string>

using std::ofstream;
using std::string;

/**
 * Represent the IPF Logging interface.
 */
class Logging {
public:

    /**
     * Logs a debug message.
     * @param message The message.
     * @param moduleName The module name that calls the method.
     */
    virtual void debug(const string& message, const string& moduleName) = 0;

    /**
     * Logs an info message.
     * @param message The message.
     * @param moduleName The module name that calls the method.
     */
    virtual void info(const string& message, const string& moduleName) = 0;

    /**
     * Logs a progress message.
     * @param message The message.
     * @param moduleName The module name that calls the method.
     */
    virtual void progress(const string& message, const string& moduleName) = 0;

    /**
     * Logs a warning message.
     * @param message The message.
     * @param moduleName The module name that calls the method.
     */
    virtual void warning(const string& message, const string& moduleName) = 0;

    /**
     * Logs an error message.
     * @param message The message.
     * @param moduleName The module name that calls the method.
     */
    virtual void error(const string& message, const string& moduleName) = 0;

protected:
    /**
     * Constructs a new instance of this class.
     */
    Logging();

    /**
     * Destructor.
     */
    virtual ~Logging() {
    };

    void openLogFile(const string& logFileName);
    string createMessageHeader(const string& moduleName, const string& moduleVersion) const;
    void logToError(const string& message, const string& moduleName, const string& moduleVersion);
    void logToStdout(const string& message, const string& moduleName, const string& moduleVersion, const string& logType);
    string getTimeString() const;

    const static string PROCESSOR_VERSION;

    ofstream logFile;
};

#endif	/* LOGGING_H */

