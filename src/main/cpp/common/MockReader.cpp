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
                segment = new SegmentImpl("SYN_COLLOCATED", minL, min(maxL, minL + lineCount - 1));
                Logger::get()->progress("Reading data for segment [" + segment->toString() + "]", "MockReader");

                // TODO - for all variables do {

                // creating variables
                const size_t columnCount = 760;
                const size_t camCount = 5;
                segment->addIntVariable("F_OLC");
                segment->addIntVariable("F_SLN");
                segment->addIntVariable("F_SLO");

                // TODO - fill segment with correct data
                const size_t valueCount = segment->getMaxL() * columnCount * camCount;
                int* olciFlagValues = new int[valueCount];
                for (size_t i = 0; i < valueCount; i++) {
                    olciFlagValues[i] = 0x0010;
                }

                segment->setSamplesInt("F_OLC", olciFlagValues);

                // }
            }
            context.setMaxLine(*segment, lineCount);
            context.setMaxLineComputed(*segment, *this, segment->getMaxL());
            return segment;
        } else {
            return 0;
        }
    } else {
        // modifying segment bounds
        Segment& segment = context.getSegment("SYN_COLLOCATED");
        size_t minRequiredLine = context.getMinLineRequired(segment);
        segment.setMinL(minRequiredLine);
        segment.setMaxL(min(minRequiredLine + stepSize, lineCount));

        // modifying segment values
        Logger::get()->progress("Reading data for segment [" + segment.toString() + "]", "MockReader");
        size_t startLine = context.getMaxLineComputed(segment, *this);
        size_t endLine = segment.getMaxL();
        for (size_t l = startLine; l < endLine; l++) {
            for (size_t m = segment.getMinM(); m < segment.getMaxM(); m++) {
                for (size_t k = segment.getMinK(); k < segment.getMaxK(); k++) {
                    //                    TODO - read values
                    //                    segment.setSampleInt();
                }
            }
        }

        context.setMaxLineComputed(segment, *this, segment.getMaxL());
        return &context.getSegment("SYN_COLLOCATED");
    }
}
