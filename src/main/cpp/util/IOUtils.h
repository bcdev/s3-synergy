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
 * File:   ReaderUtils.h
 * Author: thomass
 *
 * Created on January 5, 2011, 10:05 AM
 */

#ifndef IOUTILS_H
#define	IOUTILS_H

#include <boost/filesystem.hpp>
#include <iostream>
#include <netcdf.h>
#include <vector>

#include "Variable.h"
#include "../core/Segment.h"

using boost::filesystem::is_directory;
using boost::filesystem::directory_iterator;
using boost::filesystem::path;
using std::vector;

class IOUtils {
public:

    static int* getNcDims(int ncId, vector<Dimension*> dims) {
        int* ncDims = new int[dims.size()];
        for (size_t i = 0; i < dims.size(); i++) {
            int dimId;
            int status = nc_inq_dimid(ncId, dims[i]->getName().c_str(), &dimId);
            if (status != NC_NOERR) {
                int currentDimId;
                nc_def_dim(ncId, dims[i]->getName().c_str(), dims[i]->getRange(), &currentDimId);
                ncDims[i] = currentDimId;
            } else {
                ncDims[i] = dimId;
            }
        }
        return ncDims;
    }

    static const size_t* createCountVector(size_t dimCount, size_t camCount, size_t lineCount, size_t colCount) {
        // TODO - replace by vector
        if (dimCount == 3) {
            size_t* count = new size_t[3];
            count[0] = camCount;
            count[1] = lineCount;
            count[2] = colCount;
            return count;
        } else if (dimCount == 2) {
            size_t* count = new size_t[2];
            count[0] = lineCount;
            count[1] = colCount;
            return count;
        } else if (dimCount == 1) {
            size_t* count = new size_t[1];
            count[0] = lineCount;
            return count;
        }
        throw std::invalid_argument("Wrong number of dimensions.");
    }

    static const size_t* createStartVector(size_t dimCount, size_t startLine) {
        if (dimCount == 3) {
            size_t* start = new size_t[dimCount];
            start[0] = 0;
            start[1] = startLine;
            start[2] = 0;
            return start;
        } else if (dimCount == 2) {
            size_t* start = new size_t[dimCount];
            start[0] = startLine;
            start[1] = 0;
            return start;
        } else if (dimCount == 1) {
            size_t* start = new size_t[dimCount];
            start[0] = startLine;
            return start;
        }
        throw std::invalid_argument("Wrong number of dimensions.");
    }

    static void readData(int ncId, int varId, string& symbolicName, Segment& segment,
            const size_t dimCount, const size_t startLine, const size_t count[], const size_t index = 0) {

        const size_t* startVector = createStartVector(dimCount, startLine);

        nc_type type;
        nc_inq_vartype(ncId, varId, &type);
        switch (type) {
            case NC_BYTE:
            {
                int8_t* buffer = &segment.getAccessor(symbolicName).getByteData()[index];
                nc_get_vara_schar(ncId, varId, startVector, count, buffer);
                break;
            }
            case NC_UBYTE:
            {
                uint8_t* buffer = &segment.getAccessor(symbolicName).getUByteData()[index];
                nc_get_vara_ubyte(ncId, varId, startVector, count, buffer);
                break;
            }
            case NC_SHORT:
            {

                int16_t* buffer = &segment.getAccessor(symbolicName).getShortData()[index];
                nc_get_vara_short(ncId, varId, startVector, count, buffer);
                break;
            }
            case NC_USHORT:
            {
                uint16_t* buffer = &segment.getAccessor(symbolicName).getUShortData()[index];
                valarray<size_t> myCount(count, 3);
                valarray<size_t> myStartVector(startVector, 3);
                myCount[0] = 1;
                for (size_t k = segment.getGrid().getStartK(); k < segment.getGrid().getStartK() + segment.getGrid().getSizeK(); k++) {
                    myStartVector[0] = k;
                    nc_get_vara_ushort(ncId, varId, &myStartVector[0], &myCount[0], buffer);
                    buffer += segment.getGrid().getStrideK();
                }
                break;
            }
            case NC_INT:
            {
                int32_t* buffer = &segment.getAccessor(symbolicName).getIntData()[index];
                nc_get_vara_int(ncId, varId, startVector, count, buffer);
                break;
            }
            case NC_UINT:
            {
                uint32_t* buffer = &segment.getAccessor(symbolicName).getUIntData()[index];
                nc_get_vara_uint(ncId, varId, startVector, count, buffer);
                break;
            }
            case NC_INT64:
            {
                int64_t* buffer = &segment.getAccessor(symbolicName).getLongData()[index];
                nc_get_vara_long(ncId, varId, startVector, count, buffer);
                break;
            }
            case NC_UINT64:
            {
                // there is no function nc_get_var_ulong, so we have to read the data
                // into a signed long buffer
                int64_t* buffer = &segment.getAccessor(symbolicName).getLongData()[index];
                nc_get_vara_long(ncId, varId, startVector, count, buffer);
                break;
            }
            case NC_FLOAT:
            {
                float* buffer = &segment.getAccessor(symbolicName).getFloatData()[index];
                nc_get_vara_float(ncId, varId, startVector, count, buffer);
                break;
            }
            case NC_DOUBLE:
            {
                double* buffer = &segment.getAccessor(symbolicName).getDoubleData()[index];
                nc_get_vara_double(ncId, varId, startVector, count, buffer);
                break;
            }
            default:
            {
                throw std::invalid_argument("Variable " + symbolicName + " has invalid type.");
            }
        }

        delete[] startVector;
    }

