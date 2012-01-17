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

#include <string>
#include <valarray>

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

	~NetCDF();

	static void closeFile(int ncId);

	static int createFile(const string& path);

	static int defineDimension(int fileId, const string& dimName, size_t size);

	static int defineVariable(int fileId, const string& varName, int type,
			const valarray<int>& dimIds, bool compress = false);

	static Attribute getAttribute(int fileId, int varId,
			const string& attrName);

	static size_t getAttributeCount(int fileId, int varId);

	static size_t getAttributeLength(int fileId, int varId,
			const string& attrName);

	static string getAttributeName(int fileId, int varId, int attrId);

	static int getAttributeType(int fileId, int varId, const string& attrName);

	template<class T>
	static valarray<T> getAttributeData(int fileId, int varId,
			const string& attrName) {
		const size_t attrLength = getAttributeLength(fileId, varId, attrName);
		valarray<T> attrData(attrLength);

		const int status = nc_get_att(fileId, varId, attrName.c_str(),
				&attrData[0]);
		checkStatus(status, "getting attribute data");
		return attrData;
	}

	static string getAttributeValue(int fileId, int varId,
			const string& attrName);

	static valarray<string> getAttributeValues(int fileId, int varId,
			const string& attrName);

	static double getAttributeValueDouble(int fileId, int varId,
			const string& attrName, double defaultValue);

	static float getAttributeValueFloat(int fileId, int varId,
			const string& attrName, float defaultValue);

	static void getVariableData(int fileId, int varId,
			const valarray<size_t>& origin, const valarray<size_t>& shape,
			void* dataArray);

	static void getVariableData(int fileId, int varId,
			const valarray<size_t>& origin, const valarray<size_t>& shape,
			float* dataArray);

	static void getVariableData(int fileId, int varId,
			const valarray<size_t>& origin, const valarray<size_t>& shape,
			double* dataArray);

	static size_t getDimensionCount(int fileId, int varId);

	static valarray<int> getDimensionIds(int fileId, int varId);

	static size_t getDimensionLength(int fileId, int dimId);

	static string getDimensionName(int fileId, int dimId);

	static int getVariableId(int fileId, const string& varName);

	static int getVariableType(int fileId, int varId);

	static int openFile(const string& path);

	static void putValue(int fileId, int varId, const valarray<size_t>& indices, const void* value);

	static void putData(int fileId, int varId, const valarray<size_t>& origin,
			const valarray<size_t>& shape, const void* dataArray);

	static void putGlobalAttribute(int fileId, const Attribute& attribute);

	static void putAttribute(int fileId, int varId, const Attribute& attribute);

	static void terminateFile(int ncId);

private:
	NetCDF();

	template<class T>
	static void putAttribute(int fileId, int varId, const Attribute& attribute,
			const T& t);
	static void putAttributeString(int fileId, int varId,
			const Attribute& attribute);
	static void putAttributeStrings(int fileId, int varId,
			const Attribute& attribute);
	static void checkStatus(int status, const string& action);

};
#endif	/* NETCDF_H */

