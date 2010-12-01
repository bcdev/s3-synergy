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

class SynL2Writer : public AbstractModule {
public:
    SynL2Writer();
    ~SynL2Writer();

    Segment* processSegment(ProcessorContext& context);

private:
    NcFile* getDataFile(string variableName);
    NcFile* getDataFileByFileName(string fileName);
    map<string, NcFile*> ncFileMap;
};

#endif	/* SYNL2WRITER_H */

