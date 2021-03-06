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

#include "BasicModule.h"

void BasicModule::addAccessor(Context& context, Segment& s, const VariableDescriptor& varDescriptor) const {
    context.getLogging().info("Adding accessor for " + varDescriptor.toString() + "to segment " + s.toString(), getId());
    s.addVariable(varDescriptor);
}

AuxdataProvider& BasicModule::getAuxdataProvider(Context& context, const string& auxId) const {
    if (!context.hasObject(auxId)) {
		const string auxFileName = context.getJobOrder().getIpfConfiguration().getAuxFileName(auxId);
        shared_ptr<AuxdataProvider> auxdataProvider = shared_ptr<AuxdataProvider>(new AuxdataProvider(auxId, auxFileName));
        context.getLogging().info("Getting auxiliary data '" + auxId + ": " + auxFileName + "'", getId());
        context.addObject(auxdataProvider);
    }

    return (AuxdataProvider&) context.getObject(auxId);
}

LookupTable<double>& BasicModule::getLookupTable(Context& context, const string& auxId, const string& varName) const {
	const string contextId = auxId + "::" + varName;
	if (!context.hasObject(contextId)) {
		const string auxFileName = context.getJobOrder().getIpfConfiguration().getAuxFileName(auxId);
        const LookupTableReader reader(auxId, auxFileName);
        context.getLogging().info("Reading LUT '" + contextId + ": " + auxFileName + "'", getId());
        context.addObject(reader.readLookupTable<double>(varName));
    }
    return (LookupTable<double>&) context.getObject(contextId);
}
