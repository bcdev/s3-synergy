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

	virtual const string& getProductDescriptorIdentifier() const = 0;
	virtual const string& getSafeManifestName() const = 0;
	virtual const vector<SegmentDescriptor*> getSegmentDescriptors(const Dictionary& dict) const = 0;
	virtual void writeCommonVariables(Context& context) = 0;
	virtual void defineCommonDimensions(int fileId, const string& segmentName, const Dictionary& dict, map<const VariableDescriptor*, valarray<int> >& commonDimIds) = 0;
	virtual void defineCommonVariables(int fileId, const string& segmentName, const Dictionary& dict, const map<const VariableDescriptor*, valarray<int> >& commonDimIds) = 0;
	virtual void resolveSubsampling(int fileId, const string& segmentName) = 0;
private:
	friend class AbstractWriterTest;
	void defineNcVar(const Context& context, const ProductDescriptor& productDescriptor, const SegmentDescriptor& segmentDescriptor,
	        const VariableDescriptor& variable, const Grid& grid);
	void defineDimensions(const int fileId, const string& name, const vector<Dimension*>& dimensions, const Grid& grid, valarray<int>& dimIds);
	void putGlobalAttributes(int fileId, const VariableDescriptor& variableDescriptor, const vector<Attribute*>& attributes) const;

	void putData(int ncId, int varId, size_t dimCount, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;

    void putByteData(int ncId, int varId, size_t dimCount, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
    void putUByteData(int ncId, int varId, size_t dimCount, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
    void putShortData(int ncId, int varId, size_t dimCount, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
    void putUShortData(int ncId, int varId, size_t dimCount, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
    void putIntData(int ncId, int varId, size_t dimCount, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
    void putUIntData(int ncId, int varId, size_t dimCount, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
    void putLongData(int ncId, int varId, size_t dimCount, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
    void putULongData(int ncId, int varId, size_t dimCount, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
    void putFloatData(int ncId, int varId, size_t dimCount, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;
    void putDoubleData(int ncId, int varId, size_t dimCount, const Accessor& accessor, long firstL, long lastL, const Grid& grid) const;

	void createSafeProduct(const Context& context);
	void copyTemplateFiles() const;
	string readManifest() const;
	void setStartTime(const Context& context, string& manifest) const;
	void setChecksums(string& manifest) const;
	void writeManifest(string& manifest) const;
	void removeManifestTemplate() const;

	static void replaceString(const string& toReplace, const string& replacement, string& input);
	static string getMd5Sum(const string& file);
};

#endif	/* ABSTRACTWRITER_H */

