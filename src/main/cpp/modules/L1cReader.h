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

#include "../core/DefaultModule.h"

using std::map;

class L1cReader : public DefaultModule {
public:
    L1cReader(const string& sourceDirPath, size_t chunkSize = 1000);
    virtual ~L1cReader();
    void start(Context& context);
    void stop(Context& context);
    void process(Context& context);

private:
    int openNcFile(const string& sourceDirPath, const string& fileName);

    const path sourceDirPath;
    const size_t chunkSize;

    map<string, int> ncFileIdMap;
    map<string, int> ncVarIdMap;
};

#endif	/* L1CREADER_H */