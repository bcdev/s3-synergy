/* 
 * File:   MockReader.h
 * Author: thomass
 *
 * Created on November 18, 2010, 2:08 PM
 */

#ifndef MOCKREADER_H
#define	MOCKREADER_H

#include "AbstractModule.h"
#include "SegmentImpl.h"

class MockReader : public AbstractModule {
public:
    MockReader(size_t lineCount = 10000, size_t stepSize = 1000);
    virtual ~MockReader();

    Segment* processSegment(ProcessorContext& context);

private:
    const size_t lineCount;
    const size_t stepSize;
    SegmentImpl* segment;
};

#endif	/* MOCKREADER_H */

