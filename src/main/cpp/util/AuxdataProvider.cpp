/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

#include "../core/Boost.h"
#include "../core/NetCDF.h"

#include "AuxdataProvider.h"

AuxdataProvider::AuxdataProvider(const string& id, string filePath) :
		id(id), fileId(NetCDF::openFileReadOnly(filePath)) {
}

AuxdataProvider::~AuxdataProvider() {
	NetCDF::closeFile(fileId);
}

const string& AuxdataProvider::getId() const {
	return id;
}

void AuxdataProvider::getUByte(const string& varName, uint8_t& data) const {
	const int varId = NetCDF::getVariableId(fileId, varName);
	const valarray<size_t> origin(1);
	const valarray<size_t> shape(1, 1);

	NetCDF::getVariableData(fileId, varId, origin, shape, &data);
}

void AuxdataProvider::getUShort(const string& varName, uint16_t& data) const {
	const int varId = NetCDF::getVariableId(fileId, varName);
	const valarray<size_t> origin(1);
	const valarray<size_t> shape(1, 1);

	NetCDF::getVariableData(fileId, varId, origin, shape, &data);
}

void AuxdataProvider::getDouble(const string& varName, double& data) const {
	const int varId = NetCDF::getVariableId(fileId, varName);
	const valarray<size_t> origin(1);
	const valarray<size_t> shape(1, 1);

	NetCDF::getVariableData(fileId, varId, origin, shape, &data);
}

void AuxdataProvider::getShort(const string& varName, int16_t& data) const {
	const int varId = NetCDF::getVariableId(fileId, varName);
	const valarray<size_t> origin(1);
	const valarray<size_t> shape(1, 1);

	NetCDF::getVariableData(fileId, varId, origin, shape, &data);
}

void AuxdataProvider::getVectorDouble(const string& varName, valarray<double>& data) const {
	const int varId = NetCDF::getVariableId(fileId, varName);
	const valarray<int> dimensionIds = NetCDF::getDimensionIds(fileId, varId);
	const size_t valueCount = NetCDF::getDimensionLength(fileId, dimensionIds[0]);
	const valarray<size_t> origin(1);
	const valarray<size_t> shape(valueCount, 1);

	data.resize(valueCount);
	NetCDF::getVariableData(fileId, varId, origin, shape, &data[0]);
}

void AuxdataProvider::getVectorShort(const string& varName, valarray<int16_t>& data) const {
	const int varId = NetCDF::getVariableId(fileId, varName);
	const valarray<int> dimensionIds = NetCDF::getDimensionIds(fileId, varId);
	const size_t valueCount = NetCDF::getDimensionLength(fileId, dimensionIds[0]);
	const valarray<size_t> origin(1);
	const valarray<size_t> shape(valueCount, 1);

	data.resize(valueCount);
	NetCDF::getVariableData(fileId, varId, origin, shape, &data[0]);
}

void AuxdataProvider::getMatrixDouble(const string& varName, matrix<double>& m) const {
	const int varId = NetCDF::getVariableId(fileId, varName);
	const valarray<size_t> origin(2);
	const valarray<int> dimensionIds = NetCDF::getDimensionIds(fileId, varId);

	valarray<size_t> shape(2);
	shape[0] = NetCDF::getDimensionLength(fileId, dimensionIds[0]);
	shape[1] = NetCDF::getDimensionLength(fileId, dimensionIds[1]);

	valarray<double> data(shape[0] * shape[1]);
	NetCDF::getVariableData(fileId, varId, origin, shape, &data[0]);

	m.resize(shape[0], shape[1]);
	for (size_t i = 0, k = 0; i < shape[0]; i++) {
		for (size_t j = 0; j < shape[1]; j++, k++) {
			m.insert_element(i, j, data[k]);
		}
	}
}
