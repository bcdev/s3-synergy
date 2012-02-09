/*
 * File:   SynL1Reader.h
 * Author: thomass
 *
 * Created on November 18, 2010, 2:08 PM
 */

#ifndef SYNL1READER_H
#define	SYNL1READER_H

#include <map>

#include "../modules/BasicModule.h"

using std::map;

class SynL1Reader : public BasicModule {
public:
    SynL1Reader();
    virtual ~SynL1Reader();

    void process(Context& context);
    void start(Context& context);
    void stop(Context& context);

private:
    int getNcFile(const string& ncFileBasename);
    long getSegmentSize(const string& segmentName, const long rowCount) const;
    void addDimensionToDictionary(int fileId, int dimId, VariableDescriptor& variableDescriptor, size_t dimSize) const;
    path sourceDirPath;
    long segmentLineCount;

    map<string, int> ncFileIdMap;
    map<string, int> ncVarIdMap;

    vector<string> addedSegments;
};

#endif	/* SYNL1READER_H */
