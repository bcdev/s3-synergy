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

#include "../writer/AbstractWriter.h"

using std::map;
using std::valarray;

class SynL2Writer: public AbstractWriter {
public:
    SynL2Writer();
    virtual ~SynL2Writer();

protected:
	const string& getProductId() const;
	const vector<SegmentDescriptor*> getSegmentDescriptors(const Dictionary& dict) const;
	const string& getSafeManifestName() const;
	void writeCoordinateVariables(Context& context);
    void defineCoordinateDimensions(int fileId, const string& segmentName, const Dictionary& dict, map<const VariableDescriptor*, valarray<int> >& commonDimIds);
    void defineCoordinateVariables(int fileId, const string& segmentName, const Dictionary& dict, const map<const VariableDescriptor*, valarray<int> >& commonDimIds);

private:
	friend class SynL2WriterTest;
};

#endif	/* SYNL2WRITER_H */

