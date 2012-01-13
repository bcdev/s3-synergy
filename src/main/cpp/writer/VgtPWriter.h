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
    const vector<SegmentDescriptor*> getSegmentDescriptors(const Context& context) const;
    const string& getSafeManifestName() const;
    void writeCommonVariables(const Context& context);
    void defineCommonDimensions(int fileId, bool isSubsampled);
    void defineCommonVariables(int fileId, bool isSubsampled);

private:
	friend class VgtPWriterTest;
	const vector<SegmentDescriptor*> getCommonSegments(const Context& context) const;
	bool isCommonDescriptor(const SegmentDescriptor& segmentDescriptor) const;
};

#endif	/* VGTPWRITER_H */

