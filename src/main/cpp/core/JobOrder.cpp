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
#include "JobOrder.h"

using std::cout;
using std::invalid_argument;

JobOrder::JobOrder(const Configuration& configuration, const vector<ProcessorConfiguration>& processorConfigurations) {
    this->configuration = configuration;
    this->processorConfigurations = processorConfigurations;
}

JobOrder::~JobOrder() {
}

void JobOrder::log(Logging& logging) const {
    configuration.log(logging);
    for (size_t i = 0; i < processorConfigurations.size(); i++) {
        logging.debug("parsing processor configuration " + boost::lexical_cast<string > (i + 1) + ":", "JobOrder");
        processorConfigurations[i].log(logging);
    }
}

const vector<ProcessorConfiguration>& JobOrder::getProcessorConfigurations() const {
    return processorConfigurations;
}

const ProcessorConfiguration& JobOrder::getProcessorConfiguration(const string& id) const {
    for (size_t i = 0; i < processorConfigurations.size(); i++) {
        if (processorConfigurations[i].getTaskName().compare(id) == 0) {
            return processorConfigurations[i];
        }
    }
    BOOST_THROW_EXCEPTION(invalid_argument("no processor configuration with name " + id + "."));
}

const Configuration& JobOrder::getConfig() const {
    return configuration;
}
