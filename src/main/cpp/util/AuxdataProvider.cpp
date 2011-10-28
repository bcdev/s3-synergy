/*
 * AuxdataProvider.cpp
 *
 *  Created on: 11.10.2011
 *      Author: thomasstorm
 */

#include "../core/Boost.h"

#include "AuxdataProvider.h"
#include "NetCDF.h"

AuxdataProvider::AuxdataProvider(const string& id, string filePath) :
		id(id), fileId(NetCDF::openFile(filePath)) {
}

AuxdataProvider::~AuxdataProvider() {
	NetCDF::closeFile(fileId);
}

const string& AuxdataProvider::getId() const {
	return id;
}

uint8_t AuxdataProvider::getUByte(const string& varName) const {
	if (contains(ubytes, varName)) {
		return ubytes[varName];
	}
	const int varId = NetCDF::getVariableId(fileId, varName);
	const valarray<size_t> origin(1);
	const valarray<size_t> shape(1, 1);
	uint8_t data;
	NetCDF::getVariableData(fileId, varId, origin, shape, &data);
	return ubytes[varName] = data;
}

double AuxdataProvider::getDouble(const string& varName) const {
	if (contains(doubles, varName)) {
		return doubles[varName];
	}
	const int varId = NetCDF::getVariableId(fileId, varName);
	const valarray<size_t> origin(1);
	const valarray<size_t> shape(1, 1);
	double data;
	NetCDF::getVariableData(fileId, varId, origin, shape, &data);
	return doubles[varName] = data;
}

int16_t AuxdataProvider::getShort(const string& varName) const {
	if (contains(shorts, varName)) {
		return shorts[varName];
	}
	const int varId = NetCDF::getVariableId(fileId, varName);
	const valarray<size_t> origin(1);
	const valarray<size_t> shape(1, 1);
	int16_t data;
	NetCDF::getVariableData(fileId, varId, origin, shape, &data);
	return shorts[varName] = data;
}

const valarray<double>& AuxdataProvider::getVectorDouble(const string& varName) const {
	if (contains(doubleArrays, varName)) {
		return *doubleArrays[varName];
	}
	const int varId = NetCDF::getVariableId(fileId, varName);
	const valarray<int> dimensionIds = NetCDF::getDimensionIds(fileId, varId);
	const size_t valueCount = NetCDF::getDimensionLength(fileId, dimensionIds[0]);
	const valarray<size_t> origin(1);
	const valarray<size_t> shape(valueCount, 1);
	shared_ptr<valarray<double> > v = shared_ptr<valarray<double> >(new valarray<double>(valueCount));
	NetCDF::getVariableData(fileId, varId, origin, shape, &(*v)[0]);
	doubleArrays[varName] = v;
	return *v;
}

const valarray<int16_t>& AuxdataProvider::getVectorShort(const string& varName) const {
	if (contains(shortArrays, varName)) {
		return *shortArrays[varName];
	}
	const int varId = NetCDF::getVariableId(fileId, varName);
	const valarray<int> dimensionIds = NetCDF::getDimensionIds(fileId, varId);
	const size_t valueCount = NetCDF::getDimensionLength(fileId, dimensionIds[0]);
	const valarray<size_t> origin(1);
	const valarray<size_t> shape(valueCount, 1);
	shared_ptr<valarray<int16_t> > v = shared_ptr<valarray<int16_t> >(new valarray<int16_t>(valueCount));
	NetCDF::getVariableData(fileId, varId, origin, shape, &(*v)[0]);
	shortArrays[varName] = v;
	return *v;
}

const matrix<double>& AuxdataProvider::getMatrixDouble(const string& varName) const {
	if (contains(doubleMatrices, varName)) {
		return *doubleMatrices[varName];
	}
	const int varId = NetCDF::getVariableId(fileId, varName);
	const valarray<size_t> origin(2);
	const valarray<int> dimensionIds = NetCDF::getDimensionIds(fileId, varId);

	valarray<size_t> shape(2);
	shape[0] = NetCDF::getDimensionLength(fileId, dimensionIds[0]);
	shape[1] = NetCDF::getDimensionLength(fileId, dimensionIds[1]);

	valarray<double> data(shape[0] * shape[1]);
	NetCDF::getVariableData(fileId, varId, origin, shape, &data[0]);

	shared_ptr<matrix<double> > m = shared_ptr<matrix<double> >(new matrix<double>(shape[0], shape[1]));
	for (size_t i = 0, k = 0; i < shape[0]; i++) {
		for (size_t j = 0; j < shape[1]; j++, k++) {
			m->insert_element(i, j, data[k]);
		}
	}
	doubleMatrices[varName] = m;
	return *m;
}
