/*
 * File:   Reader.cpp
 * Author: thomass
 *
 * Created on November 18, 2010, 2:08 PM
 */

#include <algorithm>
#include <boost/algorithm/string/predicate.hpp>
#include <iostream>
#include <netcdf.h>
#include <stddef.h>
#include <stdexcept>
#include <valarray>

#include "JobOrder.h"
#include "Logger.h"
#include "Reader.h"
#include "IOUtils.h"

using std::min;
using std::max;

Reader::Reader() : DefaultModule("READ"), stepSize(2000) {
}

Reader::~Reader() {
}

void Reader::process(Context& context) {
    // switch between processor configurations

    Dictionary& dict = *(context.getDictionary());
    string processorId = "SYL2";
    string sourceDir = context.getJobOrder()->getProcessorConfiguration(processorId).getInputList()[0]->getFileNames()[0];

    Segment* segment;
    size_t endLine;

    // Modify each segment's bounds, if necessary
    modifyBoundsOfSegments(context);

    // read from each variable
    const vector<string> variablesToBeRead = dict.getVariables(true);
    for (size_t varIndex = 0; varIndex < variablesToBeRead.size(); varIndex++) {
        string symbolicName = variablesToBeRead[varIndex];
        string ncVariableName = dict.getL1cNcVarName(symbolicName);
        string fileName = dict.getL1cNcFileNameForSymbolicName(symbolicName);
        const string& segmentName = dict.getSegmentNameForL1c(symbolicName);

        if (hasSegmentComplete(symbolicName)) {
            continue;
        }

        int ncId = findFile(sourceDir, fileName);

        // getting the netCDF-variable-id
        int varId;
        nc_inq_varid(ncId, ncVariableName.c_str(), &varId);

        // getting the number of dimensions of the variable
        int dimCount;
        nc_inq_varndims(ncId, varId, &dimCount);

        // getting the dimension-ids
        valarray<int> dimensionIds(dimCount);
        nc_inq_vardimid(ncId, varId, &dimensionIds[0]);

        // getting the dimension sizes
        size_t camCount = 1;
        size_t lineCount = 1;
        size_t colCount = 1;

        if (dimCount == 3) {
            nc_inq_dimlen(ncId, dimensionIds[0], &camCount);
            nc_inq_dimlen(ncId, dimensionIds[1], &lineCount);
            nc_inq_dimlen(ncId, dimensionIds[2], &colCount);
        } else if (dimCount == 2) {
            nc_inq_dimlen(ncId, dimensionIds[0], &lineCount);
            nc_inq_dimlen(ncId, dimensionIds[1], &colCount);
        } else if (dimCount == 1) {
            nc_inq_dimlen(ncId, dimensionIds[0], &lineCount);
        }

        size_t sizeL = min(stepSize, lineCount);
        if (context.hasSegment(segmentName)) {
            segment = &context.getSegment(segmentName);
        } else {
            segment = &context.addSegment(segmentName, sizeL, colCount, camCount, 0, lineCount - 1);
        }

        if (!segment->hasVariable(symbolicName)) {
            Variable& variable = dict.getL1cVariable(symbolicName);
            setVariableType(ncId, varId, variable);
            nc_type type = getVariableType(ncId, varId, variable);
            IOUtils::addVariableToSegment(symbolicName, type, *segment);
//            addDimsToVariable(variable, camCount, sizeL, colCount);
        }

        Logger::get()->progress("Reading data for variable " + symbolicName + " into segment [" + segment->toString() + "]", getId());
        IOUtils::readData(ncId, varId, segment->getAccessor(symbolicName), segment->getGrid(), dimCount, context.getMaxLComputed(*segment, *this) + 1);

        endLine = segment->getGrid().getStartL() + segment->getGrid().getSizeL() - 1;
        setMaxLineComputed(context, segment, symbolicName, dict, endLine);
    }
}

