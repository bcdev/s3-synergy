/*
 * LookupTableReader.h
 *
 *  Created on: Aug 18, 2011
 *      Author: ralf
 */

#ifndef LOOKUPTABLEREADER_H_
#define LOOKUPTABLEREADER_H_

#include <stdexcept>
#include <functional>
#include <numeric>

#include "../core/Constants.h"
#include "../core/LookupTable.h"

#include "NetCDF.h"

class LookupTableReader {
public:
	LookupTableReader(const string& path);
	~LookupTableReader();

	template<class W>
	shared_ptr<LookupTable<W> > readLookupTable(const string& variableName) const;

private:
	int fileId;
};

template<class W>
shared_ptr<LookupTable<W> > LookupTableReader::readLookupTable(const string& variableName) const {
    using std::accumulate;
    using std::multiplies;

    const int varId = NetCDF::getVariableId(fileId, variableName);
    const valarray<int> dimensionIds = NetCDF::getDimensionIds(fileId, varId);
    const size_t dimensionCount = dimensionIds.size();

    valarray<string> dimensionNames(dimensionCount);
    valarray<size_t> dimensionLengths(dimensionCount);
    vector<valarray<W> > dimensions;

    for (size_t i = 0; i < dimensionCount; i++) {
        dimensionNames[i] = NetCDF::getDimensionName(fileId, dimensionIds[i]);
        dimensionLengths[i] = NetCDF::getDimensionLength(fileId, dimensionIds[i]);
    }
    for (size_t i = 0; i < dimensionCount; i++) {
        dimensions.push_back(valarray<W>(dimensionLengths[i]));
        const int coordinateId = NetCDF::getVariableId(fileId, dimensionNames[i]);
        NetCDF::getVariableData(fileId, coordinateId, valarray<size_t>(1), valarray<size_t>(dimensions[i].size(), 1), &dimensions[i][0]);
    }

    const size_t valueCount = accumulate(&dimensionLengths[0], &dimensionLengths[dimensionCount], size_t(1), multiplies<size_t>());
    const W scaleFactor = W(NetCDF::getAttributeValueDouble(fileId, varId, "scale_factor", 1.0));
    const W addOffset = W(NetCDF::getAttributeValueDouble(fileId, varId, "add_offset", 0.0));
    const int varType = NetCDF::getVariableType(fileId, varId);

    switch (varType) {
    case Constants::TYPE_BYTE: {
        shared_array<int8_t> values(new int8_t[valueCount]);
        NetCDF::getVariableData(fileId, varId, valarray<size_t>(dimensionCount), dimensionLengths, values.get());
        return LookupTable<W>::newLookupTable(variableName, dimensions, values, scaleFactor, addOffset);
    }
    case Constants::TYPE_UBYTE: {
        shared_array<uint8_t> values(new uint8_t[valueCount]);
        NetCDF::getVariableData(fileId, varId, valarray<size_t>(dimensionCount), dimensionLengths, values.get());
        return LookupTable<W>::newLookupTable(variableName, dimensions, values, scaleFactor, addOffset);
    }
    case Constants::TYPE_SHORT: {
        shared_array<int16_t> values(new int16_t[valueCount]);
        NetCDF::getVariableData(fileId, varId, valarray<size_t>(dimensionCount), dimensionLengths, values.get());
        return LookupTable<W>::newLookupTable(variableName, dimensions, values, scaleFactor, addOffset);
    }
    case Constants::TYPE_USHORT: {
        shared_array<uint16_t> values(new uint16_t[valueCount]);
        NetCDF::getVariableData(fileId, varId, valarray<size_t>(dimensionCount), dimensionLengths, values.get());
        return LookupTable<W>::newLookupTable(variableName, dimensions, values, scaleFactor, addOffset);
    }
    case Constants::TYPE_INT: {
        shared_array<int32_t> values(new int32_t[valueCount]);
        NetCDF::getVariableData(fileId, varId, valarray<size_t>(dimensionCount), dimensionLengths, values.get());
        return LookupTable<W>::newLookupTable(variableName, dimensions, values, scaleFactor, addOffset);
    }
    case Constants::TYPE_UINT: {
        shared_array<uint32_t> values(new uint32_t[valueCount]);
        NetCDF::getVariableData(fileId, varId, valarray<size_t>(dimensionCount), dimensionLengths, values.get());
        return LookupTable<W>::newLookupTable(variableName, dimensions, values, scaleFactor, addOffset);
    }
    case Constants::TYPE_LONG: {
        shared_array<int64_t> values(new int64_t[valueCount]);
        NetCDF::getVariableData(fileId, varId, valarray<size_t>(dimensionCount), dimensionLengths, values.get());
        return LookupTable<W>::newLookupTable(variableName, dimensions, values, scaleFactor, addOffset);
    }
    case Constants::TYPE_ULONG: {
        shared_array<uint64_t> values(new uint64_t[valueCount]);
        NetCDF::getVariableData(fileId, varId, valarray<size_t>(dimensionCount), dimensionLengths, values.get());
        return LookupTable<W>::newLookupTable(variableName, dimensions, values, scaleFactor, addOffset);
    }
    case Constants::TYPE_FLOAT: {
        shared_array<float> values(new float[valueCount]);
        NetCDF::getVariableData(fileId, varId, valarray<size_t>(dimensionCount), dimensionLengths, values.get());
        return LookupTable<W>::newLookupTable(variableName, dimensions, values, scaleFactor, addOffset);
    }
    case Constants::TYPE_DOUBLE: {
        shared_array<double> values(new double[valueCount]);
        NetCDF::getVariableData(fileId, varId, valarray<size_t>(dimensionCount), dimensionLengths, values.get());
        return LookupTable<W>::newLookupTable(variableName, dimensions, values, scaleFactor, addOffset);
    }
    default:
        BOOST_THROW_EXCEPTION(std::runtime_error("Unsupported data type."));
    }
}

#endif /* LOOKUPTABLEREADER_H_ */
