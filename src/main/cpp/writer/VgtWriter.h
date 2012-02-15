/*
 * File:   VgtWriter.h
 * Author: thomass
 *
* Created on January 11, 2012, 16:30
 */

#ifndef VGTWRITER_H
#define	VGTWRITER_H

#include "../writer/AbstractWriter.h"

class VgtWriter: public AbstractWriter {
public:
    VgtWriter(const string& productId = Constants::PRODUCT_VGP, const string safeManifestName = Constants::SAFE_MANIFEST_NAME_VGP);
    virtual ~VgtWriter();

protected:
    const string& getProductId() const;
    const string& getSafeManifestName() const;
    const vector<SegmentDescriptor*> getSegmentDescriptors(const Dictionary& dict) const;

    void writeCoordinateVariables(Context& context);
    void defineCoordinateDimensions(int fileId, const string& segmentName, const Dictionary& dict, map<const VariableDescriptor*, valarray<int> >& coordinateDimIds);
    void defineCoordinateVariables(int fileId, const string& segmentName, const Dictionary& dict, const map<const VariableDescriptor*, valarray<int> >& coordinateDimIds);

private:
	friend class VgtWriterTest;

	vector<SegmentDescriptor*> getCoordinateSegmentDescriptors(const Dictionary& dict) const;
	vector<VariableDescriptor*> getSubsampledCoordinateVariableDescriptors(const Dictionary& dict) const;
	vector<VariableDescriptor*> getCoordinateVariableDescriptors(const Dictionary& dict) const;
	bool isCoordinateSegmentDescriptor(const SegmentDescriptor& segmentDescriptor) const;
	bool isTiePointCoordinateSegment(const string& segmentName) const;
	bool isTiePointSegment(const string& segmentName) const;
	const vector<int> getFileIds() const;

	const string productId;
	const string safeManifestName;
};

#endif	/* VGTWRITER_H */

