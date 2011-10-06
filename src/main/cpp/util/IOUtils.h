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
#include "../util/NetCDF.h"
#include "../core/Segment.h"

class IOUtils {
public:
	static valarray<size_t> createCountVector(size_t dimCount, size_t camCount, size_t lineCount, size_t colCount) {
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
		BOOST_THROW_EXCEPTION( std::invalid_argument("Wrong number of dimensions."));
	}

	static valarray<size_t> createStartVector(size_t dimCount, size_t startLine) {
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
		BOOST_THROW_EXCEPTION( std::invalid_argument("Wrong number of dimensions."));
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

	static string getEnvironment(const string& name) {
		const char* value = std::getenv(name.c_str());
		if (value != 0) {
			return string(value);
		}
		BOOST_THROW_EXCEPTION(runtime_error("The environment variable '" + name + "' has not been set."));
	}

	static string getEnvironment(const string& name, const string& defaultValue) {
		const char* value = std::getenv(name.c_str());
		if (value != 0) {
			return string(value);
		}
		return defaultValue;
	}

};

#endif	/* IOUTILS_H */
