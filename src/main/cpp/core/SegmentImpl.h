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

#include <map>
#include <vector>

#include "Constants.h"
#include "GridImpl.h"
#include "Segment.h"

using std::map;
using std::vector;

class SegmentImpl: public virtual Segment {
public:
	SegmentImpl(const string& id, long sizeL, long sizeM, long sizeK, long minL, long maxL);
	virtual ~SegmentImpl();

	void addVariable(const VariableDescriptor& variableDescriptor) throw (logic_error);
	void addVariable(const VariableDescriptor& variableDescriptor, const string& targetName) throw (logic_error);
	void addVariable(const string& name, int type, double scaleFactor = 1.0, double addOffset = 0.0) throw (logic_error);
	void addVariableAlias(const string& alias, const Segment& segment, const string& name) throw (logic_error);

	void addVariableByte(const string& name, int8_t fillValue = 0, double scaleFactor = 1.0, double addOffset = 0.0) throw (logic_error);
	void addVariableDouble(const string& name, double fillValue = 0.0) throw (logic_error);
	void addVariableFloat(const string& name, float fillValue = 0.0f) throw (logic_error);
	void addVariableInt(const string& name, int32_t fillValue = 0, double scaleFactor = 1.0, double addOffset = 0.0) throw (logic_error);
	void addVariableLong(const string& name, int64_t fillValue = 0) throw (logic_error);
	void addVariableShort(const string& name, int16_t fillValue = 0, double scaleFactor = 1.0, double addOffset = 0.0) throw (logic_error);
	void addVariableUByte(const string& name, uint8_t fillValue = 0, double scaleFactor = 1.0, double addOffset = 0.0) throw (logic_error);
	void addVariableUInt(const string& name, uint32_t fillValue = 0, double scaleFactor = 1.0, double addOffset = 0.0) throw (logic_error);
	void addVariableULong(const string& name, uint64_t fillValue = 0) throw (logic_error);
	void addVariableUShort(const string& name, uint16_t fillValue = 0, double scaleFactor = 1.0, double addOffset = 0.0) throw (logic_error);

	bool hasVariable(const string& name) const {
		return accessorMap.find(name) != accessorMap.end();
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

