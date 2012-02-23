/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

#ifndef SYNL1READER_H
#define	SYNL1READER_H

#include <map>

#include "../modules/BasicModule.h"

using std::map;

using boost::filesystem::path;

/**
 * The SYN L1 reader module.
 */
class SynL1Reader : public BasicModule {
public:
    SynL1Reader();
    virtual ~SynL1Reader();

    void process(Context& context);
    void start(Context& context);
    void stop(Context& context);

private:
    int getNcFile(const string& ncFileBasename);
    long getSegmentSize(const string& segmentName, const long rowCount) const;
    void addDimensionToDictionary(int fileId, int dimId, VariableDescriptor& variableDescriptor, size_t dimSize) const;
    path sourceDirPath;
    long segmentLineCount;

    map<string, int> ncFileIdMap;
    map<string, int> ncVarIdMap;

    vector<string> addedSegments;
};

#endif	/* SYNL1READER_H */
