/*
 * AuxdataProvider.cpp
 *
 *  Created on: 11.10.2011
 *      Author: thomasstorm
 */

#include "AuxdataProvider.h"
#include "NetCDF.h"
#include "../core/Boost.h"

AuxdataProvider::AuxdataProvider(string auxdataPath) {
    const string auxdataFile = auxdataPath + "S3__SY_2_SYCPAX.nc";
    fileId = NetCDF::openFile(auxdataFile);
}

AuxdataProvider::~AuxdataProvider() {
    NetCDF::closeFile(fileId);
}

float AuxdataProvider::getFloat(const string& varName) const {
    const int varId = NetCDF::getVariableId(fileId, varName);
    valarray<size_t> origin(1);
    origin[0] = 0;
    valarray<size_t> shape(1);
    shape[0] = 1;
    shared_array<float> data(new float[shape[0]]);
    NetCDF::getVariableData(fileId, varId, origin, shape, data.get());
    return data[0];
}

valarray<float> AuxdataProvider::getFloatArray(const string& varName) const {
    const int varId = NetCDF::getVariableId(fileId, varName);
    valarray<size_t> origin(1);
    origin[0] = 0;
    valarray<size_t> shape(1);
    valarray<int> dimensionIds = NetCDF::getDimensionIds(fileId, varId);
    shape[0] = NetCDF::getDimensionLength(fileId, dimensionIds[0]);
    shared_array<float> values(new float[shape[0]]);
    NetCDF::getVariableData(fileId, varId, origin, shape, values.get());
    valarray<float> result(shape[0]);
    for(size_t i = 0; i < shape[0]; i++) {
        result[i] = values[i];
    }
    return result;
}

valarray<int16_t> AuxdataProvider::getInt16TArray(const string& varName) const {
    const int varId = NetCDF::getVariableId(fileId, varName);
    valarray<size_t> origin(1);
    origin[0] = 0;
    valarray<size_t> shape(1);
    valarray<int> dimensionIds = NetCDF::getDimensionIds(fileId, varId);
    shape[0] = NetCDF::getDimensionLength(fileId, dimensionIds[0]);
    shared_array<int16_t> values(new int16_t[shape[0]]);
    NetCDF::getVariableData(fileId, varId, origin, shape, values.get());
    valarray<int16_t> result(shape[0]);
    for(size_t i = 0; i < shape[0]; i++) {
        result[i] = values[i];
    }
    return result;
}

