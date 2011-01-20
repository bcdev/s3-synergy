/* 
 * Copyright (C) 2011 by Brockmann Consult (info@brockmann-consult.de)
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
 * File:   NullLogging.h
 * Author: ralf
 *
 * Created on January 19, 2011, 3:40 PM
 */

#ifndef NULLLOGGING_H
#define	NULLLOGGING_H

#include "Logging.h"

class NullLogging : public Logging {
public:
    virtual ~NullLogging();

    void debug(const string& message, const string& moduleName,
            const string& processorVersion = Constants::PROCESSOR_VERSION) {
    };

    void info(const string& message, const string& moduleName,
            const string& processorVersion = Constants::PROCESSOR_VERSION) {
    };

    void progress(const string& message, const string& moduleName,
            const string& processorVersion = Constants::PROCESSOR_VERSION) {
    };

    void warning(const string& message, const string& moduleName,
            const string& processorVersion = Constants::PROCESSOR_VERSION) {
    };

    void error(const string& message, const string& moduleName,
            const string& processorVersion = Constants::PROCESSOR_VERSION) {
    };

    void close() {
        
    };

    static Logging& getInstance() {
        return instance;
    }
    
private:
    NullLogging();

    NullLogging(const NullLogging& orig);

    static NullLogging instance;
};

#endif	/* NULLLOGGING_H */

