/*
 * AuxdataProvider.cpp
 *
 *  Created on: 11.10.2011
 *      Author: thomasstorm
 */

#include "AuxdataProvider.h"
#include "NetCDF.h"
#include "../core/Boost.h"

AuxdataProvider::AuxdataProvider(const string& id, string auxdataPath) : id(id) {
    fileId = NetCDF::openFile(auxdataPath);
}

AuxdataProvider::~AuxdataProvider() {
    NetCDF::closeFile(fileId);
}

const string& AuxdataProvider::getId() const {
    return id;
}

uint8_t AuxdataProvider::getUByte(const string& varName) {
    if(contains(ubytes, varName)) {
        return ubytes[varName];
    }
    const int varId = NetCDF::getVariableId(fileId, varName);
    valarray<size_t> origin(1);
    origin[0] = 0;
    valarray<size_t> shape(1);
    shape[0] = 1;
    valarray<uint8_t> data(shape[0]);
    NetCDF::getVariableData(fileId, varId, origin, shape, &data[0]);
    ubytes[varName] = data[0];
    return ubytes[varName];
}


double AuxdataProvider::getDouble(const string& varName) {
    if(contains(doubles, varName)) {
        return doubles[varName];
    }
    const int varId = NetCDF::getVariableId(fileId, varName);
    valarray<size_t> origin(1);
    origin[0] = 0;
    valarray<size_t> shape(1);
    shape[0] = 1;
    valarray<double> data(shape[0]);
    NetCDF::getVariableData(fileId, varId, origin, shape, &data[0]);
    doubles[varName] = data[0];
    return data[0];
}

valarray<double>& AuxdataProvider::getDoubleArray(const string& varName) {
    if(contains(doubleArrays, varName)) {
        return doubleArrays[varName];
    }
    const int varId = NetCDF::getVariableId(fileId, varName);
    valarray<size_t> origin(1);
    origin[0] = 0;
    valarray<size_t> shape(1);
    valarray<int> dimensionIds = NetCDF::getDimensionIds(fileId, varId);
    shape[0] = NetCDF::getDimensionLength(fileId, dimensionIds[0]);
    valarray<double> values(shape[0]);
    NetCDF::getVariableData(fileId, varId, origin, shape, &values[0]);
    doubleArrays[varName] = values;
    return doubleArrays[varName];
}

matrix<double>& AuxdataProvider::getDoubleMatrix(const string& varName) {
    if(contains(doubleMatrices, varName)) {
        return doubleMatrices[varName];
    }
    const int varId = NetCDF::getVariableId(fileId, varName);
    valarray<size_t> origin(2);
    origin[0] = 0;
    origin[1] = 0;
    valarray<size_t> shape(2);
    valarray<int> dimensionIds = NetCDF::getDimensionIds(fileId, varId);
    shape[0] = NetCDF::getDimensionLength(fileId, dimensionIds[0]);
    shape[1] = NetCDF::getDimensionLength(fileId, dimensionIds[1]);
    valarray<double> values(shape[0] * shape[1]);
    NetCDF::getVariableData(fileId, varId, origin, shape, &values[0]);
    matrix<double> result(shape[0], shape[1]);
    for(size_t i = 0; i < shape[0]; i++) {
        for(size_t j = 0; j < shape[1]; j++) {
            double value = values[i * shape[1] + j];
            result.insert_element(i, j, value);
        }
    }
    doubleMatrices[varName] = result;
    return doubleMatrices[varName];
}

int16_t AuxdataProvider::getShort(const string& varName) {
    if(contains(shorts, varName)) {
        return shorts[varName];
    }
    const int varId = NetCDF::getVariableId(fileId, varName);
    valarray<size_t> origin(1);
    origin[0] = 0;
    valarray<size_t> shape(1);
    shape[0] = 1;
    valarray<int16_t> data(shape[0]);
    NetCDF::getVariableData(fileId, varId, origin, shape, &data[0]);
    shorts[varName] = data[0];
    return shorts[varName];
}

valarray<int16_t>& AuxdataProvider::getShortArray(const string& varName) {
    if(contains(shortArrays, varName)) {
        return shortArrays[varName];
    }
    const int varId = NetCDF::getVariableId(fileId, varName);
    valarray<size_t> origin(1);
    origin[0] = 0;
    valarray<size_t> shape(1);
    valarray<int> dimensionIds = NetCDF::getDimensionIds(fileId, varId);
    shape[0] = NetCDF::getDimensionLength(fileId, dimensionIds[0]);
    valarray<int16_t> values(shape[0]);
    NetCDF::getVariableData(fileId, varId, origin, shape, &values[0]);
    shortArrays[varName] = values;
    return shortArrays[varName];
}