void Reader::setMaxLineComputed(Context& context, Segment* segment, string& symbolicName, Dictionary& dict, size_t endLine) {
    string segmentName = segment->getId();
    if (segment->getGrid().getMaxL() == endLine) {
        completedSegments[symbolicName] = segmentName;
    }
    if( segmentVariableMap.find(segmentName) == segmentVariableMap.end() ) {
        set<string> variables;
        segmentVariableMap[segmentName] = &variables;
//        segmentVariableMap[segmentName] = new set<string>();
    }
    segmentVariableMap[segmentName]->insert(symbolicName);
    if (isSegmentComputedByAllVariables(*segment, dict)) {
        segmentVariableMap.clear();
        context.setMaxLComputed(*segment, *this, endLine);
    }
}

const bool Reader::isSegmentComputedByAllVariables(Segment& segment, Dictionary& dict) {
    vector<string> variables = dict.getVariables(true);
    vector<string> varsForSegment;
    for (size_t i = 0; i < variables.size(); i++) {
        string varName = variables[i];
        string segmentName = dict.getSegmentNameForL1c(varName);
        if (segment.getId().compare(segmentName) == 0) {
            varsForSegment.push_back(varName);
        }
    }
    set<string>* varsInSegment = segmentVariableMap[segment.getId()];
    for (size_t j = 0; j < varsForSegment.size(); j++) {
        if (varsInSegment->find(varsForSegment[j]) == varsInSegment->end()) {
            return false;
        }
    }
    return true;
}

void Reader::modifyBoundsOfSegments(const Context& context) {
    const vector<Segment*> segments = context.getSegments();
    for (size_t i = 0; i < segments.size(); i++) {
        Segment* segment = segments[i];
        size_t maxL = segment->getGrid().getStartL() + segment->getGrid().getSizeL() - 1;
        if (context.getMaxLComputed(*segment, *this) == maxL &&
                !isSegmentComputedByAllVariables(*segment, *context.getDictionary())) {
            size_t minRequiredLine = context.getMinLRequired(*segment, context.getMaxLComputed(*segment, *this) + 1);
            segment->setStartL(minRequiredLine);
        }
    }
}

const bool Reader::hasSegmentComplete(const string& symbolicName) const {
    return completedSegments.find(symbolicName) != completedSegments.end();
}

const int Reader::findFile(string& sourceDir, string& fileName) {
    vector<string> fileNames = IOUtils::getFiles(sourceDir);
    if (openedFiles.find(fileName) != openedFiles.end()) {
        return openedFiles[fileName];
    }

    for (size_t i = 0; i < fileNames.size(); i++) {
        string currentFileName = sourceDir + "/" + fileNames[i];
        if (boost::ends_with(currentFileName, ".nc")) {
            if (boost::ends_with(currentFileName, fileName + ".nc") ||
                    boost::ends_with(currentFileName, fileName)) {
                int ncId;
                if (nc_open(currentFileName.c_str(), NC_NOWRITE, &ncId) != NC_NOERR) {
                    throw std::runtime_error("Unable to open netCDF-file " + currentFileName + ".");
                } else {
                    openedFiles[fileName] = ncId;

                    return ncId;
                }
            }
        }
    }
    throw std::runtime_error("No file with name " + fileName + " found.");
}

const void Reader::setVariableType(int ncId, int varId, Variable& variable) {

    nc_type type;
    nc_inq_vartype(ncId, varId, &type);
    variable.setType(type);
}

const nc_type Reader::getVariableType(int ncId, int varId, Variable& variable) {
    nc_type type;
    nc_inq_vartype(ncId, varId, &type);

    return type;
}

// needed only for debugging purposes

void Reader::addDimsToVariable(Variable& variable, size_t camCount, size_t lineCount, size_t colCount) {
    variable.addDimension(new Dimension("N_CAM", camCount));
    variable.addDimension(new Dimension("N_LINE_OLC", lineCount));
    variable.addDimension(new Dimension("N_DET_CAM", colCount));
}
