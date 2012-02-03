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

AuxdataProvider& BasicModule::getAuxdataProvider(Context& context, const string& auxId) const {
	// TODO - use mapping from configuration
	map<string, string> auxIdMap;
	auxIdMap["SYCPAX"] = "files/S3__SY_2_SYCPAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	auxIdMap["SYRTAX"] = "files/S3__SY_2_SYRTAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	auxIdMap["VPCPAX"] = "files/S3__SY_2_VPCPAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	auxIdMap["VPRTAX"] = "files/S3__SY_2_VPRTAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	auxIdMap["VPSRAX"] = "files/S3__SY_2_VPSRAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	auxIdMap["VSCPAX"] = "files/S3__SY_2_VSCPAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	auxIdMap["VSRTAX"] = "files/S3__SY_2_VSRTAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";

    if (!context.hasObject(auxId)) {
    	// TODO - resolve relative paths properly
        shared_ptr<AuxdataProvider> auxdataProvider = shared_ptr<AuxdataProvider>(new AuxdataProvider(auxId, Constants::S3_SYNERGY_HOME + "/" + auxIdMap.at(auxId)));
        context.getLogging().info("Preparing auxiliary data '" + auxId + "'", getId());
        context.addObject(auxdataProvider);
    }

    return (AuxdataProvider&) context.getObject(auxId);
}

LookupTable<double>& BasicModule::getLookupTable(Context& context, const string& auxId, const string& varName) const {
	// TODO - use mapping from configuration
	map<string, string> auxIdMap;
	auxIdMap["SYCPAX"] = "files/S3__SY_2_SYCPAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	auxIdMap["SYRTAX"] = "files/S3__SY_2_SYRTAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	auxIdMap["VPCPAX"] = "files/S3__SY_2_VPCPAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	auxIdMap["VPRTAX"] = "files/S3__SY_2_VPRTAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	auxIdMap["VPSRAX"] = "files/S3__SY_2_VPSRAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	auxIdMap["VSCPAX"] = "files/S3__SY_2_VSCPAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";
	auxIdMap["VSRTAX"] = "files/S3__SY_2_VSRTAX_20120101T000000_20140101T000000_20120101T000000__BC__D_NT_AUX_00.nc";

	const string contextId = auxId + "::" + varName;
	if (!context.hasObject(contextId)) {
    	// TODO - resolve relative paths properly
        const LookupTableReader reader(auxId, Constants::S3_SYNERGY_HOME + "/" + auxIdMap.at(auxId));
        context.getLogging().info("Reading LUT '" + contextId + "'", getId());
        context.addObject(reader.readLookupTable<double>(varName));
    }
    return (LookupTable<double>&) context.getObject(contextId);
}
