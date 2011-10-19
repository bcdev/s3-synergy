/*
 * File:   SynL1Reader.h
 * Author: thomass
 *
 * Created on November 18, 2010, 2:08 PM
 */

#ifndef SYNL1READER_H
#define	SYNL1READER_H

#include <map>
#include <netcdf.h>

#include "../modules/BasicModule.h"

using std::map;

class SynL1Reader : public BasicModule {
public:
    SynL1Reader();
    virtual ~SynL1Reader();

    void start(Context& context);
    void stop(Context& context);
    void process(Context& context);

private:
    int getNcFile(const string& ncFileBasename);

    path sourceDirPath;
    long segmentLineCount;

    map<string, int> ncFileIdMap;
    map<string, int> ncVarIdMap;
};

#endif	/* SYNL1READER_H */
