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

#include <stdexcept>
#include <iostream>

#include "Boost.h"
#include "JobOrder.h"

using std::cout;
using std::invalid_argument;

JobOrder::JobOrder() : ipfConfiguration(), ipfProcessors() {
}

JobOrder::JobOrder(const IpfConfiguration& ipfConfiguration, const vector<IpfProcessor>& ipfProcessors) : ipfConfiguration(ipfConfiguration), ipfProcessors(ipfProcessors) {
}

JobOrder::~JobOrder() {
}

void JobOrder::log(Logging& logging) const {
    ipfConfiguration.log(logging);
    for (size_t i = 0; i < ipfProcessors.size(); i++) {
        logging.debug("Parsing IPF processors " + boost::lexical_cast<string>(i + 1) + ":", "JobOrder");
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
