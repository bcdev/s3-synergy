/*
 * File:   VgtSWriter.h
 * Author: thomass
 *
* Created on January 13, 2012, 11:34
 */

#ifndef VGTSWRITER_H
#define	VGTSWRITER_H

#include "../writer/AbstractWriter.h"

class VgtSWriter: public AbstractWriter {
public:
    VgtSWriter();
    virtual ~VgtSWriter();

protected:
    const string& getProductDescriptorIdentifier() const;
    const string& getSafeManifestName() const;

private:
	friend class VgtSWriterTest;
};

#endif	/* VGTSWRITER_H */

