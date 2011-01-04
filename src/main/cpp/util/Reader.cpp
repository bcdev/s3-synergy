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
            string symbolicName = variablesToRead[varIndex];

            string fileName = fileNames[i];
//            NcFile* dataFile = new NcFile(fileName.c_str(), NcFile::ReadOnly);
            int ncId;
            nc_open(fileName.c_str(), NC_NOWRITE, &ncId);

            string ncVariableName = dict.getNcVarName(symbolicName);
//            NcVar* var = dataFile->get_var(ncName.c_str());

            // getting the netCDF-variable-id
            int varId;
            nc_inq_varid(ncId, ncVariableName.c_str(), &varId);

            // getting the number of dimensions of the variable
            int dimensionCount;
            nc_inq_varndims(ncId, varId, &dimensionCount);

            // getting the dimension-ids
            int* dimensionIds = new int[dimensionCount];
            nc_inq_vardimid(ncId, varId, dimensionIds);

            size_t camCount = 1;
            size_t lineCount = 1;
            size_t colCount = 1;
            if( dimensionCount == 3 ) {
                nc_inq_dimlen(ncId, dimensionIds[0], &camCount);
                nc_inq_dimlen(ncId, dimensionIds[1], &lineCount);
                nc_inq_dimlen(ncId, dimensionIds[2], &colCount);
            } else if( dimensionCount == 2 ) {
                nc_inq_dimlen(ncId, dimensionIds[0], &lineCount);
                nc_inq_dimlen(ncId, dimensionIds[1], &colCount);
            } else if( dimensionCount == 1 ) {
                nc_inq_dimlen(ncId, dimensionIds[0], &lineCount);
            } else {
                throw std::runtime_error("Wrong number of dimensions in netCDF-file " + fileName + ".");
            }

            if (!context.hasSegment("SYN_COLLOCATED")) {
                size_t sizeL = min(stepSize, lineCount);
                // initial reading

                Segment& segment = context.addSegment("SYN_COLLOCATED", sizeL, colCount, camCount, 0, lineCount - 1);
                Logger::get()->progress("Reading data for segment [" + segment.toString() + "]", getId());

                addVariableToSegment(symbolicName, segment, dict);
//                readData(dataFile, sizeL - 1, camCount, colCount,
//                        symbolicName, segment, dict, 0);

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
//                    readData(dataFile, lines, k, colCount, symbolicName,
//                            segment, dict, grid.getIndex(k, startLine, 0));
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

//void Reader::readData(NcFile* dataFile, const size_t lines,
//        const size_t camCount, const size_t colCount, const string& symbolicName,
//        Segment& segment, Dictionary dict, size_t index) {
//    string ncName = dict.getNcVarName(symbolicName);
//    NcVar* var = dataFile->get_var(ncName.c_str());
//    NcType type = dict.getVariable(symbolicName).getType();
//    switch (type) {
//        case ncByte:
//        {
//            ncbyte* buffer = &segment.getAccessor(symbolicName).getByteData()[index];
//            var->get(buffer, camCount, lines, colCount);
//            break;
//        }
//        case ncShort:
//        {
//            short* buffer = &segment.getAccessor(symbolicName).getShortData()[index];
//            NcVar* var = dataFile->get_var(ncName.c_str());
//            var->get(buffer, camCount, lines, colCount);
//            break;
//        }
//        case ncInt:
//        {
//            int* buffer = &segment.getAccessor(symbolicName).getIntData()[index];
//            NcVar* var = dataFile->get_var(ncName.c_str());
//            var->get(buffer, camCount, lines, colCount);
//            break;
//        }
//        case ncFloat:
//        {
//            float* buffer = &segment.getAccessor(symbolicName).getFloatData()[index];
//            NcVar* var = dataFile->get_var(ncName.c_str());
//            var->get(buffer, camCount, lines, colCount);
//            break;
//        }
//        case ncDouble:
//        {
//            double* buffer = &segment.getAccessor(symbolicName).getDoubleData()[index];
//            NcVar* var = dataFile->get_var(ncName.c_str());
//            var->get(buffer, camCount, lines, colCount);
//            break;
//        }
//        default:
//        {
//            throw std::invalid_argument("Variable " + symbolicName + " has invalid type.");
//        }
//    }
//}