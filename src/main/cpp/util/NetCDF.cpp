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
 * File:   NetCDF.cpp
 * Author: thomass
 *
 * Created on January 13, 2011, 5:41 PM
 */

#include <netcdf.h>
#include <sstream>

#include "NetCDF.h"

NetCDF::NetCDF() {
}

NetCDF::~NetCDF() {
}

int NetCDF::openFile(const string& path) {
	int ncId;
	const int status = nc_open(path.c_str(), NC_NOWRITE, &ncId);
	checkStatus(status, "opening file '" + path + "'");
	return ncId;
}

int NetCDF::getAttributeType(int fileId, int varId,
		const string& attributeName) {
	int type;
	const int status = nc_inq_atttype(fileId, varId, attributeName.c_str(),
			&type);
	checkStatus(status, "getting attribute type");
	return type;
}

double NetCDF::getAttributeDouble(int fileId, int varId,
		const string& attributeName, double defaultValue) {
	double value = defaultValue;
	const int status = nc_get_att_double(fileId, varId, attributeName.c_str(),
			&value);
	if (status != NC_ENOTATT) {
		checkStatus(status, "getting attribute value");
	}
	return value;
}

float NetCDF::getAttributeFloat(int fileId, int varId,
		const string& attributeName, float defaultValue) {
	float value = defaultValue;
	const int status = nc_get_att_float(fileId, varId, attributeName.c_str(),
			&value);
	if (status != NC_ENOTATT) {
		checkStatus(status, "getting attribute value");
	}
	return value;
}

int NetCDF::getVariableType(int fileId, int varId) {
	int type;
	const int status = nc_inq_vartype(fileId, varId, &type);
	checkStatus(status, "getting variable type");
	return type;
}

int NetCDF::getVariableId(int fileId, const string& varName) {
	int varId;
	const int status = nc_inq_varid(fileId, varName.c_str(), &varId);
	checkStatus(status, "getting variable ID for variable '" + varName + "'");
	return varId;
}

int NetCDF::getDimensionCount(int fileId, int varId) {
	int dimCount;
	const int status = nc_inq_varndims(fileId, varId, &dimCount);
	checkStatus(status, "getting dimension count");
	return dimCount;
}

valarray<int> NetCDF::getDimensionIds(int fileId, int varId) {
	const int dimCount = getDimensionCount(fileId, varId);
	valarray<int> dimIds(dimCount);
	const int status = nc_inq_vardimid(fileId, varId, &dimIds[0]);
	checkStatus(status, "getting dimension IDs");
	return dimIds;
}

size_t NetCDF::getDimensionLength(int fileId, int dimId) {
	size_t dimLength;
	const int status = nc_inq_dimlen(fileId, dimId, &dimLength);
	checkStatus(status, "getting dimension length");
	return dimLength;
}

string NetCDF::getDimensionName(int fileId, int dimId) {
	char dimName[NC_MAX_NAME + 1];const int status = nc_inq_dimname(fileId, dimId, dimName);
	checkStatus(status, "getting dimension name");
	return string(dimName);
}

void NetCDF::getData(int fileId, int varId, const valarray<size_t>& origin,
		const valarray<size_t>& shape, void* dataArray) {
	const valarray<ptrdiff_t> strides(1, origin.size());
	const int status = nc_get_vars(fileId, varId, &origin[0], &shape[0],
			&strides[0], dataArray);
	checkStatus(status, "reading data from file");
}

void NetCDF::getData(int fileId, int varId, const valarray<size_t>& origin,
		const valarray<size_t>& shape, float* dataArray) {
	const valarray<ptrdiff_t> strides(1, origin.size());
	const int status = nc_get_vars_float(fileId, varId, &origin[0], &shape[0],
			&strides[0], dataArray);
	checkStatus(status, "reading data from file");
}

void NetCDF::getData(int fileId, int varId, const valarray<size_t>& origin,
		const valarray<size_t>& shape, double* dataArray) {
	const valarray<ptrdiff_t> strides(1, origin.size());
	const int status = nc_get_vars_double(fileId, varId, &origin[0], &shape[0],
			&strides[0], dataArray);
	checkStatus(status, "reading data from file");
}

