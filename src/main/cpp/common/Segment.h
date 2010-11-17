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
class PixelImpl;

class Segment {
public:

    virtual ~Segment() {
    };

    /**
     * Gets the sample for a given variable and position.
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
     * Gets all int samples for a given variable name.
     * @param varName the variable name to get the samples for.
     * @param samples an int-vector containing the result.
     */
    virtual void getSamplesInt(const string& varName, vector<int>& samples) = 0;

    /**
     * Sets all int samples for a given variable name.
     * @param varName the variable name to set the samples for.
     * @param samples an int-vector containing the samples to set.
     */
    virtual void setSamplesInt(const string& varName, vector<int>& samples) = 0;

    /**
     * Adds a variable to the segment and sets all values to 0.
     * @param varName The name of the variable to add.
     */
    virtual void addIntVariable(const string& varName) = 0;
    virtual void remove(const string& varName) = 0;
    virtual Pixel* getPixel(int k, int l, int m, Pixel* pixel) = 0;

};

#endif	/* SEGMENT_H */

