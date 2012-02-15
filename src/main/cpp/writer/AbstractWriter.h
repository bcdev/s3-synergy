/*
 * File:   SynL2Writer.h
 * Author: thomass
 *
 * Created on November 17, 2010, 5:19 PM
 */

#ifndef ABSTRACTWRITER_H
#define	ABSTRACTWRITER_H

#include <map>
#include <valarray>

#include "../modules/BasicModule.h"

using std::map;
using std::valarray;

class AbstractWriter: public BasicModule {
public:

	void process(Context& context);
	void start(Context& context);
	void stop(Context& context);

protected:
	AbstractWriter(const string& name, const string& productId);
	virtual ~AbstractWriter();

	virtual vector<SegmentDescriptor*> getSegmentDescriptors(const ProductDescriptor& productDescriptor) const {
	    return productDescriptor.getSegmentDescriptors();
	}

	virtual void defineCoordinateVariables(const Context& context, int fileId, const ProductDescriptor& productDescriptor, const string& segmentName) const {
	}

	virtual void writeCoordinateVariables(Context& context, int fileId, const ProductDescriptor& productDescriptor, const string& segmentName) const {
	}

private:
	friend class AbstractWriterTest;

	void defineVariable(const Context& context, const ProductDescriptor& productDescriptor, const SegmentDescriptor& segmentDescriptor,
	        const VariableDescriptor& variable, const Grid& grid);
	void putGlobalAttributes(int fileId, const string& ncFileBasename, const vector<Attribute*>& attributes) const;

	const string productId;

    path targetDirPath;
    map<string, int> ncFileIdMap;
    map<string, valarray<int> > ncDimIdMap;
    map<string, int> ncVarIdMap;
};

#endif	/* ABSTRACTWRITER_H */

