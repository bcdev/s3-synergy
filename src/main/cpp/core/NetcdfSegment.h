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
	NetcdfSegment(const string& name, long k, long l, long m);
	~NetcdfSegment();

	Accessor& addVariable(const VariableDescriptor& variableDescriptor) throw (logic_error);
	Accessor& addVariable(const VariableDescriptor& variableDescriptor, const string& targetName) throw (logic_error);
	Accessor& addVariable(const string& name, int type, double scaleFactor = 1.0, double addOffset = 0.0) throw (logic_error);
	Accessor& addVariableAlias(const string& alias, const Segment& segment, const string& name) throw (logic_error);

	Accessor& addVariableByte(const string& name, int8_t fillValue = numeric_limits<int8_t>::min(), double scaleFactor = 1.0, double addOffset = 0.0) throw (logic_error);
	Accessor& addVariableDouble(const string& name, double fillValue = -numeric_limits<double>::max()) throw (logic_error);
	Accessor& addVariableFloat(const string& name, float fillValue = -numeric_limits<float>::max()) throw (logic_error);
	Accessor& addVariableInt(const string& name, int32_t fillValue = numeric_limits<int32_t>::min(), double scaleFactor = 1.0, double addOffset = 0.0) throw (logic_error);
	Accessor& addVariableLong(const string& name, int64_t fillValue = numeric_limits<int64_t>::min()) throw (logic_error);
	Accessor& addVariableShort(const string& name, int16_t fillValue = numeric_limits<int16_t>::min(), double scaleFactor = 1.0, double addOffset = 0.0) throw (logic_error);
	Accessor& addVariableUByte(const string& name, uint8_t fillValue = 0, double scaleFactor = 1.0, double addOffset = 0.0) throw (logic_error);
	Accessor& addVariableUInt(const string& name, uint32_t fillValue = 0, double scaleFactor = 1.0, double addOffset = 0.0) throw (logic_error);
	Accessor& addVariableULong(const string& name, uint64_t fillValue = 0) throw (logic_error);
	Accessor& addVariableUShort(const string& name, uint16_t fillValue = 0, double scaleFactor = 1.0, double addOffset = 0.0) throw (logic_error);

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
	mutable GridImpl grid;
	map<string, shared_ptr<Accessor> > accessorMap;
	vector<shared_ptr<Accessor> > accessorList;

	static const string className;
};

#endif /* NETCDFSEGMENT_H_ */
