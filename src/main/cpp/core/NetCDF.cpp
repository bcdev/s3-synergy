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
#include <stdexcept>
#include <sstream>

#include "NetCDF.h"

using std::runtime_error;

NetCDF::NetCDF() {
}

NetCDF::~NetCDF() {
}

int NetCDF::openFileReadOnly(const string& path) {
	int ncId;
	const int status = nc_open(path.c_str(), NC_NOWRITE, &ncId);
	checkStatus(status, "opening file '" + path + "'");
	return ncId;
}

int NetCDF::openFileWritable(const string& path) {
	int ncId;
	const int status = nc_open(path.c_str(), NC_WRITE, &ncId);
	checkStatus(status, "opening file '" + path + "'");
	return ncId;
}

void NetCDF::terminateFile(int ncId) {
	const int status = nc_enddef(ncId);
	checkStatus(status, "terminating file");
}

void NetCDF::closeFile(int ncId) {
	const int status = nc_close(ncId);
	checkStatus(status, "closing file");
}

Attribute NetCDF::getAttribute(int fileId, int varId, const string& attrName) {
	const int attrType = NetCDF::getAttributeType(fileId, varId, attrName);

	switch (attrType) {
	case Constants::TYPE_BYTE:
		return Attribute(attrType, attrName, NetCDF::getAttributeData<int8_t>(fileId, varId, attrName));
	case Constants::TYPE_SHORT:
		return Attribute(attrType, attrName, NetCDF::getAttributeData<int16_t>(fileId, varId, attrName));
	case Constants::TYPE_INT:
		return Attribute(attrType, attrName, NetCDF::getAttributeData<int32_t>(fileId, varId, attrName));
	case Constants::TYPE_LONG:
		return Attribute(attrType, attrName, NetCDF::getAttributeData<int64_t>(fileId, varId, attrName));
	case Constants::TYPE_UBYTE:
		return Attribute(attrType, attrName, NetCDF::getAttributeData<uint8_t>(fileId, varId, attrName));
	case Constants::TYPE_USHORT:
		return Attribute(attrType, attrName, NetCDF::getAttributeData<uint16_t>(fileId, varId, attrName));
	case Constants::TYPE_UINT:
		return Attribute(attrType, attrName, NetCDF::getAttributeData<uint32_t>(fileId, varId, attrName));
	case Constants::TYPE_ULONG:
		return Attribute(attrType, attrName, NetCDF::getAttributeData<uint64_t>(fileId, varId, attrName));
	case Constants::TYPE_FLOAT:
		return Attribute(attrType, attrName, NetCDF::getAttributeData<float>(fileId, varId, attrName));
	case Constants::TYPE_DOUBLE:
		return Attribute(attrType, attrName, NetCDF::getAttributeData<double>(fileId, varId, attrName));
	case Constants::TYPE_CHAR:
		return Attribute(attrType, attrName, NetCDF::getAttributeValue(fileId, varId, attrName));
	case Constants::TYPE_STRING:
		return Attribute(attrType, attrName, NetCDF::getAttributeValues(fileId, varId, attrName));
	default:
		BOOST_THROW_EXCEPTION(runtime_error("Unsupported attribute type."));
		break;
	}
}

string NetCDF::getAttributeValue(int fileId, int varId, const string& attrName) {
	const size_t attrLength = NetCDF::getAttributeLength(fileId, varId, attrName);
	valarray<char> attrData(attrLength + 1);

	const int status = nc_get_att_text(fileId, varId, attrName.c_str(), &attrData[0]);
	checkStatus(status, "getting attribute string");
	return string(&attrData[0]);

}

valarray<string> NetCDF::getAttributeValues(int fileId, int varId, const string& attrName) {
	const size_t attrLength = NetCDF::getAttributeLength(fileId, varId, attrName);
	valarray<char*> attrData(attrLength);

	const int status = nc_get_att_string(fileId, varId, attrName.c_str(), &attrData[0]);
	checkStatus(status, "getting attribute strings");

	valarray<string> strings(attrLength);
	for (size_t i = 0; i < attrLength; i++) {
		strings[i].assign(attrData[i]);
	}
	return strings;

}

size_t NetCDF::getAttributeCount(int fileId, int varId) {
	int attrCount;
	const int status = nc_inq_varnatts(fileId, varId, &attrCount);
	checkStatus(status, "getting attribute count");
	return attrCount;
}

