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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 *
 * File:   Logging.h
 * Author: ralf
 *
 * Created on December 21, 2010, 9:09 AM
 */

#ifndef LOGGING_H
#define	LOGGING_H

#include "Constants.h"

class Logging {
public:

    virtual ~Logging() {
    }

    virtual void debug(const string& message, const string& moduleName) = 0;
    virtual void debug(const string& message, const string& moduleName, const string& processorVersion) = 0;
    virtual void info(const string& message, const string& moduleName) = 0;
    virtual void info(const string& message, const string& moduleName, const string& processorVersion) = 0;
    virtual void progress(const string& message, const string& moduleName) = 0;
    virtual void progress(const string& message, const string& moduleName, const string& processorVersion) = 0;
    virtual void warning(const string& message, const string& moduleName) = 0;
    virtual void warning(const string& message, const string& moduleName, const string& processorVersion) = 0;
    virtual void error(const string& message, const string& moduleName) = 0;
    virtual void error(const string& message, const string& moduleName, const string& processorVersion) = 0;

    const static string LOG_LEVEL_INFO;
    const static string LOG_LEVEL_PROGRESS;
    const static string LOG_LEVEL_DEBUG;
    const static string LOG_LEVEL_WARNING;
    const static string LOG_LEVEL_ERROR;
};

#endif	/* LOGGING_H */

