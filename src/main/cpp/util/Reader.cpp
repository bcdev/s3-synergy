/*
 * File:   Reader.cpp
 * Author: thomass
 *
 * Created on November 18, 2010, 2:08 PM
 */

#include <algorithm>
#include <boost/algorithm/string/predicate.hpp>
#include <iostream>
#include <vector>
#include <valarray>
#include <stdexcept>


#include "JobOrder.h"
#include "Logger.h"
#include "Reader.h"

using std::min;

Reader::Reader() : DefaultModule("READ"), stepSize(500) {
}

Reader::~Reader() {
}

void Reader::process(Context& context) {
    // switch between processor configurations

    string fileName = getFileName(context, "SYL2");
    NcFile* dataFile = new NcFile(fileName.c_str(), NcFile::ReadOnly);
    size_t camCount = dataFile->get_dim("N_CAM")->size();
    size_t colCount = dataFile->get_dim("N_DET_CAM")->size();
    size_t lineCount = dataFile->get_dim("N_LINE_OLC")->size();

    if (!context.hasSegment("SYN_COLLOCATED")) {
        size_t sizeL = min(stepSize, lineCount);
        // initial reading

        Segment& segment = context.addSegment("SYN_COLLOCATED", sizeL, colCount, camCount, 0, lineCount);
        Logger::get()->progress("Reading data for segment [" + segment.toString() + "]", "Reader");

        // TODO - for all variables do {

        // creating variables
        segment.addVariableShort("TOA_Radiance_Meas_1");
        //segment.addVariableShort("error_estimates");

        readData(dataFile, sizeL, camCount, colCount, "TOA_Radiance_Meas", &segment.getAccessor("TOA_Radiance_Meas_1").getShortData()[0]);
        context.setMaxLComputed(segment, *this, sizeL - 1);
    } else {
        // modifying segment bounds
        Segment& segment = context.getSegment("SYN_COLLOCATED");
        size_t minRequiredLine = context.getMinLRequired(segment, context.getMaxLComputed(segment, *this) + 1);
        segment.setStartL(minRequiredLine);
        Grid& grid = segment.getGrid();

        // modifying segment values
        Logger::get()->progress("Reading data for segment [" + segment.toString() + "]", "Reader");
        size_t startLine = context.getMaxLComputed(segment, *this) + 1;
        size_t endLine = minRequiredLine + grid.getSizeL() - 1;

        size_t lines = endLine - startLine + 1;
        for (size_t k = 0; k < camCount; k++) {
            readData(dataFile, lines, 1, colCount, "TOA_Radiance_Meas", &segment.getAccessor("TOA_Radiance_Meas_1").getShortData()[grid.getIndex(k, startLine, 0)]);
        }
        context.setMaxLComputed(segment, *this, endLine);
    }
}

string Reader::getFileName(Context& context, string processorId) {
    vector<string> inputFileNames = context.getJobOrder()->getProcessorConfiguration(processorId).getInputList()[0]->getFileNames();
    for (size_t i = 0; i < inputFileNames.size(); i++) {
        if (boost::contains(inputFileNames[i], "OLC_RADIANCE_O1")) {
            return inputFileNames[i];
        }
    }
    throw std::invalid_argument( "No input file for processor " + processorId + "." );
}

void Reader::readData(NcFile* dataFile, size_t lines, size_t camCount, size_t colCount, string varName, short* data) {
    NcVar* var = dataFile->get_var(varName.c_str());
    var->get(data, camCount, lines, colCount);
}
