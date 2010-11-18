/* 
 * File:   MockReader.h
 * Author: thomass
 *
 * Created on November 18, 2010, 2:08 PM
 */

#ifndef MOCKREADER_H
#define	MOCKREADER_H

#include <cstddef>

#include "Reader.h"
#include "SegmentImpl.h"

class MockReader : public Reader {
public:
    MockReader(size_t segmentCount, size_t k, size_t l, size_t m);
    virtual ~MockReader();
    Segment* getNextSegment();
private:
    size_t segmentCount;
    size_t k;
    size_t l;
    size_t m;
    SegmentImpl* previous;
};

#endif	/* MOCKREADER_H */

