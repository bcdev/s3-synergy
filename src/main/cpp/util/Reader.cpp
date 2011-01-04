/*
 * File:   Reader.cpp
 * Author: thomass
 *
 * Created on November 18, 2010, 2:08 PM
 */

#include <algorithm>
#include <boost/algorithm/string/predicate.hpp>
#include <iostream>
#include <valarray>
#include <stdexcept>

#include "JobOrder.h"
#include "Logger.h"
#include "Reader.h"
#include "ReaderConstants.h"

using std::min;
using std::max;

Reader::Reader() : DefaultModule("READ"), stepSize(1000) {
}

Reader::~Reader() {
}

void Reader::process(Context& context) {
    // switch between processor configurations

    Dictionary& dict = *(context.getDictionary());
    string processorId = "SYL2";
    vector<string> fileNames = context.getJobOrder()->getProcessorConfiguration(processorId).getInputList()[0]->getFileNames();
    const vector<string>& variablesToRead = ReaderConstants::getVariablesToRead();
    // read from each file
    for (size_t i = 0; i < fileNames.size(); i++) {
        // read from each variable
        for (size_t varIndex = 0; varIndex < variablesToRead.size(); varIndex++) {
            string symbolicVarName = variablesToRead[varIndex];

            string fileName = fileNames[i];
            NcFile* dataFile = new NcFile(fileName.c_str(), NcFile::ReadOnly);

            // TODO - generify dimensions (perhaps create new Reader class)

            size_t camCount = dataFile->get_dim("N_CAM")->size();
            size_t colCount = dataFile->get_dim("N_DET_CAM")->size();
            size_t lineCount = dataFile->get_dim("N_LINE_OLC")->size();

            if (!context.hasSegment("SYN_COLLOCATED")) {
                size_t sizeL = min(stepSize, lineCount);
                // initial reading

                Segment& segment = context.addSegment("SYN_COLLOCATED", sizeL, colCount, camCount, 0, lineCount - 1);
                Logger::get()->progress("Reading data for segment [" + segment.toString() + "]", getId());

                addVariableToSegment(symbolicVarName, segment, dict);
                readData(dataFile, sizeL - 1, camCount, colCount,
                        symbolicVarName, segment, dict, 0);

                context.setMaxLComputed(segment, *this, sizeL - 1);
            } else {
                Segment& segment = context.getSegment("SYN_COLLOCATED");
                modifySegmentBounds(context, segment);

                // modifying segment values
                Grid& grid = segment.getGrid();
                Logger::get()->progress("Reading data for segment [" + segment.toString() + "]", getId());
                size_t startLine = getStartL(context, segment);
                size_t endLine = getDefaultEndL(startLine, grid);
                size_t lines = endLine - startLine + 1;

                for (size_t k = 0; k < camCount; k++) {
                    readData(dataFile, lines, k, colCount, symbolicVarName,
                            segment, dict, grid.getIndex(k, startLine, 0));
                }
                context.setMaxLComputed(segment, *this, endLine);
            }
        }
    }
}

void Reader::modifySegmentBounds(const Context& context, Segment& segment) {
    size_t minRequiredLine = context.getMinLRequired(segment, context.getMaxLComputed(segment, *this) + 1);
    segment.setStartL(minRequiredLine);
}

void Reader::addVariableToSegment(const string variable, Segment& segment, Dictionary dict) {
    NcType type = dict.getVariable(variable).getType();
    switch (type) {
        case ncByte:
        {
            segment.addVariableByte(variable);
            break;
        }
        case ncShort:
        {
            segment.addVariableShort(variable);
            break;
        }
        case ncInt:
        {
            segment.addVariableInt(variable);
            break;
        }
        case ncFloat:
        {
            segment.addVariableFloat(variable);
            break;
        }
        case ncDouble:
        {
            segment.addVariableDouble(variable);
            break;
        }
        default:
        {
            throw std::invalid_argument("Variable " + variable + " has invalid type.");
        }
    }
}

void Reader::readData(NcFile* dataFile, const size_t lines,
        const size_t camCount, const size_t colCount, const string& symbolicName,
        Segment& segment, Dictionary dict, size_t index) {
    string ncName = dict.getNcVarName(symbolicName);
    NcType type = dict.getVariable(symbolicName).getType();
    switch (type) {
        case ncByte:
        {
            ncbyte* buffer = &segment.getAccessor(symbolicName).getByteData()[index];
            NcVar* var = dataFile->get_var(ncName.c_str());
            var->get(buffer, camCount, lines, colCount);
            break;
        }
        case ncShort:
        {
            short* buffer = &segment.getAccessor(symbolicName).getShortData()[index];
            NcVar* var = dataFile->get_var(ncName.c_str());
            var->get(buffer, camCount, lines, colCount);
            break;
        }
        case ncInt:
        {
            int* buffer = &segment.getAccessor(symbolicName).getIntData()[index];
            NcVar* var = dataFile->get_var(ncName.c_str());
            var->get(buffer, camCount, lines, colCount);
            break;
        }
        case ncFloat:
        {
            float* buffer = &segment.getAccessor(symbolicName).getFloatData()[index];
            NcVar* var = dataFile->get_var(ncName.c_str());
            var->get(buffer, camCount, lines, colCount);
            break;
        }
        case ncDouble:
        {
            double* buffer = &segment.getAccessor(symbolicName).getDoubleData()[index];
            NcVar* var = dataFile->get_var(ncName.c_str());
            var->get(buffer, camCount, lines, colCount);
            break;
        }
        default:
        {
            throw std::invalid_argument("Variable " + symbolicName + " has invalid type.");
        }
    }
}