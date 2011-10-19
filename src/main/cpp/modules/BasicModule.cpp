/*
 * BasicModule.cpp
 *
 *  Created on: 19.10.2011
 *      Author: thomasstorm
 */

#include "BasicModule.h"
#include "../util/LookupTableReader.h"

void BasicModule::addAccessor(Context& context, Segment& s, const VariableDescriptor& varDescriptor) const {
    context.getLogging().info("Adding accessor for " + varDescriptor.toString() + "to segment " + s.toString(), getId());
    s.addVariable(varDescriptor);
}

void BasicModule::addMatrixLookupTable(Context& context, const string& fileName, const string& varName) const {
    if (!context.hasObject(varName)) {
        const LookupTableReader reader(getAuxdataPath() + fileName);
        context.getLogging().info("Reading LUT '" + varName + "'", getId());
        context.addObject(reader.readMatrixLookupTable<double>(varName));
    }
}

void BasicModule::addVectorLookupTable(Context& context, const string& fileName, const string& varName) const {
    if (!context.hasObject(varName)) {
        const LookupTableReader reader(getAuxdataPath() + fileName);
        context.getLogging().info("Reading LUT '" + varName + "'", getId());
        context.addObject(reader.readVectorLookupTable<double>(varName));
    }
}

void BasicModule::addScalarLookupTable(Context& context, const string& fileName, const string& varName) const {
    if (!context.hasObject(varName)) {
        const LookupTableReader reader(getAuxdataPath() + fileName);
        context.getLogging().info("Reading LUT '" + varName + "'", getId());
        context.addObject(reader.readScalarLookupTable<double>(varName));
    }
}
