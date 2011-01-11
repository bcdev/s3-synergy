/*
 * File:   Reader.h
 * Author: thomass
 *
 * Created on November 18, 2010, 2:08 PM
 */

#ifndef READER_H
#define	READER_H

#include <map>
#include <netcdf.h>

#include "../core/Context.h"
#include "../core/DefaultModule.h"

using std::map;

class Reader : public DefaultModule {
public:
    Reader();
    virtual ~Reader();
    void process(Context& context);

private:
    void modifyBoundsOfSegments(const Context& context);
    const int findFile(string& sourceDir, string& fileName);
    const void setVariableType(int ncId, int varId, Variable& variable);
    const nc_type getVariableType(int ncId, int varId, Variable& variable);
    void addDimsToVariable(Variable& variable, size_t camCount, size_t lineCount, size_t colCount);
    const bool segmentIsCompletelyComputed(const string& segmentName, const string& symbolicName) const;
    const bool segmentIsCompletelyComputed(const string& segmentName, const Dictionary& dict) const;
    map<string, int> openedFiles;
    map<string, string> completedSegments;
    const size_t stepSize;
    const Grid* olciGrid;
};

#endif	/* READER_H */