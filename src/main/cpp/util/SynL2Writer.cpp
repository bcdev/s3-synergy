/*
 * File:   Writer.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 3:40 PM
 */

#include <boost/algorithm/string/predicate.hpp>
#include <stdexcept>
#include <set>

#include "SynL2Writer.h"
#include "../core/Context.h"
#include "../util/Logger.h"

using std::invalid_argument;

SynL2Writer::SynL2Writer() : DefaultModule("SYN_L2_WRITER") {
}

SynL2Writer::~SynL2Writer() {

}

const string SynL2Writer::OLC_TOA_RADIANCE_MEAS_1 = "OLC_RADIANCE_O1_TOA_Radiance_Meas"; // to be deleted

void SynL2Writer::process(Context& context) {

    string segmentId = "SYN_COLLOCATED";
    Segment& segment = context.getSegment(segmentId);
    Logger::get()->progress("Starting to write segment [" + segment.toString() + "]", getId());

    Grid& grid = segment.getGrid();
    size_t startLine;
    if( context.getMaxLComputed(segment, *this) == 0) {
        // different for first processing step
        startLine = 0;
    } else {
        startLine = context.getMinLRequired(segment, context.getMaxLComputed(segment, *this) + 1);
    }
    size_t endLine = startLine + grid.getSizeL() - 1;
    size_t lines = endLine - startLine + 1;
    long cameras = grid.getSizeK() - grid.getStartK();
    int* values = new int[cameras * lines * grid.getSizeM()];

//    vector<string>::iterator iter = variablesToWrite.begin();
//    for (; iter != variablesToWrite.end(); iter++) {
//        Variable& variable = context.getDictionary()->getVariable(*iter);
//
//        string ncName = variable.getNcName();
//        string ncFileName = context.getDictionary()->getNcFileName(ncName);
//        NcFile* ncFile = getNcFile(ncFileName);
//        Accessor& accessor = context.getSegment(segmentId).getAccessor(ncName);
//        NcVar* ncVar = getNcVar(ncFile, &variable);
//
//        // switch for all data types:
//        // for each variable do:
//        // if variable.getType == "short" do:
//
//        for (size_t k = grid.getStartK(); k < grid.getSizeK(); k++) {
//            for (size_t l = startLine; l < endLine; l++) {
//                for (size_t m = grid.getStartM(); m < grid.getSizeM(); m++) {
//                    size_t position = segment.getGrid().getIndex(k, l, m);
//                    values[position] = accessor.getShort(position);
//                }
//            }
//        }
//
//        // else if variable.getType == "float" do:
//
//        // ... end for.
//
//        //    ncVar->put_rec(values);
//
//        ncFile->sync();
//    }
//    delete[] values;
    context.setMaxLComputed(segment, *this, endLine);
}

NcFile* SynL2Writer::getNcFile(string fileName) {
    if (ncFileMap[fileName] == 0) {
        NcFile* dataFile = new NcFile(fileName.c_str(), NcFile::Replace, 0, 0, NcFile::Netcdf4);
        ncFileMap[fileName] = dataFile;
        return dataFile;
    } else {
        return ncFileMap[fileName];
    }
}

NcVar* SynL2Writer::getNcVar(NcFile* dataFile, Variable* var) {
    if (addedVariables.find(var) == addedVariables.end()) {
        const NcDim** ncDims = createNcDims(dataFile, var->getDimensions());
        const char* varId = var->getNcName().c_str();
        size_t dimCount = var->getDimensions().size();
        NcType type = var->getType();
        NcVar* ncVar = dataFile->add_var(varId, type, dimCount, ncDims);
        addedVariables[var] = ncVar;
        return ncVar;
    } else {
        return addedVariables[var];
    }
}

const NcDim** SynL2Writer::createNcDims(NcFile* dataFile, vector<Dimension*> dims) {
    const NcDim** ncDims = new const NcDim* [dims.size()];
    for (size_t i = 0; i < dims.size(); i++) {
        dataFile->add_dim(dims[i]->getName().c_str(), dims[i]->getRange());
        ncDims[i] = dataFile->get_dim(dims[i]->getName().c_str());
    }
    return ncDims;
}

void SynL2Writer::start(Context& context) {
    variablesToWrite.push_back(OLC_TOA_RADIANCE_MEAS_1);
}