size_t NetCDF::getAttributeLength(int fileId, int varId, const string& attrName) {
	size_t attrLength;
	const int status = nc_inq_attlen(fileId, varId, attrName.c_str(), &attrLength);
	checkStatus(status, "getting attribute length");
	return attrLength;
}

string NetCDF::getAttributeName(int fileId, int varId, int attrId) {
	valarray<char> attrName(NC_MAX_NAME);
	const int status = nc_inq_attname(fileId, varId, attrId, &attrName[0]);
	checkStatus(status, "getting attribute name");
	return string(&attrName[0]);
}

int NetCDF::getAttributeType(int fileId, int varId, const string& attributeName) {
	int type;
	const int status = nc_inq_atttype(fileId, varId, attributeName.c_str(), &type);
	checkStatus(status, "getting attribute type");
	return type;
}

double NetCDF::getAttributeValueDouble(int fileId, int varId, const string& attributeName, double defaultValue) {
	double value = defaultValue;
	const int status = nc_get_att_double(fileId, varId, attributeName.c_str(), &value);
	if (status != NC_ENOTATT) {
		checkStatus(status, "getting attribute value");
	}
	return value;
}

float NetCDF::getAttributeValueFloat(int fileId, int varId, const string& attributeName, float defaultValue) {
	float value = defaultValue;
	const int status = nc_get_att_float(fileId, varId, attributeName.c_str(), &value);
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
	if (status == NC_ENOTVAR) {
	    return -1;
	}
	checkStatus(status, "getting variable ID for variable '" + varName + "'");
	return varId;
}

size_t NetCDF::getDimensionCount(int fileId, int varId) {
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
	char dimName[NC_MAX_NAME + 1];
	const int status = nc_inq_dimname(fileId, dimId, dimName);
	checkStatus(status, "getting dimension name");
	return string(dimName);
}

void NetCDF::getVariableData(int fileId, int varId, const valarray<size_t>& origin, const valarray<size_t>& shape, void* dataArray) {
	const valarray<ptrdiff_t> strides(1, origin.size());
	const int status = nc_get_vars(fileId, varId, &origin[0], &shape[0], &strides[0], dataArray);
	checkStatus(status, "reading data from file");
}

void NetCDF::getVariableData(int fileId, int varId, const valarray<size_t>& origin, const valarray<size_t>& shape, float* dataArray) {
	const valarray<ptrdiff_t> strides(1, origin.size());
	const int status = nc_get_vars_float(fileId, varId, &origin[0], &shape[0], &strides[0], dataArray);
	checkStatus(status, "reading data from file");
}

void NetCDF::getVariableData(int fileId, int varId, const valarray<size_t>& origin, const valarray<size_t>& shape, double* dataArray) {
	const valarray<ptrdiff_t> strides(1, origin.size());
	const int status = nc_get_vars_double(fileId, varId, &origin[0], &shape[0], &strides[0], dataArray);
	checkStatus(status, "reading data from file");
}

int NetCDF::findDimension(int fileId, const string& dimName) {
    int dimId;
    const int status = nc_inq_dimid(fileId, dimName.c_str(), &dimId);
    if (status == NC_EBADDIM) {
        return -1;
    }
    checkStatus(status, "finding dimension '" + dimName + "'");
    return dimId;
}

void NetCDF::putValue(int fileId, int varId, const valarray<size_t>& indices, const void* value) {
	const int status = nc_put_var1(fileId, varId, &indices[0], value);
	checkStatus(status, "putting data into file");
}

void NetCDF::putData(int fileId, int varId, const void* dataArray) {
	const int status = nc_put_var(fileId, varId, dataArray);
	checkStatus(status, "putting data into file");
}

