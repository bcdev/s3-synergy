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
 * File:   IOUtils.h
 * Author: thomass
 *
 * Created on January 5, 2011, 10:05 AM
 */

#ifndef IOUTILS_H
#define	IOUTILS_H

#include <cstdlib>
#include <iostream>
#include <netcdf.h>

#include "../core/Boost.h"
#include "../core/Dictionary.h"
#include "../core/NetCDF.h"
#include "../core/Segment.h"

class IOUtils {
public:
	static void createCountVector(size_t dimCount, size_t camCount, size_t lineCount, size_t colCount, valarray<size_t>& shape) {
	    shape.resize(dimCount);
		if (dimCount == 3) {
			shape[0] = camCount;
			shape[1] = lineCount;
			shape[2] = colCount;
		} else if (dimCount == 2) {
			shape[0] = lineCount;
			shape[1] = colCount;
		} else if (dimCount == 1) {
			shape[0] = colCount;
		} else {
		    BOOST_THROW_EXCEPTION( std::invalid_argument("Wrong number of dimensions."));
		}
	}

	static void createStartVector(size_t dimCount, size_t startLine, valarray<size_t>& origin) {
	    origin.resize(dimCount);
		if (dimCount == 3) {
			origin[0] = 0;
			origin[1] = startLine;
			origin[2] = 0;
		} else if (dimCount == 2) {
			origin[0] = startLine;
			origin[1] = 0;
		} else if (dimCount == 1) {
			origin[0] = 0;
		} else {
		    BOOST_THROW_EXCEPTION( std::invalid_argument("Wrong number of dimensions."));
		}
	}

	static valarray<size_t> getDimensionSizes(VariableDescriptor* varDesc) {
		vector<Dimension*> dimensions = varDesc->getDimensions();
		valarray<size_t> dimensionSizes(3);
		switch (dimensions.size()) {
		case 3:
			dimensionSizes[0] = dimensions[0]->getSize(); // sizeK
			dimensionSizes[1] = dimensions[1]->getSize(); // sizeL
			dimensionSizes[2] = dimensions[2]->getSize(); // sizeM
			break;
		case 2:
			dimensionSizes[0] = 1;
			dimensionSizes[1] = dimensions[0]->getSize();
			dimensionSizes[2] = dimensions[1]->getSize();
			break;
		case 1:
			dimensionSizes[0] = 1;
			dimensionSizes[1] = 1;
			dimensionSizes[2] = dimensions[0]->getSize();
			break;
		}
		return dimensionSizes;
	}

	static vector<string> getFileNames(const string& directory) {
	    vector<string> fileNames;
	    if (is_directory(directory)) {
	        for (directory_iterator iter(directory); iter != directory_iterator(); ++iter) {
	            if (!is_directory(iter->path())) {
	                fileNames.push_back(iter->path().filename().c_str());
	            }
	        }
	    }
	    return fileNames;
	}

	static vector<string> getDirectoryNames(const string& directory) {
	    vector<string> directoryNames;
	    if (is_directory(directory)) {
	        for (directory_iterator iter(directory); iter != directory_iterator(); ++iter) {
	            if (is_directory(iter->path())) {
	                directoryNames.push_back(iter->path().filename().c_str());
	            }
	        }
	    }
	    return directoryNames;
	}
};

#endif	/* IOUTILS_H */
