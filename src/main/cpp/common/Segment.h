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
#include <vector>

using std::string;
using std::vector;

class Pixel;
class Reader;

class Segment {
public:

    /**
     * Destructor.
     */
    virtual ~Segment() {
    };

    /**
     * Returns the maximum camera index.
     * @return the maximum camera index.
     */
    virtual size_t getMaxK() = 0;

    /**
     * Returns the minimum camera index.
     * @return the minimum camera index.
     */
    virtual size_t getMinK() = 0;

    /**
     * Returns the maximum line index.
     * @return the maximum line index.
     */
    virtual size_t getMaxL() = 0;

    /**
     * Returns the minimum line index.
     * @return the minimum line index.
     */
    virtual size_t getMinL() = 0;

    /**
     * Returns the maximum column index.
     * @return the maximum column index.
     */
    virtual size_t getMaxM() = 0;

    /**
     * Returns the minimum column index.
     * @return the minimum column index.
     */
    virtual size_t getMinM() = 0;

    /**
     * Returns the sample for a given variable and position.
     * @param varName the variable to get the sample for.
     * @param position the position to get the sample for.
     * @return the sample value.
     */
    virtual int getSampleInt(const string& varName, int position) = 0;

    /**
     * Sets the sample for a given variable and position.
     * @param varName the variable to set the sample for.
     * @param position the position to set the sample for.
     * @param value the sample value.
     */
    virtual void setSampleInt(const string& varName, int position, int value) = 0;

    /**
     * Adds a variable to the segment and sets all values to 0.
     * @param varName The name of the variable to add.
     */
    virtual void addIntVariable(const string& varName) = 0;
};

#endif	/* SEGMENT_H */

