/*
 * NetcdfAccessor.cpp
 *
 *  Created on: Feb 1, 2012
 *      Author: ralf
 */

#include "NetcdfAccessor.h"
#include "NetCDF.h"

NetcdfAccessor::NetcdfAccessor(const VariableDescriptor& variableDescriptor, const Grid& grid, const string& directoryPath) : scaleFactor(variableDescriptor.getScaleFactor()), addOffset(variableDescriptor.getAddOffset()), type(variableDescriptor.getType()) {
	const path targetPath = path(directoryPath + "/" + variableDescriptor.getNcFileBasename() + ".nc");
	if (!boost::filesystem::exists(targetPath.parent_path())) {
		if (!boost::filesystem::create_directories(targetPath.parent_path())) {
			BOOST_THROW_EXCEPTION( runtime_error("Cannot create directory '" + targetPath.parent_path().string() + "'."));
		}
		fileId = NetCDF::createFile(targetPath.string());
	} else {
		fileId = NetCDF::openFileWritable(targetPath.string());
	}
	const string varName = variableDescriptor.getName();
	varId = NetCDF::getVariableId(fileId, variableDescriptor.getName());
	if(varId == -1) {
		valarray<int> dimIds;
		NetCDF::defineDimensions(fileId, varName, variableDescriptor.getDimensions(), grid, dimIds);
		varId = NetCDF::defineVariable(fileId, varName, type, dimIds, false);
		foreach(const Attribute* attribute, variableDescriptor.getAttributes()) {
			NetCDF::putAttribute(fileId, varId, *attribute);
		}
	}
}

NetcdfAccessor::~NetcdfAccessor() {
	NetCDF::closeFile(fileId);
}

int8_t NetcdfAccessor::getByte(size_t i) const throw (bad_cast, out_of_range) {

}

void setByte(size_t i, int8_t value) throw (bad_cast, out_of_range);

