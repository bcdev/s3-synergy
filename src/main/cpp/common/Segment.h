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

#include "Variable.h"

using std::string;

class Segment {
public:

    /**
     * Destructor.
     */
    virtual ~Segment() {
    };

    /**
     * Adds a variable to the segment and sets all values to 0.
     * @param varName The name of the variable to add.
     */
    virtual void addIntVariable(const string& varName) = 0;

    /**
     * Adds a variable to the segment and sets all values to 0.
     * @param variable The variable to add.
     */
    virtual void addIntVariable(Variable* variable) = 0;

    /**
     * Computes the position for a pixel of interest.
     * @param k The pixel's camera coordinate.
     * @param l The pixel's line in camera image coordinate.
     * @param m The pixel's column in camera image line coordinate.
     * @return the pixel's position.
     */
    virtual size_t computePosition(size_t k, size_t l, size_t m) const = 0;

    /**
     * Returns the segment ID.
     * @return the segment ID.
     */
    virtual const string& getId() const = 0;

    /**
     * Returns the maximum camera index.
     * @return the maximum camera index.
     */
    virtual size_t getMaxK() const = 0;

    /**
     * Returns the minimum camera index.
     * @return the minimum camera index.
     */
    virtual size_t getMinK() const = 0;

    /**
     * Returns the maximum line index.
     * @return the maximum line index.
     */
    virtual size_t getMaxL() const = 0;

    /**
     * Returns the minimum line index.
     * @return the minimum line index.
     */
    virtual size_t getMinL() const = 0;

    /**
     * Returns the maximum column index.
     * @return the maximum column index.
     */
    virtual size_t getMaxM() const = 0;

    /**
     * Returns the minimum column index.
     * @return the minimum column index.
     */
    virtual size_t getMinM() const = 0;

    /**
     * Sets the minimum line index.
     * @param minL The minimum line index.
     */
    virtual void setMinL(size_t minL) = 0;

    /**
     * Sets the maximum line index.
     * @param maxL The maximum line index.
     */
    virtual void setMaxL(size_t maxL) = 0;

    /**
     * Returns the sample value of a variable at a position of interest.
     * @param varName The name of the variable of interest.
     * @param position The position of interest.
     * @return the sample value.
     */
    virtual int getSampleInt(const string& varName, size_t position) const = 0;

    /**
     * Sets the sample value of a variable at a position of interest.
     * @param varName The name of the variable of interest.
     * @param position The position of interest.
     * @param value The sample value.
     */
    virtual void setSampleInt(const string& varName, size_t position, int value) = 0;

    /**
     * Sets all sample values of a variable.
     *
     * @param varName The name of the variable of interest.
     * @param values The sample values, given as int array.
     */
    virtual void setSamplesInt(const string& varName, int* values) = 0;

    /**
     * Returns a string representation of this object.
     * @return A string representation of this object.
     */
    virtual string toString() const = 0;
    
};

#endif	/* SEGMENT_H */

