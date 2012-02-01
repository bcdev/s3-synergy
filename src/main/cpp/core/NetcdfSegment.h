/*
 * NetcdfSegment.h
 *
 *  Created on: Feb 01, 2012
 *      Author: thomasstorm
 */

#ifndef NETCDFSEGMENT_H_
#define NETCDFSEGMENT_H_

#include <limits>

#include "Constants.h"
#include "GridImpl.h"
#include "Segment.h"

using std::map;
using std::numeric_limits;
using std::vector;

class NetcdfSegment: public virtual Segment {
public:
    NetcdfSegment(const string& name, const string& targetDirPath, long l, long m);
	~NetcdfSegment();

	Accessor& addVariable(const VariableDescriptor& variableDescriptor) throw (logic_error);
	Accessor& addVariable(const VariableDescriptor& variableDescriptor, const string& targetName) throw (logic_error);
	Accessor& addVariable(const string& name, int type, double scaleFactor = 1.0, double addOffset = 0.0) throw (logic_error);
	Accessor& addVariableAlias(const string& alias, const Segment& segment, const string& name) throw (logic_error);

	Accessor& addVariableByte(const VariableDescriptor& d) throw (logic_error);
	Accessor& addVariableDouble(const VariableDescriptor& d) throw (logic_error);
	Accessor& addVariableFloat(const VariableDescriptor& d) throw (logic_error);
	Accessor& addVariableInt(const VariableDescriptor& d) throw (logic_error);
	Accessor& addVariableLong(const VariableDescriptor& d) throw (logic_error);
	Accessor& addVariableShort(const VariableDescriptor& d) throw (logic_error);
	Accessor& addVariableUByte(const VariableDescriptor& d) throw (logic_error);
	Accessor& addVariableUInt(const VariableDescriptor& d) throw (logic_error);
	Accessor& addVariableULong(const VariableDescriptor& d) throw (logic_error);
	Accessor& addVariableUShort(const VariableDescriptor& d) throw (logic_error);

	bool hasVariable(const string& name) const {
		return accessorMap.find(name) != accessorMap.end();
	}

	vector<string> getVariableNames() const {
        vector<string> variableNames;
        typedef std::pair<string, shared_ptr<Accessor> > MapEntry;
        foreach(MapEntry entry, accessorMap) {
            variableNames.push_back(entry.first);
        }
        return variableNames;
    }

	const string& getId() const {
		return id;
	}

	const Grid& getGrid() const {
		return grid;
	}

	Accessor& getAccessor(const string& name) const throw (logic_error);

	shared_ptr<Accessor> getSharedAccessor(const string& name) const {
		return accessorMap.at(name);
	}

	void moveForward(long l) throw (logic_error);

	string toString() const;

private:
	void unique(const string& varName) const throw (logic_error);

	const string id;
	const string targetDirPath;
	mutable GridImpl grid;
	map<string, shared_ptr<Accessor> > accessorMap;
	vector<shared_ptr<Accessor> > accessorList;

	static const string className;
};

#endif /* NETCDFSEGMENT_H_ */
