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

#include <iostream>

#include "JobOrder.h"

using std::cout;

JobOrder::JobOrder(Configuration config, vector<ProcessorConfiguration> processorConfigs) {
    this->config = config;
    this->processorConfigs = processorConfigs;
}

JobOrder::~JobOrder() {
}

void JobOrder::print() {
    config.print();
    for (size_t i = 0; i < processorConfigs.size(); i++) {
        cout << "processor config " << i + 1 << ":\n";
        processorConfigs.at(i).print();
    }
}

vector<ProcessorConfiguration> JobOrder::getProcessorList() const {
    return processorConfigs;
}

