/* 
 * File:   Reader.cpp
 * Author: thomass
 * 
 * Created on November 18, 2010, 2:08 PM
 */

#include <algorithm>
#include <boost/algorithm/string/predicate.hpp>
#include <vector>
#include <valarray>

#include "Reader.h"
#include "ProcessorContext.h"

using std::min;

Reader::Reader() : AbstractModule("READ"), stepSize(1000) {
    this->segment = 0;
}

Reader::~Reader() {
}

Segment* Reader::processSegment(ProcessorContext& context) {
    string fileName = getFileName(context, "SYL2");
    NcFile* dataFile = new NcFile(fileName.c_str(), NcFile::ReadOnly);
    size_t camCount = dataFile->get_dim("N_CAM")->size();
    size_t colCount = dataFile->get_dim("N_DET_CAM")->size();
    size_t lineCount = dataFile->get_dim("N_LINE_OLC")->size();

    if (!context.containsSegment("SYN_COLLOCATED")) {
        // initial reading

        size_t minL = 0;
        size_t maxL = stepSize;

        if (minL > maxL) {
            return 0;
        }
        if (minL < lineCount) {
            if (segment == 0) {
                segment = new SegmentImpl("SYN_COLLOCATED", min(maxL - 1, minL + lineCount - 1));
                Logger::get()->progress("Reading data for segment [" + segment->toString() + "]", "Reader");

                // TODO - for all variables do {

                // creating variables
                segment->addVariableInt("TOA_Radiance_Meas_1");
                segment->addVariableInt("error_estimates");

                short* data = (short*) readData(dataFile, camCount, colCount, "TOA_Radiance_Meas");

                Accessor& accessor = segment->getAccessor("TOA_Radiance_Meas_1");
                for (size_t cam = 0; cam < camCount; cam++) {
                    for (size_t line = 0; line < stepSize; line++) {
                        for (size_t col = 0; col < colCount; col++) {
                            //                            accessor.setShort(segment->getGrid().getIndex(cam, line, col), data[cam][line][col]);
                        }
                    }
                }

                // }
            }
            context.setMaxLine(*segment, lineCount - 1);
            context.setMaxLineComputed(*segment, *this, segment->getGrid().getSizeL());
            return segment;
        } else {
            return 0;
        }
    } else {
        // modifying segment bounds
        Segment& segment = context.getSegment("SYN_COLLOCATED");
        size_t minRequiredLine = context.getMinLineRequired(segment);
        Grid& grid = segment.getGrid();
        grid.setStartL(minRequiredLine);
        grid.setSizeL((min(minRequiredLine + stepSize - 1, lineCount - 1)));

        // modifying segment values
        Logger::get()->progress("Reading data for segment [" + segment.toString() + "]", "Reader");
        size_t startLine = context.getMaxLineComputed(segment, *this);
        size_t endLine = grid.getSizeL();

        size_t lines = endLine - startLine;
        short* data = new short[lines * camCount * colCount];
        dataFile->get_var("TOA_radiance_meas_1")->get(data, camCount, lines, colCount);

        for (size_t cam = grid.getStartK(); cam < grid.getSizeK(); cam++) {
            for (size_t line = startLine; line < endLine; line++) {
                for (size_t col = grid.getStartM(); col < grid.getSizeM(); col++) {
                    Accessor& accessor = segment.getAccessor("TOA_radiance_meas_1");
                    //                    accessor.setShort(segment.getGrid().getIndex(cam, line, col), data[cam][line][col]);
                }
            }
        }

        context.setMaxLineComputed(segment, *this, segment.getGrid().getSizeL());
        return &context.getSegment("SYN_COLLOCATED");
    }
}

string Reader::getFileName(ProcessorContext& context, string processorId) {
    vector<string> inputFileNames = context.getJobOrder().getProcessorConfiguration(processorId).getInputList()[0]->getFileNames();
    for (size_t i = 0; i < inputFileNames.size(); i++) {
        if (boost::contains(inputFileNames[i], "OLC_RADIANCE_O1")) {
            return inputFileNames[i];
        }
    }
}

void* Reader::readData(NcFile* dataFile, size_t camCount, size_t colCount, string varName) {
    short* data = new short[stepSize * camCount * colCount];
    NcVar* var = dataFile->get_var(varName.c_str());
    var->get(data, camCount, stepSize, colCount);
    return data;
}
