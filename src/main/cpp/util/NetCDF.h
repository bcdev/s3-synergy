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
#include <sstream>
#include <stdexcept>
#include <string>
#include <valarray>

#include "../core/Boost.h"
#include "../core/Dictionary.h"

using std::string;
using std::runtime_error;
using std::valarray;

/**
 * Convenience wrapper class for the C-NetCDF-API. Provides a range of useful
 * static methods.
 */
class NetCDF {
public:

    static int openFile(const path& filePath);

    static int getVariableType(int fileId, int varId);

    static int getVariableId(int fileId, const string& varName);

    static int getDimCountForVariable(int fileId, int varId);

    static valarray<int> getDimIdsForVariable(int fileId, int varId);

    static size_t getDimLength(int fileId, int dimId);

    static string getDimName(int fileId, int dimId);

    static void getData(int fileId, int varId, const valarray<size_t>& startVector,
            const valarray<size_t>& sizeVector, void* dataArray);

    static void putData(int fileId, int varId, const valarray<size_t>& startVector,
            const valarray<size_t>& sizeVector, const void* dataArray);

    static int createFile(const path& filePath);

    static int defineDimension(int fileId, const string& dimName, size_t size);

    static int defineVariable(int fileId, const string& varName, int type, const valarray<int>& dimIds);

    static void addAttribute(int fileId, int varId, const Attribute& attribute);

    static void setDefinitionPhaseFinished(int ncId);

    static void closeFile(int ncId);

private:

    template<class T>
    static void putAttribute(int fileId, int varId, const Attribute& attribute, const T& t);

    static void putAttributeString(int fileId, int varId, const Attribute& attribute);

    static void checkStatus(int status, const string& action);

};
#endif	/* NETCDF_H */

