/*
 * File:   SynL2Writer.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 3:40 PM
 */

#include <boost/algorithm/string/predicate.hpp>
#include <netcdf.h>
#include <stdexcept>
#include <netcdf.h>

#include "../core/Context.h"
#include "IOUtils.h"
#include "../util/Logger.h"
#include "SynL2Writer.h"

SynL2Writer::SynL2Writer() : DefaultModule("SYN_L2_WRITER"), ncIdMap(), ncIdsMap() {
}

SynL2Writer::~SynL2Writer() {
}

void SynL2Writer::process(Context& context) {
    //    const Segment& segment = context.getSegment(Constants::SYMBOLIC_NAME_SEGMENT_SYN_COLLOCATED);
    //    const Grid& grid = segment.getGrid();
    //    const size_t startL = getStartL(context, segment);
    //    const size_t endLine = getDefaultEndL(startL, grid);
    //
    //    for (size_t i = 0; i < variablesToWrite.size(); i++) {
    //        Variable& variable = context.getDictionary()->getL2Variable(variablesToWrite[i]);
    //
    //        const string ncVariableName = variable.getNcName();
    //        const string symbolicName = variable.getSymbolicName();
    //
    //        Logger::get()->progress("Writing variable " + symbolicName + " of "
    //                "segment [" + segment.toString() + "]", getId());
    //
    //        const string fileName = context.getDictionary()->getL2NcFileNameForSymbolicName(symbolicName).append(".nc");
    //        const int ncId = getNcIds(fileName);
    //
    //        vector<Dimension*> varDimensions = variable.getDimensions();
    //        size_t dimCount = varDimensions.size();
    //        int* dimensionIds = IOUtils::getNcDims(ncId, varDimensions);
    //
    //        int varId;
    //        int status = nc_inq_varid(ncId, ncVariableName.c_str(), &varId);
    //        if (status != NC_NOERR) {
    //            status = nc_def_var(ncId, ncVariableName.c_str(), variable.getType(), dimCount, dimensionIds, &varId);
    //        }
    //        if (status != NC_NOERR) {
    //            throw std::runtime_error("Unable to find or create NetCDF-variable " + ncVariableName + ".");
    //        }
    //
    //        // getting the dimension sizes
    //        size_t camCount = 1;
    //        size_t colCount = 1;
    //        nc_inq_dimlen(ncId, dimensionIds[0], &camCount);
    //        nc_inq_dimlen(ncId, dimensionIds[2], &colCount);
    //
    //        size_t lineCount = min(grid.getMaxL() - grid.getStartL() + 1, grid.getSizeL());
    //        Accessor& accessor = segment.getAccessor(symbolicName);
    //        IOUtils::write(ncId, varId, variable.getType(), dimCount, camCount, lineCount, colCount, segment, startL, endLine, accessor);
    //        nc_close(ncId);
    //    }
    //    context.setMaxLComputed(*segment, *this, endLine);
}

void SynL2Writer::start(Context& context) {
    variablesToWrite.push_back("L_1");
    const Segment& segment = context.getSegment(Constants::SYMBOLIC_NAME_SEGMENT_SYN_COLLOCATED);

    for (size_t i = 0; i < variablesToWrite.size(); i++) {
        const Variable& variable = context.getDictionary()->getL2Variable(variablesToWrite[i]);
        createNcVar(variable, segment.getGrid());
    }
    for (map<string, int>::iterator i = ncIdMap.begin(); i != ncIdMap.end(); i++) {
        nc_enddef(i->second);
    }
}

void SynL2Writer::stop(Context& context) {
    for (map<string, int>::iterator i = ncIdMap.begin(); i != ncIdMap.end(); i++) {
        nc_close(i->second);
    }
    ncIdsMap.clear();
    ncIdMap.clear();
}

void SynL2Writer::createNcVar(const Variable& variable, const Grid& grid) {
    if (!exists(ncIdMap, variable.getFileName())) {
        vector<int> ncIds(-1, 5);
        nc_create(variable.getFileName().c_str(), NC_NETCDF4, &ncIds[0]);

        const size_t sizeK = grid.getSizeK();
        const size_t sizeL = grid.getMaxL() - grid.getMinL() + 1;
        const size_t sizeM = grid.getSizeM();

        nc_def_dim(ncIds[0], Constants::SYMBOLIC_NAME_DIMENSION_N_CAM, sizeK, &ncIds[2]);
        nc_def_dim(ncIds[0], Constants::SYMBOLIC_NAME_DIMENSION_N_LINE_OLC, sizeL, &ncIds[3]);
        nc_def_dim(ncIds[0], Constants::SYMBOLIC_NAME_DIMENSION_N_DET_CAM, sizeM, &ncIds[4]);

        ncIdMap[variable.getFileName()] = ncIds[0];
        ncIdsMap[variable.getSymbolicName()] = ncIds;
    }
    vector<int>& ncIds = ncIdsMap[variable.getSymbolicName()];
    if (ncIds[1] == -1) {
        nc_def_var(ncIds[0], variable.getNcName().c_str(), variable.getType(), 3, &ncIds[2], &ncIds[1]);
    }
}