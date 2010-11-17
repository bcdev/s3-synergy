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
 * File:   PixelImpl.h
 * Author: thomass
 *
 * Created on November 11, 2010, 8:45 AM
 */

#ifndef PIXELIMPL_H
#define	PIXELIMPL_H

#include "Pixel.h"
#include "Segment.h"

using std::string;

class PixelImpl : public Pixel {
public:

    PixelImpl(Segment& parent, int k, int l, int m, int arrayPosition) : Pixel(), segment(parent) {
        this->k = k;
        this->l = l;
        this->m = m;
        this->position = arrayPosition;
    };

    /**
     * Default destructor
     */
    virtual ~PixelImpl() {
    };

    /**
     * Flag inquiry.
     */
    bool isFlagRaised(const string& name, int flagMask) const;

    /**
     * Flag setter.
     */
    void raiseFlag(const string& name, int flagMask);

    /**
     * Clears a flag.
     * @param name the flag to clear
     * @param flagMask the flag mask
     */
    void clearFlag(const string& name, int flagMask);

    /**
     * Getter for segment.
     *
     * @return the segment associated with this pixel.
     */
    Segment& getSegment() const;

private:
    Segment& segment;
    int k, l, m;
    int position;
};

#endif	/* PIXELIMPL_H */

