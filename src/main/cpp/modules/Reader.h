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
    void start(Context& context);
    void stop(Context& context);
    void process(Context& context);

private:
    void modifyBoundsOfSegments(const Context& context);
    const int findFile(const string& sourceDir, const string& fileName);
    const size_t stepSize;
    map<string, int> fileIdMap;
    map<string, int> varIdMap;
};

#endif	/* READER_H */