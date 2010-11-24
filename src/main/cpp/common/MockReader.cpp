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

MockReader::MockReader(size_t l) : AbstractModule("READ"), lineCount(l) {
    this->segment = 0;
}

MockReader::~MockReader() {
    if (segment != 0) {

    }
}

Segment* MockReader::processSegment(ProcessorContext& context) {
    if (context.containsSegment("SYN_COLLOCATED")) {
        // TODO - modify segment
    } else {
        // TODO - create segment, see below
    }
    /*
    if (maxL < minL) {
        return 0;
    }
    if (minL < this->lineCount) {
        if (segment == 0) {
            segment = new SegmentImpl("SYN_COLLOCATED", minL, min(maxL, minL + this->lineCount - 1));
            // TODO - create variable etc.
        }
        // TODO - modify segment
        return segment;
    } else {
        return 0;
    }
     */
}
