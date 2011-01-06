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

class OlciGridReader : public DefaultModule {
public:
    OlciGridReader();
    virtual ~OlciGridReader();
    void process(Context& context);

private:
    void modifySegmentBounds(const Context& context, Segment& segment);
    const int findFile(string& sourceDir, string& fileName);
    const nc_type addTypeToVariable(int ncId, int varId, Variable& variable);
    void addDimsToVariable(Variable& variable, size_t camCount, size_t lineCount, size_t colCount);
    const size_t stepSize;
};

#endif	/* READER_H */