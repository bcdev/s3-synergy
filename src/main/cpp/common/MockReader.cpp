/* 
 * File:   MockReader.cpp
 * Author: thomass
 * 
 * Created on November 18, 2010, 2:08 PM
 */

#include "MockReader.h"
#include "SegmentImpl.h"

MockReader::MockReader( size_t segmentCount, size_t k, size_t l, size_t m) : Reader() {
    this->segmentCount = segmentCount;
    this->k = k;
    this->l = l;
    this->m = m;
}

MockReader::~MockReader() {
}

Segment* MockReader::getNextSegment() {
    SegmentImpl* segment= new SegmentImpl(k, l, m);
    previous = segment;
    segmentCount--;
    return segment;
}
