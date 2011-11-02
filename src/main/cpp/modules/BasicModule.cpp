/*
 * BasicModule.cpp
 *
 *  Created on: 19.10.2011
 *      Author: thomasstorm
 */

#include "BasicModule.h"

void BasicModule::addAccessor(Context& context, Segment& s, const VariableDescriptor& varDescriptor) const {
    context.getLogging().info("Adding accessor for " + varDescriptor.toString() + "to segment " + s.toString(), getId());
    s.addVariable(varDescriptor);
}

AuxdataProvider& BasicModule::getAuxdataProvider(Context& context, const string& id) const {
    if (!context.hasObject(id)) {
        shared_ptr<AuxdataProvider> auxdataProvider = shared_ptr<AuxdataProvider>(new AuxdataProvider(id, getAuxdataPath() + "S3__SY_2_" + id + ".nc"));
        context.getLogging().info("Preparing auxiliary data '" + id + "'", getId());
        context.addObject(auxdataProvider);
    }
    return (AuxdataProvider&) context.getObject(id);
}

LookupTable<double>& BasicModule::getLookupTable(Context& context, const string& fileName, const string& varName) const {
    if (!context.hasObject(varName)) {
        const LookupTableReader reader(getAuxdataPath() + fileName);
        context.getLogging().info("Reading LUT '" + varName + "'", getId());
        context.addObject(reader.readLookupTable<double>(varName));
    }
    return (LookupTable<double>&) context.getObject(varName);
}
