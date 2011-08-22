/*
 * LookupTableReader.h
 *
 *  Created on: Aug 18, 2011
 *      Author: ralf
 */

#ifndef LOOKUPTABLEREADER_H_
#define LOOKUPTABLEREADER_H_

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
	LookupTable<W>* readLookupTable(const string& variableName) const;

private:
	int fileId;
};

LookupTableReader::LookupTableReader(const string& path) {
	fileId = NetCDF::openFile(path);
}

LookupTableReader::~LookupTableReader() {
	NetCDF::closeFile(fileId);
}

template <class W>
LookupTable<W>* LookupTableReader::readLookupTable(const string& varName) const {
    using std::accumulate;
    using std::multiplies;
    
	const int varId = NetCDF::getVariableId(fileId, varName);
	const valarray<int> dimensionIds = NetCDF::getDimensionIds(fileId, varId);
	const size_t dimensionCount = dimensionIds.size();
    
	valarray<string> dimensionNames(dimensionCount);
	valarray<size_t> dimensionLengths(dimensionCount);
	vector<valarray<W> > dimensions(dimensionCount);
    
	for (size_t i = 0; i < dimensionCount; i++) {
		dimensionNames[i] = NetCDF::getDimensionName(fileId, dimensionIds[i]);
		dimensionLengths[i] = NetCDF::getDimensionLength(fileId,
                                                         dimensionIds[i]);
		dimensions.push_back(valarray<W>(dimensionLengths[i]));
	}
	for (size_t i = 0; i < dimensionCount; i++) {
		const int varId = NetCDF::getVariableId(fileId, dimensionNames[i]);
		NetCDF::getDataFloat(fileId, varId, valarray<size_t>(1),
                             valarray<size_t>(dimensions[i].size(), 1), &dimensions[i][0]);
	}
    
	const size_t valueCount = accumulate(&dimensionLengths[0],
                                         &dimensionLengths[dimensionCount], size_t(1), multiplies<size_t>());
	const int varType = NetCDF::getVariableType(fileId, varId);
    
	switch (varType) {
        case Constants::TYPE_BYTE: {
            valarray<int8_t> values(valueCount);
            NetCDF::getData(fileId, varId, valarray<size_t>(dimensionCount),
                            dimensionLengths, &values[0]);
            return LookupTable<W>::newLookupTable(varName, dimensions, values);
        }
        case Constants::TYPE_UBYTE: {
            valarray<uint8_t> values(valueCount);
            NetCDF::getData(fileId, varId, valarray<size_t>(dimensionCount),
                            dimensionLengths, &values[0]);
            return LookupTable<W>::newLookupTable(varName, dimensions, values);
        }
        case Constants::TYPE_SHORT: {
            valarray<int16_t> values(valueCount);
            NetCDF::getData(fileId, varId, valarray<size_t>(dimensionCount),
                            dimensionLengths, &values[0]);
            return LookupTable<W>::newLookupTable(varName, dimensions, values);
        }
        case Constants::TYPE_USHORT: {
            valarray<uint16_t> values(valueCount);
            NetCDF::getData(fileId, varId, valarray<size_t>(dimensionCount),
                            dimensionLengths, &values[0]);
            return LookupTable<W>::newLookupTable(varName, dimensions, values);
        }
        case Constants::TYPE_INT: {
            valarray<int32_t> values(valueCount);
            NetCDF::getData(fileId, varId, valarray<size_t>(dimensionCount),
                            dimensionLengths, &values[0]);
            return LookupTable<W>::newLookupTable(varName, dimensions, values);
        }
        case Constants::TYPE_UINT: {
            valarray<uint32_t> values(valueCount);
            NetCDF::getData(fileId, varId, valarray<size_t>(dimensionCount),
                            dimensionLengths, &values[0]);
            return LookupTable<W>::newLookupTable(varName, dimensions, values);
        }
        case Constants::TYPE_LONG: {
            valarray<int64_t> values(valueCount);
            NetCDF::getData(fileId, varId, valarray<size_t>(dimensionCount),
                            dimensionLengths, &values[0]);
            return LookupTable<W>::newLookupTable(varName, dimensions, values);
        }
        case Constants::TYPE_ULONG: {
            valarray<uint64_t> values(valueCount);
            NetCDF::getData(fileId, varId, valarray<size_t>(dimensionCount),
                            dimensionLengths, &values[0]);
            return LookupTable<W>::newLookupTable(varName, dimensions, values);
        }
        case Constants::TYPE_FLOAT: {
            valarray<float> values(valueCount);
            NetCDF::getData(fileId, varId, valarray<size_t>(dimensionCount),
                            dimensionLengths, &values[0]);
            return LookupTable<W>::newLookupTable(varName, dimensions, values);
        }
        case Constants::TYPE_DOUBLE: {
            valarray<double> values(valueCount);
            NetCDF::getData(fileId, varId, valarray<size_t>(dimensionCount),
                            dimensionLengths, &values[0]);
            return LookupTable<W>::newLookupTable(varName, dimensions, values);
        }
        default:
            return 0;
	}
}

#endif /* LOOKUPTABLEREADER_H_ */
