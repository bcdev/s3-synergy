/* 
 * File:   SynL2Writer.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:19 PM
 */

#ifndef SYNL2WRITER_H
#define	SYNL2WRITER_H

#include <netcdfcpp.h>

#include "../core/DefaultModule.h"

using std::map;

class SynL2Writer : public DefaultModule {
public:
    SynL2Writer();
    ~SynL2Writer();
    void process(Context& context);
    void start(Context& context);

private:
    NcFile* getNcFile(string fileName);
    map<string, NcFile*> ncFileMap;
    map<Variable*, NcVar*> addedVariables;
    const NcDim** createNcDims( NcFile* dataFile, vector<Dimension*> dims );
    NcVar* getNcVar(NcFile* dataFile, Variable* var);
    static const string OLC_TOA_RADIANCE_MEAS_1; // to be deleted
    vector<string> variablesToWrite;
};

#endif	/* SYNL2WRITER_H */

