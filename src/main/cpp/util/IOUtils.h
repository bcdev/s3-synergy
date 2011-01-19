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

#include <iostream>
#include <netcdf.h>
#include <vector>

#include "../core/Boost.h"
#include "../core/Dictionary.h"
#include "../util/NetCDF.h"
#include "../core/Segment.h"

using std::vector;

class IOUtils {
public:

    static int* getNcDims(int fileId, vector<Dimension*> dims) {
        valarray<int> ncDims(dims.size());
        for (size_t i = 0; i < dims.size(); i++) {
            int dimId;
            int status = nc_inq_dimid(fileId, dims[i]->getName().c_str(), &dimId);
            if (status != NC_NOERR) {
                int currentDimId;
                nc_def_dim(fileId, dims[i]->getName().c_str(), dims[i]->getSize(), &currentDimId);
                ncDims[i] = currentDimId;
            } else {
                ncDims[i] = dimId;
            }
        }
        return &ncDims[0];
    }

    static const valarray<size_t> createCountVector(size_t dimCount, size_t camCount, size_t lineCount, size_t colCount) {
        valarray<size_t> count(dimCount);
        if (dimCount == 3) {
            count[0] = camCount;
            count[1] = lineCount;
            count[2] = colCount;
            return count;
        } else if (dimCount == 2) {
            count[0] = lineCount;
            count[1] = colCount;
            return count;
        } else if (dimCount == 1) {
            count[0] = colCount;
            return count;
        }
        BOOST_THROW_EXCEPTION(std::invalid_argument("Wrong number of dimensions."));
    }

    static const valarray<size_t> createStartVector(size_t dimCount, size_t startLine) {
        valarray<size_t> start(dimCount);
        if (dimCount == 3) {
            start[0] = 0;
            start[1] = startLine;
            start[2] = 0;
            return start;
        } else if (dimCount == 2) {
            start[0] = startLine;
            start[1] = 0;
            return start;
        } else if (dimCount == 1) {
            start[0] = 0;
            return start;
        }
        BOOST_THROW_EXCEPTION(std::invalid_argument("Wrong number of dimensions."));
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
                BOOST_THROW_EXCEPTION(std::invalid_argument("Variable " + symbolicName + " has invalid type."));
            }
        }
    }

    static vector<string> getFiles(const string& directory) {
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
};
#endif	/* IOUTILS_H */