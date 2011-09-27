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
 * File:   JobOrder.cpp
 * Author: thomass
 * 
 * Created on November 23, 2010, 1:35 PM
 */

#include <stdexcept>
#include <iostream>

#include "Boost.h"
#include "../logging/DebugLogging.h"
#include "JobOrder.h"

using std::cout;
using std::invalid_argument;

JobOrder::JobOrder() : ipfConfiguration(), ipfProcessors() {
}

JobOrder::JobOrder(const IpfConfiguration& ipfConfiguration, const vector<IpfProcessor>& ipfProcessors) {
    this->ipfConfiguration = ipfConfiguration;
    this->ipfProcessors = ipfProcessors;
}

JobOrder::~JobOrder() {
}

void JobOrder::log(Logging& logging) const {
    ipfConfiguration.log(logging);
    for (size_t i = 0; i < ipfProcessors.size(); i++) {
        logging.debug("Parsing IPF processors " + boost::lexical_cast<string > (i + 1) + ":", "JobOrder");
        ipfProcessors[i].log(logging);
    }
}

const vector<IpfProcessor>& JobOrder::getIpfProcessors() const {
    return ipfProcessors;
}

const IpfProcessor& JobOrder::getIpfProcessor(const string& id) const {
    for (size_t i = 0; i < ipfProcessors.size(); i++) {
        if (ipfProcessors[i].getTaskName().compare(id) == 0) {
            return ipfProcessors[i];
        }
    }
    BOOST_THROW_EXCEPTION(invalid_argument("No IPF processor with name " + id + "."));
}

const IpfConfiguration& JobOrder::getIpfConfiguration() const {
    return ipfConfiguration;
}

const shared_ptr<Logging> JobOrder::createLogging(const string& logFileName) const {
    const string& stdLogLevel = ipfConfiguration.getStandardLogLevel();
    if(stdLogLevel.compare("DEBUG") == 0) {
        return shared_ptr<Logging>(new DebugLogging(logFileName));
    } else if(stdLogLevel.compare("INFO") == 0) {
        return shared_ptr<Logging>(new InfoLogging(logFileName));
    }  else if(stdLogLevel.compare("PROGRESS") == 0) {
        return shared_ptr<Logging>(new ProgressLogging(logFileName));
    } else if(stdLogLevel.compare("WARNING") == 0) {
        return shared_ptr<Logging>(new WarningLogging(logFileName));
    } else if(stdLogLevel.compare("ERROR") == 0) {
        return shared_ptr<Logging>(new ErrorLogging(logFileName));
    } else if(stdLogLevel.compare("NULL") == 0) {
        return shared_ptr<Logging>(new NullLogging());
    }
    BOOST_THROW_EXCEPTION(invalid_argument("invalid standard log level '" + stdLogLevel + "'."));
}
