/*
 * LookupTableReader.cpp
 *
 *  Created on: 12.10.2011
 *      Author: thomasstorm
 */


#include "LookupTableReader.h"

LookupTableReader::LookupTableReader(const string& id, const string& path) : id(id) {
    fileId = NetCDF::openFileReadOnly(path);
}

LookupTableReader::~LookupTableReader() {
    if (fileId != 0) {
        try {
            NetCDF::closeFile(fileId);
        } catch (std::exception& ignored) {
            // ok
        }
    }
}

