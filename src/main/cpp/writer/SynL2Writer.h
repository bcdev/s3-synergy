/*
 * File:   SynL2Writer.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:19 PM
 */

#ifndef SYNL2WRITER_H
#define	SYNL2WRITER_H

#include <map>
#include <valarray>

#include "../modules/BasicModule.h"

using std::map;
using std::valarray;

class SynL2Writer: public BasicModule {
public:
	SynL2Writer();
	virtual ~SynL2Writer();
	void process(Context& context);
	void start(Context& context);
	void stop(Context& context);

private:
	friend class SynL2WriterTest;
	void copyTemplateFiles() const;
	string readManifest() const;
	void setStartTime(Context& context, string& xml) const;
	void setChecksums(string& manifest) const;
	void writeManifest(string& manifest) const;
	void replaceString(const string& toReplace, const string& replacement, string& input) const;
	string getMd5Sum(const string& file) const;
	void createNcVar(const ProductDescriptor& productDescriptor,
			const SegmentDescriptor& segmentDescriptor,
			const VariableDescriptor& variable, const Grid& grid);

	path targetDirPath;

	map<string, int> ncFileIdMap;
	map<string, valarray<int> > ncDimIdMap;
	map<string, int> ncVarIdMap;
};

#endif	/* SYNL2WRITER_H */

