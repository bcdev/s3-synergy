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
#include "../util/Logger.h"

using std::cout;
using std::invalid_argument;

JobOrder::JobOrder(Configuration config, vector<ProcessorConfiguration*> processorConfigs) {
    this->config = config;
    this->processorConfigs = processorConfigs;
}

JobOrder::~JobOrder() {
    foreach(ProcessorConfiguration* pc, processorConfigs) {
        delete pc;
    }
}

// TODO - replace by toString() and get rid of logger dependency
void JobOrder::log() {
    config.log();
    for (size_t i = 0; i < processorConfigs.size(); i++) {
        Logger::get()->debug("parsing processor config " + boost::lexical_cast<string>(i + 1) + ":", "JobOrder");
        processorConfigs.at(i)->log();
    }
}

vector<ProcessorConfiguration*> JobOrder::getProcessorList() const {
    return processorConfigs;
}

ProcessorConfiguration& JobOrder::getProcessorConfiguration(const string& id) const {
    for (size_t i = 0; i < processorConfigs.size(); i++) {
        if( processorConfigs[i]->getTaskName().compare( id ) == 0 ) {
            return *processorConfigs[i];
        }
    }
    BOOST_THROW_EXCEPTION(invalid_argument("no processor configuration with name " + id + "."));
}

Configuration JobOrder::getConfig() const {
    return config;
}