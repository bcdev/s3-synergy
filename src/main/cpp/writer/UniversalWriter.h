/*
 * File:   UniversalWriter.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:19 PM
 */

#ifndef UNIVERSALWRITER_H
#define	UNIVERSALWRITER_H

#include <valarray>

#include "../core/Writer.h"

using std::map;

class UniversalWriter: public Writer {
public:
	UniversalWriter();
	virtual ~UniversalWriter();
	void process(Context& context);
	void start(Context& context);
	void stop(Context& context);

private:

	void createNcVar(const ProductDescriptor& productDescriptor,
			const SegmentDescriptor& segmentDescriptor,
			const VariableDescriptor& variable, const Grid& grid);

	path targetDirPath;

	map<string, int> ncFileIdMap;
	map<string, valarray<int> > ncDimIdMap;
	map<string, int> ncVarIdMap;
};

#endif	/* UNIVERSALWRITER_H */

