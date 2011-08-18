/*
 * LookupTableReader.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: ralf
 */

#include "NetCDF.h"
#include "LookupTableReader.h"

LookupTableReader::LookupTableReader() {
}

LookupTableReader::~LookupTableReader() {
}

LookupTable<float>* LookupTableReader::readFloat(const string& path,
		const string& varName) const {
	const int fileId = NetCDF::openFile(path);
	const int varId = NetCDF::getVariableId(fileId, varName);
	const valarray<int> dimIds = NetCDF::getDimensionIds(fileId, varId);

	valarray<string> dimensionNames(dimIds.size());
	valarray<size_t> origin(dimIds.size());
	valarray<size_t> shape(dimIds.size());
	size_t valueCount = 1;
	for (size_t i = 0; i < dimIds.size(); i++) {
		dimensionNames[i] = NetCDF::getDimensionName(fileId, dimIds[i]);
		shape[i] = NetCDF::getDimensionLength(fileId, dimIds[i]);
		valueCount *= shape[i];
	}
	valarray<float> values(valueCount);
	NetCDF::getData(fileId, varId, origin, shape, &values[0]);


	return 0;
}
