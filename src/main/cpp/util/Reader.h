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

private:
    void modifySegmentBounds(const Context& context, Segment& segment);
    const int findFile(vector<string> fileNames, string& fileName);
    const nc_type addDimsAndTypeToVariable(int ncId, int varId, string& symbolicName, Dictionary& dict);
    const size_t stepSize;
};

#endif	/* READER_H */