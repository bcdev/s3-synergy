/*
 * File:   SynL2Writer.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:19 PM
 */

#ifndef SYNL2WRITER_H
#define	SYNL2WRITER_H

#include <valarray>

#include "../core/Boost.h"
#include "../core/Writer.h"

using std::map;

class SynL2Writer : public Writer {
public:
    SynL2Writer(const string& targetDirPath);
    ~SynL2Writer();
    void process(Context& context);
    void start(Context& context);
    void stop(Context& context);

private:

    void createNcVar(const ProductDescriptor& productDescriptor,
            const SegmentDescriptor& segmentDescriptor,
            const VariableDescriptor& variable,
            const Grid& grid);

    const path targetDirPath;
    map<string, int> ncFileIdMap;
    map<string, valarray<int> > ncDimIdMap;
    map<string, int> ncVarIdMap;
};

#endif	/* SYNL2WRITER_H */

