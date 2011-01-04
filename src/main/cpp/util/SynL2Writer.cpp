/*
 * File:   SynL2Writer.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 3:40 PM
 */

#include <boost/algorithm/string/predicate.hpp>
#include <stdexcept>
#include <set>

#include "SynL2Writer.h"
#include "../core/Context.h"
#include "../util/Logger.h"

using std::invalid_argument;

SynL2Writer::SynL2Writer() : DefaultModule("SYN_L2_WRITER") {
}

SynL2Writer::~SynL2Writer() {

}

const string SynL2Writer::OLC_TOA_RADIANCE_MEAS_1 = "OLC_RADIANCE_O1_TOA_Radiance_Meas"; // to be deleted

//void SynL2Writer::process(Context& context) {
//    string segmentId = "SYN_COLLOCATED";
//    Segment& segment = context.getSegment(segmentId);
//    Logger::get()->progress("Starting to write segment [" + segment.toString() + "]", getId());
//
//    if (!segment.hasVariable("SYN_flags")) {
//        segment.addVariableInt("SYN_flags");
//    }
//
//    Grid& grid = segment.getGrid();
//
//    size_t startLine = getStartL(context, segment);
//    size_t endLine = getDefaultEndL(startLine, grid);
//    size_t lines = endLine - startLine + 1;
//
//    size_t valueCount = grid.getSizeK() * lines * grid.getSizeM();
//    int* values = new int[valueCount];
//
//    vector<string>::iterator iter;
//    for (iter = variablesToWrite.begin(); iter != variablesToWrite.end(); iter++) {
//        Variable& variable = context.getDictionary()->getVariable(*iter);
//
//        string ncName = variable.getNcName();
//        string ncFileName = context.getDictionary()->getNcFileName(ncName);
//        ncFileName.append( ".nc" );
//        NcFile* ncFile = writerUtils.getNcFile(ncFileName);
//        Accessor& accessor = context.getSegment(segmentId).getAccessor(ncName);
//        NcVar* ncVar = writerUtils.getNcVar(ncFile, &variable, *(context.getDictionary()));
//
//        // switch for all data types:
//        // for each variable do:
//        // if variable.getType == "short" do:
//
//        for (size_t k = grid.getStartK(); k < grid.getSizeK(); k++) {
//            for (size_t l = startLine; l <= endLine; l++) {
//                for (size_t m = grid.getStartM(); m < grid.getSizeM(); m++) {
//                    size_t position = segment.getGrid().getIndex(k, l, m);
//                    values[position] = accessor.getShort(position);
//                }
//            }
//        }
//
//        // else if variable.getType == "float" do:
//
//        // ... end for.
//
//        ncVar->put(values, grid.getSizeK() - grid.getStartK(), lines, grid.getSizeM() - grid.getStartM());
//        ncFile->sync();
//    }
//    delete[] values;
//    context.setMaxLComputed(segment, *this, endLine);
//}
//
//void SynL2Writer::start(Context& context) {
//    // add all variables to be written
//    variablesToWrite.push_back(OLC_TOA_RADIANCE_MEAS_1);
//    variablesToWrite.push_back("SYN_flags");
//}

void SynL2Writer::setWriterUtils(WriterUtils writerUtils) {
    this->writerUtils = writerUtils;
}