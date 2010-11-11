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

class PixelImpl : public Pixel {
public:

    PixelImpl(Segment& parent, int k, int l, int m) : Pixel(), segment(parent) {
        this->k = k;
        this->l = l;
        this->m = m;
        position = computePosition();
    };

    /**
     * Default destructor
     */
    virtual ~PixelImpl() {
    };

    /**
     * Getter.
     */
    double getDouble(const std::string& name) const;

    /**
     * Setter.
     */
    void setDouble(const std::string& name, double value);

    /**
     * Flag inquiry.
     */
    bool isRaised(const std::string& name, int flagMask) const;

    /**
     * Flag setter.
     */
    void raise(const std::string& name, int flagMask);

    /**
     * Clears a flag.
     * @param name the flag to clear
     * @param flagMask the flag mask
     */
    void clear(const std::string& name, int flagMask);

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
    int computePosition();
};

#endif	/* PIXELIMPL_H */

