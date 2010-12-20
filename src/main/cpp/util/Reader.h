/*
 * File:   Reader.h
 * Author: thomass
 *
 * Created on November 18, 2010, 2:08 PM
 */

#ifndef READER_H
#define	READER_H

#include <netcdfcpp.h>

#include "AbstractModule.h"
#include "../core/SegmentImpl.h"

class Reader : public AbstractModule {
public:
    Reader();
    virtual ~Reader();
    Segment* processSegment(ProcessorContext& context);
    void* readData(NcFile* dataFile, size_t camCount, size_t colCount, string varName);

private:
    const size_t stepSize;
    SegmentImpl* segment;
    string getFileName(ProcessorContext& context, string processorName);
};

#endif	/* READER_H */