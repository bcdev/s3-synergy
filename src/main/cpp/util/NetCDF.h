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
 * File:   NetCDF.h
 * Author: thomass
 *
 * Created on January 13, 2011, 9:23 AM
 */

#ifndef NETCDF_H
#define	NETCDF_H

#include <netcdf.h>
#include <boost/algorithm/string/split.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <valarray>

#include "../core/Dictionary.h"

using std::runtime_error;
using std::valarray;

class NetCDF {
public:

    static int openFile(const char* fileName) {
        int ncId;
        int status = nc_open(fileName, NC_NOWRITE, &ncId);
        checkStatus(status, "opening file");
        return ncId;
    }

    static nc_type getVariableType(int fileId, int varId) {
        nc_type type;
        int status = nc_inq_vartype(fileId, varId, &type);
        checkStatus(status, "getting variable type");
        return type;
    }

    static int getVariableId(int fileId, const char* name) {
        int varId;
        int status = nc_inq_varid(fileId, name, &varId);
        checkStatus(status, "getting variable id");
        return varId;
    }

    static int getDimCountForVariable(int fileId, int varId) {
        int dimCount;
        int status = nc_inq_varndims(fileId, varId, &dimCount);
        checkStatus(status, "getting dimension count");
        return dimCount;
    }

    static valarray<int> getDimIdsForVariable(int fileId, int varId) {
        int dimCount = getDimCountForVariable(fileId, varId);
        valarray<int> dimensionIds(dimCount);
        int status = nc_inq_vardimid(fileId, varId, &dimensionIds[0]);
        checkStatus(status, "getting dimension ids");
        return dimensionIds;
    }

    static size_t getDimLength(int fileId, int dimId) {
        size_t dimLength;
        int status = nc_inq_dimlen(fileId, dimId, &dimLength);
        checkStatus(status, "getting dimension length");
        return dimLength;
    }

    static void putData(int fileId, int varId, const size_t* startVector,
            const size_t* sizeVector, const void* dataArray) {
        int status = nc_put_vars(fileId, varId, startVector, sizeVector, 0, dataArray);
        checkStatus(status, "putting data into file");
    }

    static int createFile(const char* file) {
        int ncId;
        int status = nc_create(file, NC_NETCDF4, &ncId);
        checkStatus(status, "creating file");
        return ncId;
    }

    static int defineDimension(int fileId, const char* name, size_t size) {
        int dimId;
        int status = nc_def_dim(fileId, name, size, &dimId);
        checkStatus(status, "defining dimension");
        return dimId;
    }

    static int defineVariable(int fileId, const char* name, int type, int dimCount, const int* dimIds) {
        int varId;
        int status = nc_def_var(fileId, name, type, dimCount, &dimIds[0], &varId);
        checkStatus(status, "defining variable");
        return varId;
    }

    static void addAttribute(int fileId, int varId, Attribute attribute) {
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

    static void setDefinitionPhaseFinished(int ncId) {
        int status = nc_enddef(ncId);
        checkStatus(status, "setting definition phase to finished");
    }

    static void closeFile(int ncId) {
        int status = nc_close(ncId);
        checkStatus(status, "closing netCDF-file");
    }

private:

    template<class T>
    static void putAttribute(int fileId, int varId, Attribute attribute, T t) {
        T v = t;
        int status = nc_put_att(fileId, varId, attribute.getName().c_str(), attribute.getType(), 1, &v);
        checkStatus(status, "putting attribute");
    }

    static void putAttributeString(int fileId, int varId, Attribute attribute) {
        const string& value = attribute.getValue();
        int status = nc_put_att(fileId, varId, attribute.getName().c_str(), NC_CHAR, value.size(), value.c_str());
        checkStatus(status, "putting attribute");
    }

    static void checkStatus(int status, const char* action) {
        if (status != NC_NOERR) {
            std::stringstream message;
            message << "NetCDF-error ";
            message << action;
            message << ". Code '" << status << "'.";
            throw runtime_error(message.str());
        }
    }

};
#endif	/* NETCDF_H */

