/*
 * File:   SynL2Writer.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 3:40 PM
 */

#include <boost/algorithm/string/predicate.hpp>
#include <stdexcept>
#include <set>
#include <netcdf.h>

#include "../core/Context.h"
#include "IOUtils.h"
#include "../util/Logger.h"
#include "SynL2Writer.h"

using std::invalid_argument;

SynL2Writer::SynL2Writer() : DefaultModule("SYN_L2_WRITER") {
}

SynL2Writer::~SynL2Writer() {

}

void SynL2Writer::process(Context& context) {
    string segmentId = "SYN_COLLOCATED";
    Segment& segment = context.getSegment(segmentId);

    Grid& grid = segment.getGrid();

    size_t startLine = getStartL(context, segment);
    size_t endLine = getDefaultEndL(startLine, grid);

    for (size_t i = 0; i < variablesToWrite.size(); i++) {
        Variable& variable = context.getDictionary()->getVariable(variablesToWrite[i]);

        string ncVariableName = variable.getNcName();
        string symbolicName = variable.getSymbolicName();

        Logger::get()->progress("Writing variable " + symbolicName + " into "
                "segment [" + segment.toString() + "]", getId());

        string fileName = context.getDictionary()->getNcFileNameForSymbolicName(symbolicName);
        fileName.append(".nc");

        const int ncId = getNcId(fileName);
        vector<Dimension*> varDimensions = variable.getDimensions();
        int* dimensionIds = IOUtils::createNcDims(ncId, varDimensions);

        size_t dimCount = varDimensions.size();

        int varId;
        int status = nc_inq_varid(ncId, ncVariableName.c_str(), &varId);
        if (status != NC_NOERR) {
            status = nc_def_var(ncId, ncVariableName.c_str(), variable.getType(), dimCount, dimensionIds, &varId);
        }
        if (status != NC_NOERR) {
            throw std::runtime_error("Unable to find or create NetCDF-variable " + ncVariableName + ".");
        }

        // getting the dimension sizes
        size_t camCount = 1;
        size_t lineCount = 1;
        size_t colCount = 1;
        nc_inq_dimlen(ncId, dimensionIds[0], &camCount);
        nc_inq_dimlen(ncId, dimensionIds[1], &lineCount);
        nc_inq_dimlen(ncId, dimensionIds[2], &colCount);

        Accessor& accessor = segment.getAccessor(symbolicName);
        IOUtils::write(ncId, varId, variable.getType(), dimCount, camCount, lineCount, colCount, segment, startLine, endLine, accessor);
        nc_close(ncId);
    }
    context.setMaxLComputed(segment, *this, endLine);
}

void SynL2Writer::start(Context& context) {
    // add symbolic names of all variables to be written
    variablesToWrite.push_back("L_1");
    variablesToWrite.push_back("L_2");
    //    variablesToWrite.push_back("SYN_flags");
}

const int SynL2Writer::getNcId(string fileName) {
    if (ncFiles.find(fileName) == ncFiles.end()) {
        int ncId;
        int status = nc_create(fileName.c_str(), NC_NETCDF4, &ncId);
        if (status != NC_NOERR) {
            throw std::runtime_error("Unable to create NetCDF-file " + fileName + ".");
        }
        ncFiles.insert(fileName);
        return ncId;
    } else {
        int ncId;
        int status = nc_open(fileName.c_str(), NC_WRITE, &ncId);
        if (status != NC_NOERR) {
            throw std::runtime_error("Unable to write to NetCDF-file " + fileName + ".");
        }
        ncFiles.insert(fileName);
        return ncId;
    }
}