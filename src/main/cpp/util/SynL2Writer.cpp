/*
 * File:   Writer.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 3:40 PM
 */

#include <boost/algorithm/string/predicate.hpp>
#include <stdexcept>

#include "SynL2Writer.h"

using std::invalid_argument;

SynL2Writer::SynL2Writer() : AbstractModule("SYN_L2_WRITER") {
}

SynL2Writer::~SynL2Writer() {

}

Segment* SynL2Writer::processSegment(ProcessorContext& context) {

    string segmentId = "SYN_COLLOCATED";
    Segment& segment = context.getSegment(segmentId);
    Logger::get()->progress("Starting to write segment [" + segment.toString() + "]", getModuleId());

    // TODO - for all variables to be written, do:

    string variableName = "SDR_1";
    NcFile* ncFile = getDataFile(variableName);
    Variable* var = context.getSegment(segmentId).getIntVariable(variableName);
    NcVar* ncVar = getNcVar(    ncFile, var);

    size_t minL = context.getMaxLineComputed(segment, *this);
    size_t maxL = segment.getRowCount();
    size_t minM = segment.getM();
    size_t maxM = segment.getColCount();
    long lines = maxL - minL + 1;
    long cameras = segment.getCamCount() - segment.getMinK() + 1;
    long columns = maxM - minM + 1;

    int* values = new int[cameras * lines * columns];

    for (size_t l = minL; l < maxL; l++) {
        for (size_t k = segment.getMinK(); k < segment.getCamCount(); k++) {
            for (size_t m = minM; m < maxM; m++) {
                size_t position = 0;
//                values[position] = segment.getSampleInt(variableName, segment.computePosition(k, l, m));
            }
        }
    }

//    ncVar->put_rec(values);

    ncFile->sync();
    delete[] values;
    context.setMaxLineComputed(segment, *this, segment.getRowCount());
    return &segment;
}

NcFile* SynL2Writer::getDataFile(string variableName) {
    if (boost::starts_with(variableName, "SDR_")) {
        return getDataFileByFileName("l2_syn_surface_direction_reflectance.nc");
    }
    throw invalid_argument("SynL2Writer::getDataFile: no variable with name " + variableName + " allowed.");
}

NcFile* SynL2Writer::getDataFileByFileName(string fileName) {
    if (ncFileMap[fileName] == 0) {
        NcFile* dataFile = new NcFile(fileName.c_str(), NcFile::Replace, 0, 0, NcFile::Netcdf4);
        ncFileMap[fileName] = dataFile;
        return dataFile;
    } else {
        return ncFileMap[fileName];
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

NcVar* SynL2Writer::getNcVar(NcFile* dataFile, Variable* var) {
    if (addedVariables.find(var) == addedVariables.end()) {
        const NcDim** ncDims = createNcDims(dataFile, var->getDimensions());
        const char* varId = var->getId().c_str();
        size_t dimCount = var->getDimensions().size();
        NcType type = var->getType();
        NcVar* ncVar = dataFile->add_var(varId, type, dimCount, ncDims);
        addedVariables[var] = ncVar;
        return ncVar;
    } else {
        return addedVariables[var];
    }
}