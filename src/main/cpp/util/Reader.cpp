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
using std::max;

const string Reader::OLC_TOA_RADIANCE_MEAS_1 = "OLC_RADIANCE_O1_TOA_Radiance_Meas";

Reader::Reader() : DefaultModule("READ"), stepSize(2) {
}

Reader::~Reader() {
}

void Reader::process(Context& context) {
    // switch between processor configurations

    Dictionary& dict = *(context.getDictionary());
    string processorId = "SYL2";
    vector<string> fileNames = context.getJobOrder()->getProcessorConfiguration(processorId).getInputList()[0]->getFileNames();

    // read from each file
    for (size_t i = 0; i < fileNames.size(); i++) {

        vector<string> ncNames;
        vector<string> symbolicNames;

        string fileName = fileNames[i];
        NcFile* dataFile = new NcFile(fileName.c_str(), NcFile::ReadOnly);
        size_t camCount = dataFile->get_dim("N_CAM")->size();
        size_t colCount = dataFile->get_dim("N_DET_CAM")->size();
        size_t lineCount = dataFile->get_dim("N_LINE_OLC")->size();

        if (!context.hasSegment("SYN_COLLOCATED")) {
            size_t sizeL = min(stepSize, lineCount);
            // initial reading

            Segment& segment = context.addSegment("SYN_COLLOCATED", sizeL, colCount, camCount, 0, lineCount - 1);
            Logger::get()->progress("Reading data for segment [" + segment.toString() + "]", getId());

            vector<string>::iterator iter = variablesToRead.begin();
            for (; iter != variablesToRead.end(); iter++) {
                string variable = *iter;

                // creating variables
                segment.addVariableShort(variable);
                readData(dataFile, sizeL - 1, camCount, colCount, dict.getNcVarName(variable, fileName), &segment.getAccessor(variable).getShortData()[0]);
            }

            context.setMaxLComputed(segment, *this, sizeL - 1);
        } else {
            // modifying segment bounds
            Segment& segment = context.getSegment("SYN_COLLOCATED");
            size_t minRequiredLine = context.getMinLRequired(segment, context.getMaxLComputed(segment, *this) + 1);
            segment.setStartL(minRequiredLine);
            Grid& grid = segment.getGrid();

            // modifying segment values
            Logger::get()->progress("Reading data for segment [" + segment.toString() + "]", getId());
            size_t startLine = getStartL(context, segment);
            size_t endLine = getDefaultEndL(startLine, grid);
            size_t lines = endLine - startLine + 1;

            vector<string>::iterator iter = variablesToRead.begin();
            for (; iter != variablesToRead.end(); iter++) {
                string variable = *iter;
                for (size_t k = 0; k < camCount; k++) {
                    readData(dataFile, lines, k, colCount, dict.getNcVarName(variable, fileName), &segment.getAccessor(variable).getShortData()[grid.getIndex(k, startLine, 0)]);
                }
            }
            context.setMaxLComputed(segment, *this, endLine);
        }
    }
}

void Reader::readData(NcFile* dataFile, size_t lines, size_t camCount, size_t colCount, string varName, short* data) {
    NcVar* var = dataFile->get_var(varName.c_str());
    var->get(data, camCount, lines, colCount);
}

void Reader::start(Context& context) {
    variablesToRead.push_back(OLC_TOA_RADIANCE_MEAS_1);
}