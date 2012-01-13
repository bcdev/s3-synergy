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
    const string& getSafeManifestName() const;

private:
	friend class VgtPWriterTest;
};

#endif	/* VGTPWRITER_H */