    /**
     * Writes variable values of type 'int8_t' to the netCDF-file.
     * @param ncId The id of the netCDF-file to write to.
     * @param varId The id of the variable to be written.
     * @param type The type of the variable to be written.
     * @param dimCount The number of the variable's dimensions.
     * @param camCount The number of cameras.
     * @param lineCount The number of lines.
     * @param colCount The number of columns.
     * @param segment The segment containing the data to be written.
     * @param startLine The line to start writing at.
     * @param endLine The line to end writing at.
     * @param accessor The segment's accessor to the data.
     */
    static void write(int ncId, int varId, nc_type type, size_t dimCount, size_t camCount,
            size_t lineCount, size_t colCount, Segment& segment, size_t startLine,
            size_t endLine, Accessor& accessor) {
        switch (type) {
            case(NC_BYTE):
            {
                writeByte(ncId, varId, dimCount, camCount, lineCount,
                        colCount, segment, startLine, endLine, accessor);
                break;
            }
            case(NC_UBYTE):
            {
                writeUByte(ncId, varId, dimCount, camCount, lineCount,
                        colCount, segment, startLine, endLine, accessor);
                break;
            }
            case NC_SHORT:
            {
                writeShort(ncId, varId, dimCount, camCount, lineCount,
                        colCount, segment, startLine, endLine, accessor);
                break;
            }
            case NC_USHORT:
            {
                writeUShort(ncId, varId, dimCount, camCount, lineCount,
                        colCount, segment, startLine, endLine, accessor);
                break;
            }
            case NC_INT:
            {
                writeInt(ncId, varId, dimCount, camCount, lineCount,
                        colCount, segment, startLine, endLine, accessor);
                break;
            }
            case NC_UINT:
            {
                writeUInt(ncId, varId, dimCount, camCount, lineCount,
                        colCount, segment, startLine, endLine, accessor);
                break;
            }
            case NC_INT64:
            {
                writeLong(ncId, varId, dimCount, camCount, lineCount,
                        colCount, segment, startLine, endLine, accessor);
                break;
            }
            case NC_UINT64:
            {
                writeULong(ncId, varId, dimCount, camCount, lineCount,
                        colCount, segment, startLine, endLine, accessor);
                break;
            }
            case NC_FLOAT:
            {
                writeFloat(ncId, varId, dimCount, camCount, lineCount,
                        colCount, segment, startLine, endLine, accessor);
                break;
            }
            case NC_DOUBLE:
            {
                writeDouble(ncId, varId, dimCount, camCount, lineCount,
                        colCount, segment, startLine, endLine, accessor);
                break;
            }
            default:
            {
                throw std::runtime_error("Trying to write illegal type.");
            }
        }
    }

