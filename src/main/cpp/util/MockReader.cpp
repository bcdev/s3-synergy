/* 
 * File:   MockReader.cpp
 * Author: thomass
 * 
 * Created on November 18, 2010, 2:08 PM
 */

#include <algorithm>

#include "MockReader.h"
#include "SegmentImpl.h"
#include "ProcessorContext.h"

using std::min;

MockReader::MockReader(size_t lineCount, size_t stepSize) : AbstractModule("READ"), lineCount(lineCount), stepSize(stepSize) {
    this->segment = 0;
}

MockReader::~MockReader() {
}

Segment* MockReader::processSegment(ProcessorContext& context) {
    if (!context.containsSegment("SYN_COLLOCATED")) {
        // initial reading
        size_t minL = 0;
        size_t maxL = stepSize;

        if (minL > maxL) {
            return 0;
        }
        if (minL < this->lineCount) {
            if (segment == 0) {
                segment = new SegmentImpl("SYN_COLLOCATED", minL, min(maxL - 1, minL + lineCount - 1));
                Logger::get()->progress("Reading data for segment [" + segment->toString() + "]", "MockReader");

                // TODO - for all variables do {

                // creating variables
                const size_t valueCount = segment->getValueCount();
                segment->addVariableInt("F_OLC", valueCount);
                segment->addVariableInt("F_SLN", valueCount);
                segment->addVariableInt("F_SLO", valueCount);

                // TODO - fill segment with correct data
                for (size_t i = 0; i < valueCount; i++) {
                    segment->setSampleInt("F_OLC", i, 0x0010);
                }

                // }
            }
            context.setMaxLine(*segment, lineCount - 1);
            context.setMaxLineComputed(*segment, *this, segment->getRowCount());
            return segment;
        } else {
            return 0;
        }
    } else {
        // modifying segment bounds
        Segment& segment = context.getSegment("SYN_COLLOCATED");
        size_t minRequiredLine = context.getMinLineRequired(segment);
        segment.setMinL(minRequiredLine);
        segment.setMaxL(min(minRequiredLine + stepSize - 1, lineCount - 1));

        // modifying segment values
        Logger::get()->progress("Reading data for segment [" + segment.toString() + "]", "MockReader");
        size_t startLine = context.getMaxLineComputed(segment, *this);
        size_t endLine = segment.getRowCount();
        for (size_t l = startLine; l < endLine; l++) {
            for (size_t m = segment.getM(); m < segment.getColCount(); m++) {
                for (size_t k = segment.getMinK(); k < segment.getCamCount(); k++) {
                    //                    TODO - read values
                    //                    segment.setSampleInt();
                }
            }
        }

        context.setMaxLineComputed(segment, *this, segment.getRowCount());
        return &context.getSegment("SYN_COLLOCATED");
    }
}
