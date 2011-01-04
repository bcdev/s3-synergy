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
 * File:   WriterUtils.h
 * Author: thomass
 *
 * Created on December 30, 2010, 1:51 PM
 */

#ifndef WRITERUTILS_H
#define	WRITERUTILS_H

#include <map>
#include <netcdf.h>
#include <vector>

#include "Dictionary.h"
#include "Variable.h"

using std::map;
using std::vector;

class WriterUtils {
public:
    WriterUtils();
    virtual ~WriterUtils();
//    NcFile* getNcFile(string fileName);
//    NcVar* getNcVar(NcFile* dataFile, Variable* var, Dictionary& dict);
//
//    static const NcDim** createNcDims(NcFile* dataFile, vector<Dimension*> dims) {
//        const NcDim** ncDims = new const NcDim* [dims.size()];
//        for (size_t i = 0; i < dims.size(); i++) {
//            dataFile->add_dim(dims[i]->getName().c_str(), dims[i]->getRange());
//            ncDims[i] = dataFile->get_dim(dims[i]->getName().c_str());
//        }
//        return ncDims;
//    }
//
//private:
//    map<string, NcFile*> ncFileMap;
//    map<Variable*, NcVar*> addedVariables;
};

#endif	/* WRITERUTILS_H */

