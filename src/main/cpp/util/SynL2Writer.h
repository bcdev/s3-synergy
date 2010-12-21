/* 
 * File:   SynL2Writer.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:19 PM
 */

#ifndef SYNL2WRITER_H
#define	SYNL2WRITER_H

#include <netcdfcpp.h>

#include "AbstractModule.h"

using std::map;

class SynL2Writer : public AbstractModule {
public:
    SynL2Writer();
    ~SynL2Writer();

    Segment* processSegment(ProcessorContext& context);

private:
    NcFile* getNcFile(string variableName);
    NcFile* getNcFileByFileName(string fileName);
    map<string, NcFile*> ncFileMap;
    map<Variable*, NcVar*> addedVariables;
    const NcDim** createNcDims( NcFile* dataFile, vector<Dimension*> dims );
    NcVar* getNcVar(NcFile* dataFile, Variable* var);
};

#endif	/* SYNL2WRITER_H */

