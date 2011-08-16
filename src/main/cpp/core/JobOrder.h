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
 * File:   JobOrder.h
 * Author: thomass
 *
 * Created on November 23, 2010, 1:35 PM
 */

#ifndef JOBORDER_H
#define	JOBORDER_H

#include <vector>

#include "Configuration.h"
#include "Logging.h"
#include "ProcessorConfiguration.h"

using std::vector;

class JobOrder {
public:
    JobOrder(const Configuration& configuration, const vector<ProcessorConfiguration>& processorConfigurations);
    virtual ~JobOrder();
    
    const Configuration& getConfiguration() const;
    const vector<ProcessorConfiguration>& getProcessorConfigurations() const;
    const ProcessorConfiguration& getProcessorConfiguration(const string& id) const;

    void log(Logging& logging) const;

private:
    Configuration configuration;
    vector<ProcessorConfiguration> processorConfigurations;
};

#endif	/* JOBORDER_H */

