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
#include "WriterUtils.h"

using std::map;

class SynL2Writer : public DefaultModule {
public:
    SynL2Writer();
    ~SynL2Writer();
//    void process(Context& context);
//    void start(Context& context);
    void setWriterUtils(WriterUtils writerUtils);

private:
//    NcFile* getNcFile(string fileName);
//    const NcDim** createNcDims( NcFile* dataFile, vector<Dimension*> dims );
//    NcVar* getNcVar(NcFile* dataFile, Variable* var, Dictionary& dict);
    static const string OLC_TOA_RADIANCE_MEAS_1; // to be deleted
    vector<string> variablesToWrite;
    WriterUtils writerUtils;
};

#endif	/* SYNL2WRITER_H */

