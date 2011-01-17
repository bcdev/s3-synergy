/*
 * File:   SynL2Writer.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 3:40 PM
 */

#include <iostream>

#include "../util/IOUtils.h"
#include "../util/NetCDF.h"
#include "SynL2Writer.h"

using std::make_pair;

SynL2Writer::SynL2Writer() : Writer("SYN_L2_WRITER") {
}

SynL2Writer::~SynL2Writer() {
}

void SynL2Writer::process(Context& context) {

    for (size_t i = 0; i < variablesToWrite.size(); i++) {
        Dictionary& dict = *context.getDictionary();
        const Variable& variable = dict.getL2Variable(variablesToWrite[i]);
        const string varName = variable.getName();

        string segmentName = dict.getSegmentNameForL2(varName);
        const Segment& segment = context.getSegment(segmentName);
        const Grid& grid = segment.getGrid();
        const size_t startL = getStartL(context, segment);
        size_t endL = context.getMaxLWritable(segment, *this);

        const string ncFileName = variable.getNcFileName();

        if (!exists(varIdMap, varName)) {
            throw logic_error("Unknown variable '" + varName + "'.");
        }
        if (!exists(fileIdMap, ncFileName)) {
            throw logic_error("Unknown netCDF file '" + ncFileName + "'.");
        }
        const int varId = varIdMap[varName];
        const int ncId = fileIdMap[ncFileName];

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
        NetCDF::putData(ncId, varId, starts, sizes, accessor.getUntypedData());

        setMaxLComputed(context, varName, dict, endL);
    }
}

void SynL2Writer::start(Context& context) {
    // todo: extract directory path from job order ...
    Dictionary& dict = *context.getDictionary();

    variablesToWrite = dict.getVariables(false);
    for (size_t i = 0; i < variablesToWrite.size(); i++) {
        const Variable& variable = dict.getL2Variable(variablesToWrite[i]);
        const string segmentName = variable.getSegmentName();
        const Segment& segment = context.getSegment(segmentName);
        createNcVar(variable, segment.getGrid());
    }
    // TODO - set global attributes
    for (map<string, int>::iterator i = fileIdMap.begin(); i != fileIdMap.end(); i++) {
        NetCDF::setDefinitionPhaseFinished(i->second);
    }
}

void SynL2Writer::stop(Context& context) {
    for (map<string, int>::iterator i = fileIdMap.begin(); i != fileIdMap.end(); i++) {
        NetCDF::closeFile(i->second);
    }
    fileIdMap.clear();
    dimIdMap.clear();
    varIdMap.clear();
}

void SynL2Writer::createNcVar(const Variable& variable, const Grid& grid) {
    const string ncFileName = variable.getNcFileName();
    const string varName = variable.getName();

    if (exists(varIdMap, varName)) {
        throw logic_error("Variable '" + varName + "' already exists.");
    }
    if (!exists(fileIdMap, ncFileName)) {
        int fileId = NetCDF::createFile(variable.getNcFileName().append(".nc").c_str());

        const size_t sizeK = grid.getSizeK();
        const size_t sizeL = grid.getMaxL() - grid.getMinL() + 1;
        const size_t sizeM = grid.getSizeM();

        valarray<int> dimIds(3);
        dimIds[0] = NetCDF::defineDimension(fileId, Constants::SYMBOLIC_NAME_DIMENSION_N_CAM, sizeK);
        dimIds[1] = NetCDF::defineDimension(fileId, Constants::SYMBOLIC_NAME_DIMENSION_N_LINE_OLC, sizeL);
        dimIds[2] = NetCDF::defineDimension(fileId, Constants::SYMBOLIC_NAME_DIMENSION_N_DET_CAM, sizeM);

        fileIdMap[ncFileName] = fileId;
        dimIdMap.insert(make_pair(ncFileName, dimIds));
    }
    const int fileId = fileIdMap[ncFileName];
    const valarray<int>& dimIds = dimIdMap[ncFileName];
    int varId = NetCDF::defineVariable(fileId, variable.getNcVarName().c_str(), variable.getType(), dimIds);
    varIdMap[varName] = varId;

    const vector<Attribute*> attributes = variable.getAttributes();
    for (vector<Attribute*>::const_iterator iter = attributes.begin(); iter != attributes.end(); iter++) {
        NetCDF::addAttribute(fileId, varId, **iter);
    }
}

void SynL2Writer::setMaxLComputed(Context& context, string symbolicName, Dictionary& dict, size_t endLine ) {
    string segmentName = dict.getSegmentNameForL2(symbolicName);
    Segment* segment = &context.getSegment(segmentName);
    if (segmentVariableMap.find(segmentName) == segmentVariableMap.end()) {
        set<string> variables;
        segmentVariableMap[segmentName] = variables;
    }
    segmentVariableMap[segmentName].insert(symbolicName);
    if (isSegmentComputedByAllVariables(*segment, dict)) {
        segmentVariableMap.clear();
        context.setMaxLComputed(*segment, *this, endLine);
    }
}

const bool SynL2Writer::isSegmentComputedByAllVariables(Segment& segment, Dictionary& dict) {
    vector<string> variables = dict.getVariables(true);
    vector<string> varsForSegment;
    for (size_t i = 0; i < variables.size(); i++) {
        string varName = variables[i];
        string segmentName = dict.getSegmentNameForL2(varName);
        if (segment.getId().compare(segmentName) == 0) {
            varsForSegment.push_back(varName);
        }
    }
    set<string> varsInSegment = segmentVariableMap[segment.getId()];
    for (size_t j = 0; j < varsForSegment.size(); j++) {
        if (varsInSegment.find(varsForSegment[j]) == varsInSegment.end()) {
            return false;
        }
    }
    return true;
}