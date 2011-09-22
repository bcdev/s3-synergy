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
 * File:   Segment.h
 * Author: thomass
 *
 * Created on November 11, 2010, 9:50 AM
 */

#ifndef SEGMENT_H
#define	SEGMENT_H

#include <stdexcept>
#include <string>

#include "Accessor.h"
#include "Boost.h"
#include "Dictionary.h"
#include "Grid.h"
#include "Identifiable.h"

using std::logic_error;
using std::string;

/**
 * A container for variables, which exhibit the same Grid.
 */
class Segment: public Identifiable {
public:

	/**
	 * Destructor.
	 */
	virtual ~Segment() {
	}

	/**
	 * Adds a variable to the segment.
	 * @param variableDescriptor The descriptor of the variable being added.
	 */
	virtual void addVariable(const VariableDescriptor& variableDescriptor) throw (logic_error) = 0;

	/**
	 * Adds a variable to the segment.
	 * @param variableDescriptor The descriptor of the variable being added.
	 * @param name The name of the variable being added.
	 */
	virtual void addVariable(const VariableDescriptor& variableDescriptor, const string& targetName) throw (logic_error) = 0;

	/**
	 * Adds a variable to the segment.
	 * @param name The name of the variable being added.
	 * @param type The type of the variable being added.
	 * @param scaleFactor The scale factor of the variable being added.
	 * @param addOffset The add-offset of the variable being added.
	 */
	virtual void addVariable(const string& name, int type, double scaleFactor =
			1.0, double addOffset = 0.0) throw (logic_error) = 0;

	/**
	 * Adds a variable from an other segment.
	 * @param alias The alias name of variable being copied.
	 * @param segment The source segment.
	 * @param name The name of the variable being copied.
	 */
	virtual void addVariableAlias(const string& alias, const Segment& segment,
			const string& name) throw (logic_error) = 0;

	/**
	 * Tests whether a variable of interest has been added to the the segment
	 * or not.
	 * @param name The name of the variable of interest.
	 * @return true if the variable has been added to the segment,
	 *         false otherwise.
	 */
	virtual bool hasVariable(const string& name) const = 0;

	/**
	 * Returns the segment ID.
	 * @return the segment ID.
	 */
	virtual const string& getId() const = 0;

	/**
	 * Returns the segment grid.
	 * @return the segment grid.
	 */
	virtual const Grid& getGrid() const = 0;

	/**
	 * Returns an accessor to a variable's samples.
	 * @param name The name of the variable to be accessed.
	 * @return the accessor.
	 */
	virtual Accessor& getAccessor(const string& name) const throw (logic_error) = 0;

	/**
	 * Moves the start of the segment to the row with the given index. The
	 * segment must not be moved backward and must not be moved beyond the
	 * the row succeeding the last row in the current segment grid.
	 *
	 * The segment cannot be moved to a row, which results in an extent
	 * beyond the maximum valid row in the segment grid. The extent of the
	 * segment is constrained to the region defined by the the minimum and
	 * maximum valid rows in the segment grid.
	 *
	 * On return the segment is moved to the maximum row, which is consistent
	 * with the given target row and the extent constraints.
	 *
	 * @param l The index of the row the segment shall be moved to.
	 */
	virtual void moveForward(size_t l) throw (logic_error) = 0;

	/**
	 * Returns a string representation of the segment.
	 * @return a string representation of the segment.
	 */
	virtual string toString() const = 0;

	/**
	 * Equal to operator.
	 * @param that A segment.
	 * @return true if this and that segment are the
	 *         same, false otherwise.
	 */
	bool operator==(const Segment& that) const {
		return this == &that;
	}

	virtual shared_ptr<Accessor> getSharedAccessor(
			const string& name) const = 0;
};

#endif	/* SEGMENT_H */

