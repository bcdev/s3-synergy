/*
 * File:   SynL2Writer.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 3:40 PM
 */

#include <netcdf.h>
#include <iostream>

#include "SynL2Writer.h"

using std::make_pair;

SynL2Writer::SynL2Writer() : DefaultModule("SYN_L2_WRITER"), Writer() {
}

SynL2Writer::~SynL2Writer() {
}

void SynL2Writer::process(Context& context) {
    const Segment& segment = context.getSegment(Constants::SYMBOLIC_NAME_SEGMENT_SYN_COLLOCATED);
    const Grid& grid = segment.getGrid();
    const size_t startL = getStartL(context, segment);
    const size_t endL = context.getMaxLWritable(segment, *this);

    for (size_t i = 0; i < variablesToWrite.size(); i++) {
        const Variable& variable = context.getDictionary()->getL2Variable(variablesToWrite[i]);
        const string varName = variable.getSymbolicName();
        const string ncFileName = variable.getFileName();

        if (!exists(varIdMap, varName)) {
            throw logic_error("Unknown variable '" + varName + "'.");
        }
        if (!exists(ncIdMap, ncFileName)) {
            throw logic_error("Unknown netCDF file '" + ncFileName + "'.");
        }
        const int varId = varIdMap[varName];
        const int ncId = ncIdMap[ncFileName];

        valarray<size_t> starts(3);
        valarray<size_t> sizes(3);
        starts[0] = grid.getStartK();
        starts[1] = startL;
        starts[2] = grid.getStartM();
        sizes[0] = grid.getSizeK();
        sizes[1] = endL - startL + 1;
        sizes[2] = grid.getSizeM();

        if (context.getLogging() != 0) {
            context.getLogging()->progress("Writing variable " + varName + " of segment [" + segment.toString() + "]", getId());
        }
        const Accessor& accessor = segment.getAccessor(varName);
        nc_put_vars(ncId, varId, &starts[0], &sizes[0], 0, accessor.getUntypedData());
    }
    context.setMaxLComputed(segment, *this, endL);

    // todo: other segments ...
}

void SynL2Writer::start(Context& context) {
    // todo: extract directory path from job order ...

    variablesToWrite = context.getDictionary()->getVariables(false);
    const Segment& segment = context.getSegment(Constants::SYMBOLIC_NAME_SEGMENT_SYN_COLLOCATED);

    for (size_t i = 0; i < variablesToWrite.size(); i++) {
        const Variable& variable = context.getDictionary()->getL2Variable(variablesToWrite[i]);
        createNcVar(variable, segment.getGrid());
    }
    for (map<string, int>::iterator i = ncIdMap.begin(); i != ncIdMap.end(); i++) {
        nc_enddef(i->second);
    }

    // todo: other segments ...
}

void SynL2Writer::stop(Context& context) {
    for (map<string, int>::iterator i = ncIdMap.begin(); i != ncIdMap.end(); i++) {
        nc_close(i->second);
    }
    ncIdMap.clear();
    dimIdMap.clear();
    varIdMap.clear();
}

void SynL2Writer::createNcVar(const Variable& variable, const Grid& grid) {
    const string ncFileName = variable.getFileName();
    const string varName = variable.getSymbolicName();

    if (exists(varIdMap, varName)) {
        throw logic_error("Variable '" + varName + "' already exists.");
    }
    if (!exists(ncIdMap, ncFileName)) {
        int ncId;
        nc_create(variable.getFileName().append(".nc").c_str(), NC_NETCDF4, &ncId);

        const size_t sizeK = grid.getSizeK();
        const size_t sizeL = grid.getMaxL() - grid.getMinL() + 1;
        const size_t sizeM = grid.getSizeM();

        valarray<int> dimIds(3);
        nc_def_dim(ncId, Constants::SYMBOLIC_NAME_DIMENSION_N_CAM, sizeK, &dimIds[0]);
        nc_def_dim(ncId, Constants::SYMBOLIC_NAME_DIMENSION_N_LINE_OLC, sizeL, &dimIds[1]);
        nc_def_dim(ncId, Constants::SYMBOLIC_NAME_DIMENSION_N_DET_CAM, sizeM, &dimIds[2]);

        ncIdMap[ncFileName] = ncId;
        dimIdMap.insert(make_pair(ncFileName, dimIds));
    }
    const int ncId = ncIdMap[ncFileName];
    const valarray<int>& dimIds = dimIdMap[ncFileName];
    int varId;
    nc_def_var(ncId, variable.getNcName().c_str(), variable.getType(), 3, &dimIds[0], &varId);
    varIdMap[varName] = varId;

    // todo: attributes ...
}