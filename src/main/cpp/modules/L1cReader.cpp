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
            const string varName = variableDescriptor->getName();
            const string ncVarName = variableDescriptor->getNcVarName();
            const string ncFileBasename = variableDescriptor->getNcFileBasename();
            const int fileId = getNcFile(ncFileBasename);
            const int varId = NetCDF::getVariableId(fileId, ncVarName.c_str());
            const int dimCount = NetCDF::getDimCountForVariable(fileId, varId);
            const valarray<int> dimIds = NetCDF::getDimIdsForVariable(fileId, varId);

            size_t camCount;
            size_t rowCount;
            size_t colCount;

            switch (dimCount) {
                case 3:
                    camCount = NetCDF::getDimLength(fileId, dimIds[0]);
                    rowCount = NetCDF::getDimLength(fileId, dimIds[1]);
                    colCount = NetCDF::getDimLength(fileId, dimIds[2]);
                    variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimIds[0])).setSize(camCount);
                    variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimIds[1])).setSize(rowCount);
                    variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimIds[2])).setSize(colCount);
                    break;
                case 2:
                    camCount = 1;
                    rowCount = NetCDF::getDimLength(fileId, dimIds[0]);
                    colCount = NetCDF::getDimLength(fileId, dimIds[1]);
                    variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimIds[0])).setSize(rowCount);
                    variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimIds[1])).setSize(colCount);
                    break;
                case 1:
                    camCount = 1;
                    rowCount = 1;
                    colCount = NetCDF::getDimLength(fileId, dimIds[0]);
                    variableDescriptor->addDimension(NetCDF::getDimName(fileId, dimIds[0])).setSize(colCount);
                    break;
                default:
                    BOOST_THROW_EXCEPTION(runtime_error("invalid number of dimensions for variable '" + ncVarName + "'."));
            }
            if (!context.hasSegment(segmentName)) {
                const size_t sizeL = min(chunkSize, rowCount);
                context.addSegment(segmentName, sizeL, colCount, camCount, 0, rowCount - 1);
            }
            const int type = NetCDF::getVariableType(fileId, varId);
            variableDescriptor->setType(type);

            IOUtils::addVariableToSegment(varName, type, context.getSegment(segmentName));
            ncVarIdMap[varName] = varId;
        }
    }
}

void L1cReader::stop(Context& context) {
    pair<string, int> fileIdPair;

    foreach(fileIdPair, ncFileIdMap) {
        NetCDF::closeFile(fileIdPair.second);
    }
    ncVarIdMap.clear();
    ncFileIdMap.clear();
}

void L1cReader::process(Context& context) {
    const Dictionary& dict = *context.getDictionary();
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
                const string ncFileName = variableDescriptor->getNcFileBasename();

                if (!contains(ncVarIdMap, varName)) {
                    BOOST_THROW_EXCEPTION(logic_error("Unknown variable '" + varName + "'."));
                }
                if (!contains(ncFileIdMap, ncFileName)) {
                    BOOST_THROW_EXCEPTION(logic_error("Unknown netCDF file '" + ncFileName + "'."));
                }
                const int varId = ncVarIdMap[varName];
                const int fileId = ncFileIdMap[ncFileName];
                const size_t dimCount = variableDescriptor->getDimensions().size();
                const valarray<size_t> starts = IOUtils::createStartVector(dimCount, startL);
                const valarray<size_t> counts = IOUtils::createCountVector(dimCount, grid.getSizeK(), endL - startL + 1, grid.getSizeM());
                context.getLogging().progress("Reading variable '" + variableDescriptor->getNcVarName() + "' into segment '" + segment.toString() + "'", getId());
                const Accessor& accessor = segment.getAccessor(varName);
                NetCDF::getData(fileId, varId, starts, counts, accessor.getUntypedData());
            }
            context.setMaxLComputed(segment, *this, endL);
        }
    }
}

int L1cReader::getNcFile(const string& ncFileBasename) {
    if (contains(ncFileIdMap, ncFileBasename)) {
        return ncFileIdMap[ncFileBasename];
    }
    const path ncFilePath = sourceDirPath / (ncFileBasename + ".nc");
    const int fileId = NetCDF::openFile(ncFilePath);
    ncFileIdMap[ncFileBasename] = fileId;

    return fileId;
}
