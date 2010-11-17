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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * File:   SegmentImpl.h
 * Author: thomass
 *
 * Created on November 11, 2010, 10:04 AM
 */

#ifndef SEGMENTIMPL_H
#define	SEGMENTIMPL_H

#include <map>
#include <valarray>

#include "Segment.h"
#include "PixelImpl.h"

using std::map;
using std::string;
using std::vector;

class SegmentImpl : public Segment {
public:
    SegmentImpl(int xPos, int yPos, int width, int height) {
        this->xPos = xPos;
        this->yPos = yPos;
        this->width = width;
        this->height = height;
        numValues = width * height;
    }

    ~SegmentImpl();

    int getSampleInt(const string& varName, int position);
    void setSampleInt(const string& varName, int position, int value);

    void getSamplesInt(const string& varName, vector<int>& samples);
    void setSamplesInt(const string& varName, vector<int>& samples);

    void addIntVariable(const string& varName);
    void remove(const string& varName);
    Pixel* getPixel(int k, int l, int m, Pixel* pixel);

private:
    int xPos;
    int yPos;
    int width;
    int height;
    int numValues;
    const static int NO_DATA_VALUE = 0;
    map<string, void* > dataBufferMap;
    int computeArrayPosition( int k, int l, int m );
};

#endif	/* SEGMENTIMPL_H */