    static void addVariableToSegment(const string symbolicName, nc_type type, Segment& segment) {
        switch (type) {
            case NC_BYTE:
            {
                segment.addVariableByte(symbolicName);
                break;
            }
            case NC_UBYTE:
            {
                segment.addVariableUByte(symbolicName);
                break;
            }
            case NC_SHORT:
            {
                segment.addVariableShort(symbolicName);
                break;
            }
            case NC_USHORT:
            {
                segment.addVariableUShort(symbolicName);
                break;
            }
            case NC_INT:
            {
                segment.addVariableInt(symbolicName);
                break;
            }
            case NC_UINT:
            {
                segment.addVariableUInt(symbolicName);
                break;
            }
            case NC_INT64:
            {
                segment.addVariableLong(symbolicName);
                break;
            }
            case NC_UINT64:
            {
                segment.addVariableULong(symbolicName);
                break;
            }
            case NC_FLOAT:
            {
                segment.addVariableFloat(symbolicName);
                break;
            }
            case NC_DOUBLE:
            {
                segment.addVariableDouble(symbolicName);
                break;
            }
            default:
            {
                throw std::invalid_argument("Variable " + symbolicName + " has invalid type.");
            }
        }
    }

    static vector<string> getFiles(string& directory) {
        vector<string> files;
        if (is_directory(directory)) {
            for (directory_iterator iter(directory); iter != directory_iterator(); ++iter) {
                path file = iter->path();
                if (!is_directory(file)) {
                    files.push_back(file.filename());
                }
            }
        }

        return files;
    }

private:

    /**
     * Writes values of type 'int8_t' to the netCDF-file.
     *
     * @param ncId The id of the netCDF-file to write to.
     * @param varId The id of the variable to be written.
     * @param dimCount The number of the variable's dimensions.
     * @param camCount The number of cameras.
     * @param lineCount The number of lines.
     * @param colCount The number of columns.
     * @param segment The segment containing the data to be written.
     * @param startLine The line to start writing at.
     * @param endLine The line to end writing at.
     * @param accessor The segment's accessor to the data.
     */
    static void writeByte(int ncId, int varId, size_t dimCount, size_t camCount,
            size_t lineCount, size_t colCount, Segment& segment, size_t startLine,
            size_t endLine, Accessor& accessor) {

        Grid& grid = segment.getGrid();
        size_t lines = endLine - startLine + 1;
        size_t valueCount = grid.getSizeK() * lines * grid.getSizeM();
        const size_t* startVector = IOUtils::createStartVector(dimCount, startLine);
        const size_t* countVector = IOUtils::createCountVector(dimCount, camCount, lineCount, colCount);
        int8_t* valuesTemp = new int8_t[valueCount];
        for (size_t k = 0; k < camCount; k++) {
            for (size_t l = startLine; l <= endLine; l++) {
                for (size_t m = grid.getStartM(); m < grid.getSizeM(); m++) {
                    size_t position = grid.getIndex(k, l, m);
                    valuesTemp[position] = accessor.getByte(position);
                }
            }
        }
        const int8_t* values = valuesTemp;

        nc_put_vara_schar(ncId, varId, startVector, countVector, values);
        delete[] valuesTemp;
    }

    /**
     * Writes values of type 'uint8_t' to the netCDF-file.
     *
     * @param ncId The id of the netCDF-file to write to.
     * @param varId The id of the variable to be written.
     * @param dimCount The number of the variable's dimensions.
     * @param camCount The number of cameras.
     * @param lineCount The number of lines.
     * @param colCount The number of columns.
     * @param segment The segment containing the data to be written.
     * @param startLine The line to start writing at.
     * @param endLine The line to end writing at.
     * @param accessor The segment's accessor to the data.
     */
    static void writeUByte(int ncId, int varId, size_t dimCount, size_t camCount,
            size_t lineCount, size_t colCount, Segment& segment, size_t startLine,
            size_t endLine, Accessor& accessor) {

        Grid& grid = segment.getGrid();
        size_t lines = endLine - startLine + 1;
        size_t valueCount = grid.getSizeK() * lines * grid.getSizeM();
        const size_t* startVector = IOUtils::createStartVector(dimCount, startLine);
        const size_t* countVector = IOUtils::createCountVector(dimCount, camCount, lineCount, colCount);
        uint8_t* valuesTemp = new uint8_t[valueCount];
        for (size_t k = 0; k < camCount; k++) {
            for (size_t l = startLine; l <= endLine; l++) {
                for (size_t m = grid.getStartM(); m < grid.getSizeM(); m++) {
                    size_t position = grid.getIndex(k, l, m);
                    valuesTemp[position] = accessor.getUByte(position);
                }
            }
        }
        const uint8_t* values = valuesTemp;

        nc_put_vara_ubyte(ncId, varId, startVector, countVector, values);
        delete[] valuesTemp;
    }

