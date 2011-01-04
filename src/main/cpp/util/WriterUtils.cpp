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
 * File:   WriterUtils.cpp
 * Author: thomass
 * 
 * Created on December 30, 2010, 1:51 PM
 */

#include <string>

#include "WriterUtils.h"

WriterUtils::WriterUtils() {
}

WriterUtils::~WriterUtils() {
}

//NcFile* WriterUtils::getNcFile(string fileName) {
//    if (ncFileMap[fileName] == 0) {
//        NcFile* dataFile = new NcFile(fileName.c_str(), NcFile::Replace, 0, 0, NcFile::Netcdf4);
//        ncFileMap[fileName] = dataFile;
//        return dataFile;
//    } else {
//        return ncFileMap[fileName];
//    }
//}
//
//NcVar* WriterUtils::getNcVar(NcFile* dataFile, Variable* var, Dictionary& dict) {
//    if (addedVariables.find(var) == addedVariables.end()) {
//        const NcDim** ncDims = createNcDims(dataFile, var->getDimensions());
//        const char* varId = dict.getNcVarName(var->getNcName()).c_str();
//        size_t dimCount = var->getDimensions().size();
//        NcType type = var->getType();
//        NcVar* ncVar = dataFile->add_var(varId, type, dimCount, ncDims);
//        addedVariables[var] = ncVar;
//        return ncVar;
//    } else {
//        return addedVariables[var];
//    }
//}