/*
 * Copyright (C) 2010 by Brockmann Consult (info@brockmann-consult.de)
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
 *
 * File:   SegmentImpl.h
 * Author: ralf
 *
 * Created on December 15, 2010, 5:28 PM
 */

#ifndef SEGMENTIMPL_H
#define	SEGMENTIMPL_H

#include <limits>
#include <map>
#include <vector>

#include "Constants.h"
#include "GridImpl.h"
#include "Segment.h"

using std::map;
using std::numeric_limits;
using std::vector;

class SegmentImpl: public virtual Segment {
public:
	SegmentImpl(const string& id, long sizeL, long sizeM, long sizeK, long minL, long maxL);
	virtual ~SegmentImpl();

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

#endif	/* SEGMENTIMPL_H */

