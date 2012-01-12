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
	AbstractWriter(const string& name);
	virtual ~AbstractWriter();
	void process(Context& context);
	void start(Context& context);
	void stop(Context& context);

protected:
    path targetDirPath;

    map<string, int> ncFileIdMap;
    map<string, valarray<int> > ncDimIdMap;
    map<string, int> ncVarIdMap;

	virtual const string getProductDescriptorIdentifier() const = 0;
	virtual void createSafeProduct(const Context& context) = 0;

private:
	friend class AbstractWriterTest;
	const ProductDescriptor& getProductDescriptor(const Context& context) const;
	const vector<SegmentDescriptor*> getSegmentDescriptors(const Context& context) const;
	void createNcVar(const ProductDescriptor& productDescriptor,
			const SegmentDescriptor& segmentDescriptor,
			const VariableDescriptor& variable, const Grid& grid);
	void putAttributes(int fileId, const VariableDescriptor& variableDescriptor, const vector<Attribute*>& attributes) const;
};

#endif	/* ABSTRACTWRITER_H */

