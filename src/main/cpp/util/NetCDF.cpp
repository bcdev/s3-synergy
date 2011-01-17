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

#include "NetCDF.h"

int NetCDF::openFile(const string& fileName) {
    int ncId;
    int status = nc_open(fileName.c_str(), NC_NOWRITE, &ncId);
    checkStatus(status, "opening file");
    return ncId;
}

nc_type NetCDF::getVariableType(int fileId, int varId) {
    nc_type type;
    int status = nc_inq_vartype(fileId, varId, &type);
    checkStatus(status, "getting variable type");
    return type;
}

int NetCDF::getVariableId(int fileId, const string& varName) {
    int varId;
    int status = nc_inq_varid(fileId, varName.c_str(), &varId);
    checkStatus(status, "getting variable id");
    return varId;
}

int NetCDF::getDimCountForVariable(int fileId, int varId) {
    int dimCount;
    int status = nc_inq_varndims(fileId, varId, &dimCount);
    checkStatus(status, "getting dimension count");
    return dimCount;
}

valarray<int> NetCDF::getDimIdsForVariable(int fileId, int varId) {
    int dimCount = getDimCountForVariable(fileId, varId);
    valarray<int> dimensionIds(dimCount);
    int status = nc_inq_vardimid(fileId, varId, &dimensionIds[0]);
    checkStatus(status, "getting dimension ids");
    return dimensionIds;
}

size_t NetCDF::getDimLength(int fileId, int dimId) {
    size_t dimLength;
    int status = nc_inq_dimlen(fileId, dimId, &dimLength);
    checkStatus(status, "getting dimension length");
    return dimLength;
}

void NetCDF::getData(int fileId, int varId, const valarray<size_t>& startVector,
        const valarray<size_t>& sizeVector, void* dataArray) {
    int status = nc_get_vars(fileId, varId, &startVector[0], &sizeVector[0], 0, dataArray);
    checkStatus(status, "putting data into file");
}

void NetCDF::putData(int fileId, int varId, const valarray<size_t>& startVector,
        const valarray<size_t>& sizeVector, const void* dataArray) {
    int status = nc_put_vars(fileId, varId, &startVector[0], &sizeVector[0], 0, dataArray);
    checkStatus(status, "putting data into file");
}

int NetCDF::createFile(const string& fileName) {
    int ncId;
    int status = nc_create(fileName.c_str(), NC_NETCDF4, &ncId);
    checkStatus(status, "creating file");
    return ncId;
}

int NetCDF::defineDimension(int fileId, const string& dimName, size_t size) {
    int dimId;
    int status = nc_def_dim(fileId, dimName.c_str(), size, &dimId);
    checkStatus(status, "defining dimension");
    return dimId;
}

int NetCDF::defineVariable(int fileId, const string& varName, int type, const valarray<int>& dimIds) {
    int varId;
    int status = nc_def_var(fileId, varName.c_str(), type, dimIds.size(), &dimIds[0], &varId);
    checkStatus(status, "defining variable");
    return varId;
}

void NetCDF::addAttribute(int fileId, int varId, const Attribute& attribute) {
    switch (attribute.getType()) {
        case NC_BYTE:
        {
            putAttribute(fileId, varId, attribute, attribute.getByte());
            break;
        }
        case NC_UBYTE:
        {
            putAttribute(fileId, varId, attribute, attribute.getUByte());
            break;
        }
        case NC_SHORT:
        {
            putAttribute(fileId, varId, attribute, attribute.getShort());
            break;
        }
        case NC_USHORT:
        {
            putAttribute(fileId, varId, attribute, attribute.getUShort());
            break;
        }
        case NC_INT:
        {
            putAttribute(fileId, varId, attribute, attribute.getInt());
            break;
        }
        case NC_UINT:
        {
            putAttribute(fileId, varId, attribute, attribute.getUInt());
            break;
        }
        case NC_INT64:
        {
            putAttribute(fileId, varId, attribute, attribute.getLong());
            break;
        }
        case NC_UINT64:
        {
            putAttribute(fileId, varId, attribute, attribute.getULong());
            break;
        }
        case NC_FLOAT:
        {
            putAttribute(fileId, varId, attribute, attribute.getFloat());
            break;
        }
        case NC_DOUBLE:
        {
            putAttribute(fileId, varId, attribute, attribute.getDouble());
            break;
        }
        default:
        {
            putAttributeString(fileId, varId, attribute);
        }
    }
}

void NetCDF::setDefinitionPhaseFinished(int ncId) {
    int status = nc_enddef(ncId);
    checkStatus(status, "setting definition phase to finished");
}

void NetCDF::closeFile(int ncId) {
    int status = nc_close(ncId);
    checkStatus(status, "closing netCDF-file");
}

template<class T>
void NetCDF::putAttribute(int fileId, int varId, const Attribute& attribute, const T& t) {
    const T v = t;
    int status = nc_put_att(fileId, varId, attribute.getName().c_str(), attribute.getType(), 1, &v);
    checkStatus(status, "putting attribute");
}

void NetCDF::putAttributeString(int fileId, int varId, const Attribute& attribute) {
    const string& value = attribute.getValue();
    int status = nc_put_att(fileId, varId, attribute.getName().c_str(), NC_CHAR, value.size(), value.c_str());
    checkStatus(status, "putting attribute");
}

void NetCDF::checkStatus(int status, const string& action) {
    if (status != NC_NOERR) {
        std::stringstream message;
        message << "NetCDF-error ";
        message << action;
        message << ". Code '" << status << "'.";
        throw runtime_error(message.str());
    }
}