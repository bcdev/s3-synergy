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
    MockReader(size_t lineCount = 60000);
    virtual ~MockReader();

    Segment* processSegment(ProcessorContext& context);

private:
    const size_t lineCount;
    
    SegmentImpl* segment;
};

#endif	/* MOCKREADER_H */

