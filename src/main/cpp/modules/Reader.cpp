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

void Reader::start(Context& context) {
    if (context.getJobOrder() == 0) {
        throw logic_error("Reader::start: job order missing.");
    }
    const string processorId = "SYL2";
    const string sourceDir = context.getJobOrder()->getProcessorConfiguration(processorId).getInputList()[0]->getFileNames()[0];
    Dictionary& dict = *context.getDictionary();

    const vector<SegmentDescriptor*> segmentDescriptors =
            dict.getProductDescriptor(Constants::SYMBOLIC_NAME_L1C).getSegmentDescriptors();

    foreach(SegmentDescriptor* segmentDescriptor, segmentDescriptors) {
        const vector<VariableDescriptor*> variableDescriptors =
                segmentDescriptor->getVariableDescriptors();
        const string& segmentName = segmentDescriptor->getName();

        foreach(VariableDescriptor* variableDescriptor, variableDescriptors) {
            const string symbolicName = variableDescriptor->getName();
            const string ncVariableName = variableDescriptor->getNcVarName();
            const string fileName = variableDescriptor->getNcFileName();

            int fileId = findFile(sourceDir, fileName);
            int varId = NetCDF::getVariableId(fileId, ncVariableName.c_str());
            const int type = NetCDF::getVariableType(fileId, varId);
            variableDescriptor->setType(type);
            varIdMap[symbolicName] = varId;
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
                variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimensionIds[0])).setSize(camCount);
                variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimensionIds[1])).setSize(lineCount);
                variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimensionIds[2])).setSize(colCount);
            } else if (dimCount == 2) {
                lineCount = NetCDF::getDimLength(fileId, dimensionIds[0]);
                colCount = NetCDF::getDimLength(fileId, dimensionIds[1]);
                variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimensionIds[0])).setSize(lineCount);
                variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimensionIds[1])).setSize(colCount);
            } else if (dimCount == 1) {
                colCount = NetCDF::getDimLength(fileId, dimensionIds[0]);
                variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimensionIds[0])).setSize(colCount);
            }

            const size_t sizeL = min(stepSize, lineCount);
            Segment* segment;
            if (context.hasSegment(segmentName)) {
                segment = &context.getSegment(segmentName);
            } else {
                segment = &context.addSegment(segmentName, sizeL, colCount, camCount, 0, lineCount - 1);
            }
            IOUtils::addVariableToSegment(symbolicName, type, *segment);
        }
    }
}

void Reader::stop(Context& context) {
    pair<string, int> fileIdPair;

    foreach(fileIdPair, fileIdMap) {
        NetCDF::closeFile(fileIdPair.second);
    }

    fileIdMap.clear();
}

void Reader::process(Context& context) {
    Dictionary& dict = *context.getDictionary();
    const vector<SegmentDescriptor*> segmentDescriptors =
            dict.getProductDescriptor(Constants::SYMBOLIC_NAME_L1C).getSegmentDescriptors();

    foreach(SegmentDescriptor* segmentDescriptor, segmentDescriptors) {
        const Segment& segment = context.getSegment(segmentDescriptor->getName());
        const Grid& grid = segment.getGrid();
        if (!context.hasMaxLComputed(segment, *this) ||
                context.getMaxLComputed(segment, *this) < grid.getStartL() + grid.getSizeL() - 1) {
            const vector<VariableDescriptor*> variableDescriptors =
                    segmentDescriptor->getVariableDescriptors();
            const size_t startL = getStartL(context, segment);
            const size_t endL = min(grid.getStartL() + grid.getSizeL() - 1, grid.getMaxL());

            foreach(VariableDescriptor* variableDescriptor, variableDescriptors) {
                const string varName = variableDescriptor->getName();
                const string ncFileName = variableDescriptor->getNcFileName();

                if (!contains(varIdMap, varName)) {
                    throw (logic_error("Unknown variable '" + varName + "'."));
                }
                if (!contains(fileIdMap, ncFileName)) {
                    throw logic_error("Unknown netCDF file '" + ncFileName + "'.");
                }
                const int varId = varIdMap[varName];
                const int fileId = fileIdMap[ncFileName];
                const size_t dimCount = variableDescriptor->getDimensions().size();
                const valarray<size_t> starts = IOUtils::createStartVector(dimCount, startL);
                const valarray<size_t> counts = IOUtils::createCountVector(dimCount, grid.getSizeK(), endL - startL + 1, grid.getSizeM());
                if (context.getLogging() != 0) {
                    context.getLogging()->progress("Reading variable " + varName + " of segment [" + segment.toString() + "]", getId());
                }
                const Accessor& accessor = segment.getAccessor(varName);
                NetCDF::getData(fileId, varId, starts, counts, accessor.getUntypedData());
            }
            context.setMaxLComputed(segment, *this, endL);
        }
    }
}

const int Reader::findFile(const string& sourceDir, const string& fileName) {
    vector<string> fileNames = IOUtils::getFiles(sourceDir);
    if (fileIdMap.find(fileName) != fileIdMap.end()) {
        return fileIdMap[fileName];
    }

    for (size_t i = 0; i < fileNames.size(); i++) {
        string currentFileName = sourceDir + "/" + fileNames[i];
        if (boost::ends_with(currentFileName, ".nc")) {
            if (boost::ends_with(currentFileName, fileName + ".nc") ||
                    boost::ends_with(currentFileName, fileName)) {

                int fileId = NetCDF::openFile(currentFileName.c_str());
                fileIdMap[fileName] = fileId;
                return fileId;
            }
        }
    }
    throw std::runtime_error("No file with name " + fileName + " found.");
}
