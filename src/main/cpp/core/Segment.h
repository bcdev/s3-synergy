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
#include "Grid.h"
#include "Identifiable.h"

using std::logic_error;
using std::string;

/**
 * A container for variables, which exhibit the same Grid.
 */
class Segment : public Identifiable {
public:

    /**
     * Destructor.
     */
    virtual ~Segment() {
    };

    /**
     * Adds a variable of type int8_t to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableByte(const string& varName) throw (logic_error) = 0;

    /**
     * Adds a variable of type uint8_t to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableUByte(const string& varName) throw (logic_error) = 0;

    /**
     * Adds a variable of type int16_t to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableShort(const string& varName) throw (logic_error) = 0;

    /**
     * Adds a variable of type uint16_t to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableUShort(const string& varName) throw (logic_error) = 0;

    /**
     * Adds a variable of type int32_t to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableInt(const string& varName) throw (logic_error) = 0;

    /**
     * Adds a variable of type uint32_t to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableUInt(const string& varName) throw (logic_error) = 0;

    /**
     * Adds a variable of type int64_t to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableLong(const string& varName) throw (logic_error) = 0;

    /**
     * Adds a variable of type uint64_t to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableULong(const string& varName) throw (logic_error) = 0;

    /**
     * Adds a variable of type float to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableFloat(const string& varName) throw (logic_error) = 0;

    /**
     * Adds a variable of type double to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableDouble(const string& varName) throw (logic_error) = 0;

    /**
     * Tests whether a variable of interest has been added to the the segment
     * or not.
     * @param varName The name of the variable of interest.
     * @return true if the variable has been added to the segment,
     *         false otherwise.
     */
    virtual bool hasVariable(const string&) const = 0;

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
     * @param varName The name of the variable to be accessed.
     * @return the accessor.
     */
    virtual Accessor& getAccessor(const string& varName) const throw (logic_error) = 0;

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
};

#endif	/* SEGMENT_H */

