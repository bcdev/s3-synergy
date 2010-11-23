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
    MockReader(size_t lineCount);
    virtual ~MockReader();
    
    Segment* readSegment(size_t minL, size_t maxL);

private:
    const size_t lineCount;
    
    SegmentImpl* segment;
};

#endif	/* MOCKREADER_H */

