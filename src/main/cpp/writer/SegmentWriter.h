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
	void* getData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;

	void* getByteData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void* getShortData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void* getIntData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void* getLongData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void* getUByteData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void* getUShortData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void* getUIntData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void* getULongData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void* getFloatData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
	void* getDoubleData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;

	path targetDirPath;

	map<string, int> ncFileIdMap;
	map<string, valarray<int> > ncDimIdMap;
	map<string, int> ncVarIdMap;
};

#endif	/* SEGMENTWRITER_H */

