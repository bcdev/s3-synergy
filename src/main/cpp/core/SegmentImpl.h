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

#include "Boost.h"
#include "Constants.h"
#include "GridImpl.h"
#include "Segment.h"

using std::map;
using std::vector;

class SegmentImpl: public virtual Segment {
public:
	SegmentImpl(const string& id, size_t sizeL, size_t sizeM =
			Constants::N_DET_CAM, size_t sizeK = Constants::N_CAM, size_t minL =
			0, size_t maxL = Constants::N_LINE_OLC - 1);
	virtual ~SegmentImpl();

	void addVariable(const string& name, int type) throw (logic_error);
	void addVariable(const string& name, const Segment& segment) throw (logic_error);

	void addVariableByte(const string& name) throw (logic_error);
	void addVariableDouble(const string& name) throw (logic_error);
	void addVariableFloat(const string& name) throw (logic_error);
	void addVariableInt(const string& name) throw (logic_error);
	void addVariableLong(const string& name) throw (logic_error);
	void addVariableShort(const string& name) throw (logic_error);
	void addVariableUByte(const string& name) throw (logic_error);
	void addVariableUInt(const string& name) throw (logic_error);
	void addVariableULong(const string& name) throw (logic_error);
	void addVariableUShort(const string& name) throw (logic_error);

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

	void moveForward(size_t l) throw (logic_error);

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

