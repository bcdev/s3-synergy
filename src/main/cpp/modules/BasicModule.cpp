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
	// TODO - use mapping from configuration
	map<string, string> idMap;
	idMap["SYCPAX"] = "files/S3__SY_2_SYCPAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	idMap["SYRTAX"] = "files/S3__SY_2_SYRTAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	idMap["VPCPAX"] = "files/S3__SY_2_VPCPAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	idMap["VPRTAX"] = "files/S3__SY_2_VPRTAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	idMap["VPSRAX"] = "files/S3__SY_2_VPSRAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	idMap["VSCPAX"] = "files/S3__SY_2_VSCPAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	idMap["VSRTAX"] = "files/S3__SY_2_VPRTAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";

    if (!context.hasObject(id)) {
    	// TODO - resolve relative paths properly
        shared_ptr<AuxdataProvider> auxdataProvider = shared_ptr<AuxdataProvider>(new AuxdataProvider(id, Constants::S3_SYNERGY_HOME + idMap.at(id)));
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
