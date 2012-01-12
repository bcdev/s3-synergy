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

protected:
	const string getProductDescriptorIdentifier() const;
	void createSafeProduct(const Context& context);

private:
	friend class SynL2WriterTest;
	void copyTemplateFiles() const;
	string readManifest() const;
	void setStartTime(const Context& context, string& xml) const;
	void setChecksums(string& manifest) const;
	void writeManifest(string& manifest) const;
	void removeManifestTemplate() const;
	void replaceString(const string& toReplace, const string& replacement, string& input) const;
	string getMd5Sum(const string& file) const;
};

#endif	/* SYNL2WRITER_H */

