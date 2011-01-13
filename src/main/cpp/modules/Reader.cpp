/*
 * File:   Reader.cpp
 * Author: thomass
 *
 * Created on November 18, 2010, 2:08 PM
 */

#include <algorithm>
#include <boost/algorithm/string/predicate.hpp>
#include <iostream>
#include <stddef.h>
#include <stdexcept>
#include <valarray>

#include "../util/IOUtils.h"
#include "../core/JobOrder.h"
#include "../util/Logger.h"
#include "../util/NetCDF.h"
#include "Reader.h"

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

        int fileId = findFile(sourceDir, fileName);
        int varId = NetCDF::getVariableId(fileId, ncVariableName.c_str());
        int dimCount = NetCDF::getDimCountForVariable(fileId, varId);
        valarray<int> dimensionIds = NetCDF::getDimIdsForVariable(fileId, varId);

        // getting the dimension sizes
        size_t camCount = 1;
        size_t lineCount = 1;
        size_t colCount = 1;

        if (dimCount == 3) {
            camCount = NetCDF::getDimLength(fileId, dimensionIds[0]);
            lineCount = NetCDF::getDimLength(fileId, dimensionIds[1]);
            colCount = NetCDF::getDimLength(fileId, dimensionIds[2]);
        } else if (dimCount == 2) {
            lineCount = NetCDF::getDimLength(fileId, dimensionIds[0]);
            colCount = NetCDF::getDimLength(fileId, dimensionIds[1]);
        } else if (dimCount == 1) {
            lineCount = NetCDF::getDimLength(fileId, dimensionIds[0]);
        }

        size_t sizeL = min(stepSize, lineCount);
        if (context.hasSegment(segmentName)) {
            segment = &context.getSegment(segmentName);
        } else {
            segment = &context.addSegment(segmentName, sizeL, colCount, camCount, 0, lineCount - 1);
        }

        if (!segment->hasVariable(symbolicName)) {
            Variable& variable = dict.getL1cVariable(symbolicName);
            int type = NetCDF::getVariableType(fileId, varId);
            variable.setType(type);
            IOUtils::addVariableToSegment(symbolicName, type, *segment);
            //            addDimsToVariable(variable, camCount, sizeL, colCount);
        }

        Logger::get()->progress("Reading data for variable " + symbolicName + " into segment [" + segment->toString() + "]", getId());
        IOUtils::readData(fileId, varId, segment->getAccessor(symbolicName), segment->getGrid(), dimCount, context.getMaxLComputed(*segment, *this) + 1);

        endLine = segment->getGrid().getStartL() + segment->getGrid().getSizeL() - 1;
        setMaxLineComputed(context, segment, symbolicName, dict, endLine);
    }
}

void Reader::setMaxLineComputed(Context& context, Segment* segment, string& symbolicName, Dictionary& dict, size_t endLine) {
    string segmentName = segment->getId();
    if (segment->getGrid().getMaxL() == endLine) {
        completedSegments[symbolicName] = segmentName;
    }
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
    set<string> varsInSegment = segmentVariableMap[segment.getId()];
    for (size_t j = 0; j < varsForSegment.size(); j++) {
        if (varsInSegment.find(varsForSegment[j]) == varsInSegment.end()) {
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

                int fileId = NetCDF::openFile(currentFileName.c_str());
                openedFiles[fileName] = fileId;
                return fileId;
            }
        }
    }
    throw std::runtime_error("No file with name " + fileName + " found.");
}

// needed only for debugging purposes

void Reader::addDimsToVariable(Variable& variable, size_t camCount, size_t lineCount, size_t colCount) {
    variable.addDimension(new Dimension("N_CAM", camCount));
    variable.addDimension(new Dimension("N_LINE_OLC", lineCount));
    variable.addDimension(new Dimension("N_DET_CAM", colCount));
}