    /**
     * Writes values of type 'int16_t' to the netCDF-file.
     *
     * @param ncId The id of the netCDF-file to write to.
     * @param varId The id of the variable to be written.
     * @param dimCount The number of the variable's dimensions.
     * @param camCount The number of cameras.
     * @param lineCount The number of lines.
     * @param colCount The number of columns.
     * @param segment The segment containing the data to be written.
     * @param startLine The line to start writing at.
     * @param endLine The line to end writing at.
     * @param accessor The segment's accessor to the data.
     */
    static void writeShort(int ncId, int varId, size_t dimCount, size_t camCount,
            size_t lineCount, size_t colCount, Segment& segment, size_t startLine,
            size_t endLine, Accessor& accessor) {

        Grid& grid = segment.getGrid();
        size_t lines = endLine - startLine + 1;
        size_t valueCount = grid.getSizeK() * lines * grid.getSizeM();
        const size_t* startVector = IOUtils::createStartVector(dimCount, startLine);
        const size_t* countVector = IOUtils::createCountVector(dimCount, camCount, lineCount, colCount);
        int16_t* valuesTemp = new int16_t[valueCount];
        for (size_t k = 0; k < camCount; k++) {
            for (size_t l = startLine; l <= endLine; l++) {
                for (size_t m = grid.getStartM(); m < grid.getSizeM(); m++) {
                    size_t position = grid.getIndex(k, l, m);
                    valuesTemp[position] = accessor.getShort(position);
                }
            }
        }
        const int16_t* values = valuesTemp;

        nc_put_vara_short(ncId, varId, startVector, countVector, values);
        delete[] valuesTemp;
    }

    /**
     * Writes values of type 'uint16_t' to the netCDF-file.
     *
     * @param ncId The id of the netCDF-file to write to.
     * @param varId The id of the variable to be written.
     * @param dimCount The number of the variable's dimensions.
     * @param camCount The number of cameras.
     * @param lineCount The number of lines.
     * @param colCount The number of columns.
     * @param segment The segment containing the data to be written.
     * @param startLine The line to start writing at.
     * @param endLine The line to end writing at.
     * @param accessor The segment's accessor to the data.
     */
    static void writeUShort(int ncId, int varId, size_t dimCount, size_t camCount,
            size_t lineCount, size_t colCount, Segment& segment, size_t startLine,
            size_t endLine, Accessor& accessor) {

        Grid& grid = segment.getGrid();
        size_t lines = endLine - startLine + 1;
        size_t valueCount = grid.getSizeK() * lines * grid.getSizeM();
        const size_t* startVector = IOUtils::createStartVector(dimCount, startLine);
        const size_t* countVector = IOUtils::createCountVector(dimCount, camCount, lineCount, colCount);
        uint16_t* valuesTemp = new uint16_t[valueCount];
        for (size_t k = 0; k < camCount; k++) {
            for (size_t l = startLine; l <= endLine; l++) {
                for (size_t m = grid.getStartM(); m < grid.getSizeM(); m++) {
                    size_t position = grid.getIndex(k, l, m);
                    valuesTemp[position] = accessor.getUShort(position);
                }
            }
        }
        const uint16_t* values = valuesTemp;

        nc_put_vara_ushort(ncId, varId, startVector, countVector, values);
        delete[] valuesTemp;
    }

    /**
     * Writes values of type 'int32_t' to the netCDF-file.
     *
     * @param ncId The id of the netCDF-file to write to.
     * @param varId The id of the variable to be written.
     * @param dimCount The number of the variable's dimensions.
     * @param camCount The number of cameras.
     * @param lineCount The number of lines.
     * @param colCount The number of columns.
     * @param segment The segment containing the data to be written.
     * @param startLine The line to start writing at.
     * @param endLine The line to end writing at.
     * @param accessor The segment's accessor to the data.
     */
    static void writeInt(int ncId, int varId, size_t dimCount, size_t camCount,
            size_t lineCount, size_t colCount, Segment& segment, size_t startLine,
            size_t endLine, Accessor& accessor) {

        Grid& grid = segment.getGrid();
        size_t lines = endLine - startLine + 1;
        size_t valueCount = grid.getSizeK() * lines * grid.getSizeM();
        const size_t* startVector = IOUtils::createStartVector(dimCount, startLine);
        const size_t* countVector = IOUtils::createCountVector(dimCount, camCount, lineCount, colCount);
        int32_t* valuesTemp = new int32_t[valueCount];
        for (size_t k = 0; k < camCount; k++) {
            for (size_t l = startLine; l <= endLine; l++) {
                for (size_t m = grid.getStartM(); m < grid.getSizeM(); m++) {
                    size_t position = grid.getIndex(k, l, m);
                    valuesTemp[position] = accessor.getInt(position);
                }
            }
        }
        const int32_t* values = valuesTemp;

        nc_put_vara_int(ncId, varId, startVector, countVector, values);
        delete[] valuesTemp;
    }