void NetCDF::putData(int fileId, int varId, const valarray<size_t>& origin,
		const valarray<size_t>& shape, const void* dataArray) {
	const valarray<ptrdiff_t> strides(1, origin.size());
	const int status = nc_put_vars(fileId, varId, &origin[0], &shape[0],
			&strides[0], dataArray);
	checkStatus(status, "putting data into file");
}

int NetCDF::createFile(const string& path) {
	int ncId;
	const int status = nc_create(path.c_str(), NC_NETCDF4, &ncId);
	checkStatus(status, "creating file '" + path + "'");
	return ncId;
}

int NetCDF::defineDimension(int fileId, const string& dimName, size_t size) {
	int dimId;
	const int status = nc_def_dim(fileId, dimName.c_str(), size, &dimId);
	checkStatus(status, "defining dimension '" + dimName + "'");
	return dimId;
}

int NetCDF::defineVariable(int fileId, const string& varName, int type,
		const valarray<int>& dimIds) {
	int varId;
	const int status = nc_def_var(fileId, varName.c_str(), type, dimIds.size(),
			&dimIds[0], &varId);
	checkStatus(status, "defining variable");
	return varId;
}

void NetCDF::putAttribute(int fileId, int varId, const Attribute& attribute) {
	switch (attribute.getType()) {
	case NC_BYTE: {
		putAttribute(fileId, varId, attribute, attribute.getBytes());
		break;
	}
	case NC_UBYTE: {
		putAttribute(fileId, varId, attribute, attribute.getUBytes());
		break;
	}
	case NC_SHORT: {
		putAttribute(fileId, varId, attribute, attribute.getShorts());
		break;
	}
	case NC_USHORT: {
		putAttribute(fileId, varId, attribute, attribute.getUShorts());
		break;
	}
	case NC_INT: {
		putAttribute(fileId, varId, attribute, attribute.getInts());
		break;
	}
	case NC_UINT: {
		putAttribute(fileId, varId, attribute, attribute.getUInts());
		break;
	}
	case NC_INT64: {
		putAttribute(fileId, varId, attribute, attribute.getLongs());
		break;
	}
	case NC_UINT64: {
		putAttribute(fileId, varId, attribute, attribute.getULongs());
		break;
	}
	case NC_FLOAT: {
		putAttribute(fileId, varId, attribute, attribute.getFloats());
		break;
	}
	case NC_DOUBLE: {
		putAttribute(fileId, varId, attribute, attribute.getDoubles());
		break;
	}
	default: {
		putAttributeString(fileId, varId, attribute);
		break;
	}
	}
}

void NetCDF::terminateDefinition(int ncId) {
	const int status = nc_enddef(ncId);
	checkStatus(status, "terminating definition phase");
}

void NetCDF::closeFile(int ncId) {
	const int status = nc_close(ncId);
	checkStatus(status, "closing netCDF-file");
}

template<class T>
void NetCDF::putAttribute(int fileId, int varId, const Attribute& attribute,
		const T& t) {
	const int status = nc_put_att(fileId, varId, attribute.getName().c_str(),
			attribute.getType(), t.size(), &t[0]);
	checkStatus(status, "putting attribute '" + attribute.getName() + "'");
}

void NetCDF::putAttributeString(int fileId, int varId,
		const Attribute& attribute) {
	const string& value = attribute.getValue();
	int status = nc_put_att(fileId, varId, attribute.getName().c_str(), NC_CHAR,
			value.size(), value.c_str());
	checkStatus(status, "putting attribute '" + attribute.getName() + "'");
}

void NetCDF::checkStatus(int status, const string& action) {
	if (status != NC_NOERR) {
		std::stringstream message;
		message << "NetCDF error ";
		message << action;
		message << ". Code '" << status << "'.";
		BOOST_THROW_EXCEPTION(std::runtime_error(message.str()));
	}
}
