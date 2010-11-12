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

class Pixel;
class PixelImpl;

class Segment {
public:

    virtual ~Segment() {
    };

    //    template<class T> virtual void getSamples(const std::string& varName, std::valarray<T> samples) = 0;
    //    template<class T> virtual void setSamples(const std::string& varName, std::valarray<T> samples) = 0;

    /**
     * Gets all int samples for a given variable name.
     * @param varName the variable name to get the samples for.
     * @param samples an int-vector containing the result.
     */
    virtual void getSamplesInt(const std::string& varName, std::vector<int>& samples) = 0;

    /**
     * Sets all int samples for a given variable name.
     * @param varName the variable name to set the samples for.
     * @param samples an int-vector containing the samples to set.
     */
    virtual void setSamplesInt(const std::string& varName, std::vector<int>& samples) = 0;

    /**
     * Adds a variable to the segment. However, no samples are set, this is done using setSamplesXXX().
     * @param varName The name of the variable to add.
     */
    virtual void addIntVariable(const std::string& varName) = 0;
    virtual void remove(const std::string& varName) = 0;
    virtual Pixel* getPixel(int k, int l, int m, Pixel* pixel) = 0;

};

#endif	/* SEGMENT_H */

