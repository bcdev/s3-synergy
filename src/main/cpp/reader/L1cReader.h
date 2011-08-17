/*
 * File:   L1cReader.h
 * Author: thomass
 *
 * Created on November 18, 2010, 2:08 PM
 */

#ifndef L1CREADER_H
#define	L1CREADER_H

#include <map>
#include <netcdf.h>

#include "../core/BasicModule.h"

using std::map;

class L1cReader : public BasicModule {
public:
    L1cReader();
    virtual ~L1cReader();

    void start(Context& context);
    void stop(Context& context);
    void process(Context& context);

private:
    int getNcFile(const string& ncFileBasename);

    path sourceDirPath;
    size_t segmentLineCount;

    map<string, int> ncFileIdMap;
    map<string, int> ncVarIdMap;
};

#endif	/* L1CREADER_H */
