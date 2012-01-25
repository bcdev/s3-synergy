/*
 * File:   SegmentWriter.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:19 PM
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
	void putData(int ncId, int varId, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;

	void putByteData(int ncId, int varId, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void putUByteData(int ncId, int varId, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void putShortData(int ncId, int varId, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void putUShortData(int ncId, int varId, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void putIntData(int ncId, int varId, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void putUIntData(int ncId, int varId, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void putLongData(int ncId, int varId, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void putULongData(int ncId, int varId, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void putFloatData(int ncId, int varId, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void putDoubleData(int ncId, int varId, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;

	path targetDirPath;

	map<string, int> ncFileIdMap;
	map<string, valarray<int> > ncDimIdMap;
	map<string, int> ncVarIdMap;
};

#endif	/* SEGMENTWRITER_H */

