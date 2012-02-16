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

#ifndef SEGMENTWRITER_H
#define	SEGMENTWRITER_H

#include <map>
#include <valarray>

#include "../modules/BasicModule.h"

using std::map;
using std::valarray;

class SegmentWriter: public BasicModule {
public:
	SegmentWriter();
	virtual ~SegmentWriter();

	void process(Context& context);
	void start(Context& context);
	void stop(Context& context);

private:

	void createNcVar(const Segment& segment, const string& varName);

	path targetDirPath;

	map<string, int> ncFileIdMap;
	map<string, valarray<int> > ncDimIdMap;
	map<string, int> ncVarIdMap;
};

#endif	/* SEGMENTWRITER_H */