    /**
     * Writes values of type 'uint32_t' to the netCDF-file.
     *
     * @param ncId The id of the netCDF-file to write to.
     * @param varId The id of the variable to be written.
     * @param dimCount The number of the variable's dimensions.
     * @param camCount The number of cameras.
     * @param lineCount The number of lines.
     * @param colCount The number of columns.
     * @param segment The segment containing the data to be written.
     * @param startLine The line to start writing at.
     * @param endLine The line to end writing at.
     * @param accessor The segment's accessor to the data.
     */
    static void writeUInt(int ncId, int varId, size_t dimCount, size_t camCount,
            size_t lineCount, size_t colCount, Segment& segment, size_t startLine,
            size_t endLine, Accessor& accessor) {

        Grid& grid = segment.getGrid();
        size_t lines = endLine - startLine + 1;
        size_t valueCount = grid.getSizeK() * lines * grid.getSizeM();
        const size_t* startVector = IOUtils::createStartVector(dimCount, startLine);
        const size_t* countVector = IOUtils::createCountVector(dimCount, camCount, lineCount, colCount);
        uint32_t* valuesTemp = new uint32_t[valueCount];
        for (size_t k = 0; k < camCount; k++) {
            for (size_t l = startLine; l <= endLine; l++) {
                for (size_t m = grid.getStartM(); m < grid.getSizeM(); m++) {
                    size_t position = grid.getIndex(k, l, m);
                    valuesTemp[position] = accessor.getUInt(position);
                }
            }
        }
        const uint32_t* values = valuesTemp;

        nc_put_vara_uint(ncId, varId, startVector, countVector, values);
        delete[] valuesTemp;
    }

    /**
     * Writes values of type 'int64_t' to the netCDF-file.
     *
     * @param ncId The id of the netCDF-file to write to.
     * @param varId The id of the variable to be written.
     * @param dimCount The number of the variable's dimensions.
     * @param camCount The number of cameras.
     * @param lineCount The number of lines.
     * @param colCount The number of columns.
     * @param segment The segment containing the data to be written.
     * @param startLine The line to start writing at.
     * @param endLine The line to end writing at.
     * @param accessor The segment's accessor to the data.
     */
    static void writeLong(int ncId, int varId, size_t dimCount, size_t camCount,
            size_t lineCount, size_t colCount, Segment& segment, size_t startLine,
            size_t endLine, Accessor& accessor) {

        Grid& grid = segment.getGrid();
        size_t lines = endLine - startLine + 1;
        size_t valueCount = grid.getSizeK() * lines * grid.getSizeM();
        const size_t* startVector = IOUtils::createStartVector(dimCount, startLine);
        const size_t* countVector = IOUtils::createCountVector(dimCount, camCount, lineCount, colCount);
        int64_t* valuesTemp = new int64_t[valueCount];
        for (size_t k = 0; k < camCount; k++) {
            for (size_t l = startLine; l <= endLine; l++) {
                for (size_t m = grid.getStartM(); m < grid.getSizeM(); m++) {
                    size_t position = grid.getIndex(k, l, m);
                    valuesTemp[position] = accessor.getLong(position);
                }
            }
        }
        const int64_t* values = valuesTemp;

        nc_put_vara_long(ncId, varId, startVector, countVector, values);
        delete[] valuesTemp;
    }

