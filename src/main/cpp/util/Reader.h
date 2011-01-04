/*
 * File:   Reader.h
 * Author: thomass
 *
 * Created on November 18, 2010, 2:08 PM
 */

#ifndef READER_H
#define	READER_H

#include <netcdf.h>

#include "../core/Context.h"
#include "../core/DefaultModule.h"

class Reader : public DefaultModule {
public:
    Reader();
    virtual ~Reader();
    void process(Context& context);
//    void readData(NcFile* dataFile, const size_t lines, const size_t camCount,
//            const size_t colCount, const string& symbolicName, Segment& segment,
//            Dictionary dict, size_t index);

private:
    void modifySegmentBounds(const Context& context, Segment& segment);
    void addVariableToSegment(const string variable, Segment& segment, Dictionary dict);
    const size_t stepSize;
};

#endif	/* READER_H */