/*
 * File:   Writer.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 3:40 PM
 */

#include <boost/algorithm/string/predicate.hpp>
#include <stdexcept>

#include "SynL2Writer.h"

using std::invalid_argument;

SynL2Writer::SynL2Writer() : AbstractModule("SYN_L2_WRITER") {
}

SynL2Writer::~SynL2Writer() {

}

Segment* SynL2Writer::processSegment(ProcessorContext& context) {

    // TODO - for all variables to be written, do:

    string variableName = "SYN_COLLOCATED";
    const NcFile* dataFile = getDataFile(variableName);
    if( dataFile->get_var(variableName.c_str()) == 0 ) {
//        dataFile.add_var(variableName, );
    }

    Segment& segment = context.getSegment(variableName);
    for (size_t l = context.getMaxLineComputed(segment, *this); l < segment.getMaxL(); l++) {
        for (size_t k = segment.getMinK(); k < segment.getMaxK(); k++) {
            for (size_t m = segment.getMinM(); m < segment.getMaxM(); m++) {
                int sample = segment.getSampleInt(variableName, segment.computePosition(k, l, m));
            }
        }
    }
    return &segment;
}

NcFile* SynL2Writer::getDataFile(string variableName) {
    if (boost::starts_with(variableName, "SDR_")) {
        return getDataFileByFileName("l2_syn_surface_direction_reflectance");
    }
    if (variableName.compare("SDR_2") == 0) {
        return getDataFileByFileName("l2_syn_surface_direction_reflectance");
    }
    throw invalid_argument( "no variable with name " + variableName + " allowed." );
}

NcFile* SynL2Writer::getDataFileByFileName(string fileName) {
    if (ncFileMap[fileName] == 0) {
        NcFile* dataFile = new NcFile(fileName.c_str(), NcFile::Replace, 0, 0, NcFile::Netcdf4);
        ncFileMap[fileName] = dataFile;
        return dataFile;
    } else {
        return ncFileMap[fileName];
    }
}