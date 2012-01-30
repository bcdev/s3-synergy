/*
 * File:   VgtPWriter.h
 * Author: thomass
 *
* Created on January 11, 2012, 16:30
 */

#ifndef VGTPWRITER_H
#define	VGTPWRITER_H

#include "../writer/AbstractWriter.h"

class VgtPWriter: public AbstractWriter {
public:
    VgtPWriter();
    virtual ~VgtPWriter();

protected:
    const string& getProductDescriptorIdentifier() const;
    const vector<SegmentDescriptor*> getSegmentDescriptors(const Dictionary& dict) const;
    const string& getSafeManifestName() const;
    void writeCommonVariables(Context& context);
    void defineCommonDimensions(int fileId, const string& segmentName, const Dictionary& dict, map<const VariableDescriptor*, valarray<int> >& commonDimIds);
    void defineCommonVariables(int fileId, const string& segmentName, const Dictionary& dict, const map<const VariableDescriptor*, valarray<int> >& commonDimIds);

private:
	friend class VgtPWriterTest;

	const vector<SegmentDescriptor*> getCommonSegments(const Dictionary& dict) const;
	const vector<VariableDescriptor*> getSubsampledCommonVariables(const Dictionary& dict) const;
	const vector<VariableDescriptor*> getNonSubsampledCommonVariables(const Dictionary& dict) const;
	bool isCommonDescriptor(const SegmentDescriptor& segmentDescriptor) const;
	bool isSubsampledCommonSegment(const string& segmentName) const;
	bool isSubsampledSegment(const string& segmentName) const;
	valarray<int> getFileIds();
};

#endif	/* VGTPWRITER_H */