void NetCDF::putData(int fileId, int varId, const valarray<size_t>& origin, const valarray<size_t>& shape, const void* dataArray) {
	const valarray<ptrdiff_t> strides(1, origin.size());
	const int status = nc_put_vars(fileId, varId, &origin[0], &shape[0], &strides[0], dataArray);
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

int NetCDF::defineVariable(int fileId, const string& varName, int type, const valarray<int>& dimIds, bool compress) {
	int varId;
	int status = nc_def_var(fileId, varName.c_str(), type, dimIds.size(), &dimIds[0], &varId);
	checkStatus(status, "defining variable");
	if (compress) {
		status = nc_def_var_deflate(fileId, varId, 1, 1, 7);
		checkStatus(status, "defining compression parameters");
	}
	return varId;
}

void NetCDF::putGlobalAttribute(int fileId, const Attribute& attribute) {
    putAttribute(fileId, NC_GLOBAL, attribute);
}

void NetCDF::putAttribute(int fileId, int varId, const Attribute& attribute) {
	switch (attribute.getType()) {
	case Constants::TYPE_BYTE:
		putAttribute(fileId, varId, attribute, attribute.getBytes());
		break;
	case Constants::TYPE_UBYTE:
		putAttribute(fileId, varId, attribute, attribute.getUBytes());
		break;
	case Constants::TYPE_SHORT:
		putAttribute(fileId, varId, attribute, attribute.getShorts());
		break;
	case Constants::TYPE_USHORT:
		putAttribute(fileId, varId, attribute, attribute.getUShorts());
		break;
	case Constants::TYPE_INT:
		putAttribute(fileId, varId, attribute, attribute.getInts());
		break;
	case Constants::TYPE_UINT:
		putAttribute(fileId, varId, attribute, attribute.getUInts());
		break;
	case Constants::TYPE_LONG:
		putAttribute(fileId, varId, attribute, attribute.getLongs());
		break;
	case Constants::TYPE_ULONG:
		putAttribute(fileId, varId, attribute, attribute.getULongs());
		break;
	case Constants::TYPE_FLOAT:
		putAttribute(fileId, varId, attribute, attribute.getFloats());
		break;
	case Constants::TYPE_DOUBLE:
		putAttribute(fileId, varId, attribute, attribute.getDoubles());
		break;
	case Constants::TYPE_CHAR:
		putAttributeString(fileId, varId, attribute);
		break;
	case Constants::TYPE_STRING:
		putAttributeStrings(fileId, varId, attribute);
		break;
	default:
		BOOST_THROW_EXCEPTION(runtime_error("Unsupported attribute type."));
		break;
	}
}

template<class T>
void NetCDF::putAttribute(int fileId, int varId, const Attribute& attribute, const T& t) {
	const int status = nc_put_att(fileId, varId, attribute.getName().c_str(), attribute.getType(), t.size(), &t[0]);
	checkStatus(status, "putting attribute '" + attribute.getName() + "'");
}

void NetCDF::putAttributeString(int fileId, int varId, const Attribute& attribute) {
	const string& value = attribute.getValue();
	int status = nc_put_att_text(fileId, varId, attribute.getName().c_str(), value.size(), value.c_str());
	checkStatus(status, "putting attribute '" + attribute.getName() + "'");
}

void NetCDF::putAttributeStrings(int fileId, int varId, const Attribute& attribute) {
	const valarray<string> tokens = attribute.getTokens();
	valarray<const char*> op(tokens.size());
	for (size_t i = 0; i < tokens.size(); i++) {
		op[i] = tokens[i].c_str();
	}
	int status = nc_put_att_string(fileId, varId, attribute.getName().c_str(), op.size(), &op[0]);
	checkStatus(status, "putting attribute '" + attribute.getName() + "'");
}

void NetCDF::defineDimensions(const int fileId, const string& name, const vector<Dimension*>& dimensions, const Grid& grid, valarray<int>& dimIds) {
    const long sizeK = grid.getSizeK();
    const long sizeL = grid.getMaxL() - grid.getMinL() + 1;
    const long sizeM = grid.getSizeM();
    const size_t dimCount = dimensions.size();

    dimIds.resize(dimCount, 0);
    switch (dimCount) {
    case 3:
        dimIds[0] = NetCDF::defineDimension(fileId, dimensions[0]->getName(), sizeK);
        dimIds[1] = NetCDF::defineDimension(fileId, dimensions[1]->getName(), sizeL);
        dimIds[2] = NetCDF::defineDimension(fileId, dimensions[2]->getName(), sizeM);
        break;
    case 2:
        dimIds[0] = NetCDF::defineDimension(fileId, dimensions[0]->getName(), sizeL);
        dimIds[1] = NetCDF::defineDimension(fileId, dimensions[1]->getName(), sizeM);
        break;
    case 1:
        dimIds[0] = NetCDF::defineDimension(fileId, dimensions[0]->getName(), sizeM);
        break;
    default:
        BOOST_THROW_EXCEPTION(logic_error("AbstractWriter::createNcVar(): invalid number of dimensions (" + name + ")"));
        break;
    }
}


void NetCDF::checkStatus(int status, const string& action) {
	using std::ostringstream;
	if (status != 0) {
		ostringstream oss;
		oss << "IO error " << status << " while " << action << " ";
		BOOST_THROW_EXCEPTION( boost::enable_error_info(runtime_error(oss.str())) << errinfo_nc_status(status));
	}
}
