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
    VgtWriter(const string& productId = Constants::PRODUCT_VGP);
    ~VgtWriter();

protected:
	vector<SegmentDescriptor*> getSegmentDescriptors(const ProductDescriptor& productDescriptor) const;
    void defineCoordinateVariables(const Context& context, int fileId, const ProductDescriptor& productDescriptor, const string& segmentName) const;
    void writeCoordinateVariables(Context& context, int fileId, const ProductDescriptor& productDescriptor, const string& segmentName) const;

private:
	friend class VgtWriterTest;

	vector<VariableDescriptor*> getCoordinateVariableDescriptors(const ProductDescriptor& productDescriptor) const;
	vector<VariableDescriptor*> getTiePointCoordinateVariableDescriptors(const ProductDescriptor& productDescriptor) const;

	static bool isCoordinateSegment(const string& segmentName);
	static bool isTiePointCoordinateSegment(const string& segmentName);
	static bool isTiePointSegment(const string& segmentName);
};

#endif	/* VGTWRITER_H */

