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
    /**
     * Constructs a new segment object.
     *
     * @param k the camera count.
     * @param l the number of lines per camera image.
     * @param m the number of pixels per camera image line.
     */
    SegmentImpl(size_t k, size_t l, size_t m);

    ~SegmentImpl();

    size_t getK();
    size_t getL();
    size_t getM();

    int getSampleInt(const string& varName, int position);
    void setSampleInt(const string& varName, int position, int value);

    void addIntVariable(const string& varName);

    Reader* getReader();
    void setReader(Reader* reader);

    Segment* getPrevious();
    void setPrevious(Segment* previous);

    Segment* getNext();
    void setNext(Segment* next);

private:
    int computeArrayPosition(int k, int l, int m);

    const size_t K;
    const size_t L;
    const size_t M;
    const size_t valueCount;

    Reader* reader;
    SegmentImpl* previous;
    SegmentImpl* next;

    map<string, void* > dataMap;
};

#endif	/* SEGMENTIMPL_H */

