/* 
 * File:   SynL2Writer.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:19 PM
 */

#ifndef SYNL2WRITER_H
#define	SYNL2WRITER_H

#include <netcdf.h>

#include "../core/DefaultModule.h"

using std::set;

class SynL2Writer : public DefaultModule {
public:
    SynL2Writer();
    ~SynL2Writer();
    void process(Context& context);
    void start(Context& context);

private:
    const int getNcId(string fileName);
    vector<string> variablesToWrite;
    set<string> ncFiles;
};

#endif	/* SYNL2WRITER_H */

