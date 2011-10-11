/*
 * ConfigurationAuxdataProvider.cpp
 *
 *  Created on: 11.10.2011
 *      Author: thomasstorm
 */

#include "ConfigurationAuxdataProvider.h"
#include "NetCDF.h"
#include "../core/Boost.h"

ConfigurationAuxdataProvider::ConfigurationAuxdataProvider(string auxdataPath) : initialTau550(-1.0), initialNus(0), initialOmegas(0), AMINs(0) {
    const string auxdataFile = auxdataPath + "S3__SY_2_SYCPAX.nc";
    fileId = NetCDF::openFile(auxdataFile);
}

ConfigurationAuxdataProvider::~ConfigurationAuxdataProvider() {
    NetCDF::closeFile(fileId);
}

float ConfigurationAuxdataProvider::getInitialTau550() {
    if(initialTau550 != -1.0) {
        return initialTau550;
    }
    const int varId = NetCDF::getVariableId(fileId, "T550_ini");
    valarray<size_t> origin(1);
    origin[0] = 0;
    valarray<size_t> shape(1);
    shape[0] = 1;
    shared_array<float> data(new float[shape[0]]);
    NetCDF::getVariableData(fileId, varId, origin, shape, data.get());
    initialTau550 = data[0];
    return initialTau550;
}

valarray<float> ConfigurationAuxdataProvider::getInitialNus() {
    if(initialNus.size() != 0) {
        return initialNus;
    }
    const int varId = NetCDF::getVariableId(fileId, "v_ini");
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
    initialNus = result;
    return result;
}

valarray<float> ConfigurationAuxdataProvider::getInitialOmegas() {
    if(initialOmegas.size() != 0) {
        return initialOmegas;
    }
    const int varId = NetCDF::getVariableId(fileId, "w_ini");
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
    initialOmegas = result;
    return result;
}

valarray<int16_t> ConfigurationAuxdataProvider::getAMINs() {
    if(AMINs.size() != 0) {
        return AMINs;
    }
    const int varId = NetCDF::getVariableId(fileId, "AMIN");
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
    AMINs = result;
    return result;
}

valarray<int16_t> ConfigurationAuxdataProvider::getNdviIndices() {
    if(ndviIndices.size() != 0) {
        return ndviIndices;
    }
    const int varId = NetCDF::getVariableId(fileId, "NDV_channel");
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
    ndviIndices = result;
    return result;
}
