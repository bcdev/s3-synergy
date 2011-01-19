/*
 * File:   L1cReader.cpp
 * Author: thomass
 *
 * Created on November 18, 2010, 2:08 PM
 */

#include "../util/IOUtils.h"
#include "../util/NetCDF.h"
#include "L1cReader.h"

using std::min;
using std::max;

L1cReader::L1cReader(const string& sourceDirPath, size_t chunkSize) : DefaultModule("L1C_READER"), sourceDirPath(sourceDirPath), chunkSize(chunkSize) {
}

L1cReader::~L1cReader() {
}

void L1cReader::start(Context& context) {
    const Dictionary& dict = *context.getDictionary();
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

            int fileId = findFile(sourceDirPath.string(), fileName);
            int varId = NetCDF::getVariableId(fileId, ncVariableName.c_str());
            const int type = NetCDF::getVariableType(fileId, varId);
            variableDescriptor->setType(type);
            varIdMap[symbolicName] = varId;
            int dimCount = NetCDF::getDimCountForVariable(fileId, varId);
            valarray<int> dimensionIds = NetCDF::getDimIdsForVariable(fileId, varId);

            // getting the dimension sizes
            size_t camCount = 1;
            size_t rowCount = 1;
            size_t colCount = 1;

            if (dimCount == 3) {
                camCount = NetCDF::getDimLength(fileId, dimensionIds[0]);
                rowCount = NetCDF::getDimLength(fileId, dimensionIds[1]);
                colCount = NetCDF::getDimLength(fileId, dimensionIds[2]);
                variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimensionIds[0])).setSize(camCount);
                variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimensionIds[1])).setSize(rowCount);
                variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimensionIds[2])).setSize(colCount);
            } else if (dimCount == 2) {
                rowCount = NetCDF::getDimLength(fileId, dimensionIds[0]);
                colCount = NetCDF::getDimLength(fileId, dimensionIds[1]);
                variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimensionIds[0])).setSize(rowCount);
                variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimensionIds[1])).setSize(colCount);
            } else if (dimCount == 1) {
                colCount = NetCDF::getDimLength(fileId, dimensionIds[0]);
                variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimensionIds[0])).setSize(colCount);
            }

            const size_t sizeL = min(chunkSize, rowCount);
            Segment* segment;
            if (context.hasSegment(segmentName)) {
                segment = &context.getSegment(segmentName);
            } else {
                segment = &context.addSegment(segmentName, sizeL, colCount, camCount, 0, rowCount - 1);
            }
            IOUtils::addVariableToSegment(symbolicName, type, *segment);
        }
    }
}

void L1cReader::stop(Context& context) {
    pair<string, int> fileIdPair;

    foreach(fileIdPair, fileIdMap) {
        NetCDF::closeFile(fileIdPair.second);
    }

    fileIdMap.clear();
}

void L1cReader::process(Context& context) {
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

int L1cReader::findFile(const string& sourceDirPath, const string& fileName) {
    vector<string> fileNames = IOUtils::getFiles(sourceDirPath);
    if (fileIdMap.find(fileName) != fileIdMap.end()) {
        return fileIdMap[fileName];
    }

    for (size_t i = 0; i < fileNames.size(); i++) {
        string currentFileName = sourceDirPath + "/" + fileNames[i];
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
