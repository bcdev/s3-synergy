/*
 * File:   Reader.h
 * Author: thomass
 *
 * Created on November 18, 2010, 2:08 PM
 */

#ifndef READER_H
#define	READER_H

#include <netcdfcpp.h>

#include "../core/Context.h"
#include "../core/DefaultModule.h"

class Reader : public DefaultModule {
public:
    Reader();
    virtual ~Reader();
    void process(Context& context);
    void readData(NcFile* dataFile, size_t lines, size_t camCount, size_t colCount, string varName, short* data);

private:
    const size_t stepSize;
    string getFileName(Context& context, string processorName);
};

#endif	/* READER_H */