/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

#ifndef ABSTRACTWRITER_H
#define	ABSTRACTWRITER_H

#include <map>
#include <valarray>

#include "../modules/BasicModule.h"

using std::map;
using std::valarray;

using boost::filesystem::path;

/**
 * An abstract writer module.
 */
class AbstractWriter: public BasicModule {
public:

	void process(Context& context);
	void start(Context& context);
	void stop(Context& context);

protected:
	AbstractWriter(const string& name, const string& productId);
	virtual ~AbstractWriter();

	virtual vector<SegmentDescriptor*> getSegmentDescriptors(const ProductDescriptor& productDescriptor) const {
	    return productDescriptor.getSegmentDescriptors();
	}

	virtual void defineCoordinateVariables(const Context& context, int fileId, const ProductDescriptor& productDescriptor, const string& segmentName) const {
	}

	virtual void writeCoordinateVariables(Context& context, int fileId, const ProductDescriptor& productDescriptor, const string& segmentName) const {
	}

private:
	friend class AbstractWriterTest;

	void defineVariable(const Context& context, const ProductDescriptor& productDescriptor, const SegmentDescriptor& segmentDescriptor,
	        const VariableDescriptor& variable, const Grid& grid);
	void putGlobalAttributes(int fileId, const string& ncFileBasename, const vector<Attribute*>& attributes) const;

	const string productId;

    path targetDirPath;
    map<string, int> ncFileIdMap;
    map<string, valarray<int> > ncDimIdMap;
    map<string, int> ncVarIdMap;
};

#endif	/* ABSTRACTWRITER_H */

