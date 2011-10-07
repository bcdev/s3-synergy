/*
 * File:   SegmentWriter.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:19 PM
 */

#ifndef SEGMENTWRITER_H
#define	SEGMENTWRITER_H

#include <valarray>

#include "../core/BasicModule.h"

using std::map;

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

