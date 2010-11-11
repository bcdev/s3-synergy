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
     * @param varName the variable name to get the sampels for.
     * @param samples an int-vector containing the result.
     */
    virtual void getSamplesInt(const std::string& varName, std::vector<int>& samples) = 0;
//    virtual void setSamplesInt(const std::string& varName, std::valarray<int>& samples);

    virtual void add(const std::string& varName) = 0;
    virtual void remove(const std::string& varName) = 0;
    virtual Pixel& getPixel(int k, int l, int m, Pixel& pixel) = 0;
    virtual PixelImpl createPixel(const std::string varName, int k, int l, int m) = 0;

};

#endif	/* SEGMENT_H */

