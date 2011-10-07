/*
 * File:   SynL2Writer.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:19 PM
 */

#ifndef SYNL2WRITER_H
#define	SYNL2WRITER_H

#include <valarray>

#include "../core/BasicModule.h"

using std::map;

class SynL2Writer: public BasicModule {
public:
	SynL2Writer();
	virtual ~SynL2Writer();
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

#endif	/* SYNL2WRITER_H */