    /**
     * Writes values of type 'unsigned long' to the netCDF-file. Note that there
     * is no support for handling this type of the netCDF-C-API, so we have to
     * use signed long values.
     * @param ncId The id of the netCDF-file to write to.
     * @param varId The id of the variable to be written.
     * @param dimCount The number of the variable's dimensions.
     * @param camCount The number of cameras.
     * @param lineCount The number of lines.
     * @param colCount The number of columns.
     * @param segment The segment containing the data to be written.
     * @param startLine The line to start writing at.
     * @param endLine The line to end writing at.
     * @param accessor The segment's accessor to the data.
     */
    static void writeULong(int ncId, int varId, size_t dimCount, size_t camCount,
            size_t lineCount, size_t colCount, Segment& segment, size_t startLine,
            size_t endLine, Accessor& accessor) {

        Grid& grid = segment.getGrid();
        size_t lines = endLine - startLine + 1;
        size_t valueCount = grid.getSizeK() * lines * grid.getSizeM();
        const size_t* startVector = IOUtils::createStartVector(dimCount, startLine);
        const size_t* countVector = IOUtils::createCountVector(dimCount, camCount, lineCount, colCount);
        int64_t* valuesTemp = new int64_t[valueCount];
        for (size_t k = 0; k < camCount; k++) {
            for (size_t l = startLine; l <= endLine; l++) {
                for (size_t m = grid.getStartM(); m < grid.getSizeM(); m++) {
                    size_t position = grid.getIndex(k, l, m);
                    valuesTemp[position] = accessor.getUByte(position);
                }
            }
        }
        const int64_t* values = valuesTemp;

        nc_put_vara_long(ncId, varId, startVector, countVector, values);
        delete[] valuesTemp;
    }

    /**
     * Writes values of type 'float' to the netCDF-file.
     *
     * @param ncId The id of the netCDF-file to write to.
     * @param varId The id of the variable to be written.
     * @param dimCount The number of the variable's dimensions.
     * @param camCount The number of cameras.
     * @param lineCount The number of lines.
     * @param colCount The number of columns.
     * @param segment The segment containing the data to be written.
     * @param startLine The line to start writing at.
     * @param endLine The line to end writing at.
     * @param accessor The segment's accessor to the data.
     */
    static void writeFloat(int ncId, int varId, size_t dimCount, size_t camCount,
            size_t lineCount, size_t colCount, Segment& segment, size_t startLine,
            size_t endLine, Accessor& accessor) {

        Grid& grid = segment.getGrid();
        size_t lines = endLine - startLine + 1;
        size_t valueCount = grid.getSizeK() * lines * grid.getSizeM();
        const size_t* startVector = IOUtils::createStartVector(dimCount, startLine);
        const size_t* countVector = IOUtils::createCountVector(dimCount, camCount, lineCount, colCount);
        float* valuesTemp = new float[valueCount];
        for (size_t k = 0; k < camCount; k++) {
            for (size_t l = startLine; l <= endLine; l++) {
                for (size_t m = grid.getStartM(); m < grid.getSizeM(); m++) {
                    size_t position = grid.getIndex(k, l, m);
                    valuesTemp[position] = accessor.getFloat(position);
                }
            }
        }
        const float* values = valuesTemp;

        nc_put_vara_float(ncId, varId, startVector, countVector, values);
        delete[] valuesTemp;
    }

    /**
     * Writes values of type 'double' to the netCDF-file.
     *
     * @param ncId The id of the netCDF-file to write to.
     * @param varId The id of the variable to be written.
     * @param dimCount The number of the variable's dimensions.
     * @param camCount The number of cameras.
     * @param lineCount The number of lines.
     * @param colCount The number of columns.
     * @param segment The segment containing the data to be written.
     * @param startLine The line to start writing at.
     * @param endLine The line to end writing at.
     * @param accessor The segment's accessor to the data.
     */
    static void writeDouble(int ncId, int varId, size_t dimCount, size_t camCount,
            size_t lineCount, size_t colCount, Segment& segment, size_t startLine,
            size_t endLine, Accessor& accessor) {

        Grid& grid = segment.getGrid();
        size_t lines = endLine - startLine + 1;
        size_t valueCount = grid.getSizeK() * lines * grid.getSizeM();
        const size_t* startVector = IOUtils::createStartVector(dimCount, startLine);
        const size_t* countVector = IOUtils::createCountVector(dimCount, camCount, lineCount, colCount);
        double* valuesTemp = new double[valueCount];
        for (size_t k = 0; k < camCount; k++) {
            for (size_t l = startLine; l <= endLine; l++) {
                for (size_t m = grid.getStartM(); m < grid.getSizeM(); m++) {
                    size_t position = grid.getIndex(k, l, m);
                    valuesTemp[position] = accessor.getDouble(position);
                }
            }
        }
        const double* values = valuesTemp;

        nc_put_vara_double(ncId, varId, startVector, countVector, values);
        delete[] valuesTemp;
    }

};
#endif	/* IOUTILS_H */