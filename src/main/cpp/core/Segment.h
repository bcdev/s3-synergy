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

#include <string>

#include "Accessor.h"

using std::string;

class Segment {
public:

    /**
     * Destructor.
     */
    virtual ~Segment() {
    };

    /**
     * Adds a variable of type {@code int8_t} to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableByte(const string& varName) = 0;

    /**
     * Adds a variable of type {@code uint8_t} to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableUByte(const string& varName) = 0;

    /**
     * Adds a variable of type {@code int16_t} to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableShort(const string& varName) = 0;

    /**
     * Adds a variable of type {@code uint16_t} to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableUShort(const string& varName) = 0;

    /**
     * Adds a variable of type {@code int32_t} to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableInt(const string& varName) = 0;

    /**
     * Adds a variable of type {@code uint32_t} to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableUInt(const string& varName) = 0;

    /**
     * Adds a variable of type {@code int64_t} to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableLong(const string& varName) = 0;

    /**
     * Adds a variable of type {@code uint64_t} to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableULong(const string& varName) = 0;

    /**
     * Adds a variable of type {@code float} to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableFloat(const string& varName) = 0;

    /**
     * Adds a variable of type {@code double} to the segment.
     * @param varName The name of the variable being added.
     */
    virtual void addVariableDouble(const string& varName) = 0;

    /**
     * Tests whether a variable of interest has been added to the the segment
     * or not.
     * @param varName The name of the variable of interest.
     * @return {@code true} if the variable has been added to the segment,
     *         {@code false} otherwise.
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
     * Moves the segment grid to a given target row index. On return
     * the segment grid starts with the target row index.
     * @param l The target row index.
     */
    virtual const Grid& setGridL(uint16_t l) = 0;

    /**
     * Returns an accessor to a variable's samples.
     * @param varName The name of the variable to be accessed.
     * @return the accessor.
     */
    virtual Accessor& getAccessor(const string& varName) const = 0;

    /**
     * Returns a string representation of the segment.
     * @return a string representation of the segment.
     */
    virtual string toString() const = 0;
};

#endif	/* SEGMENT_H */

