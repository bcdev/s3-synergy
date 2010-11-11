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
 * File:   PixelClassification.h
 * Author: thomass
 *
 * Created on November 10, 2010, 4:26 PM
 */

#ifndef PIXELCLASSIFICATION_H
#define	PIXELCLASSIFICATION_H

#include "Pixel.h"

class PixelClassification {
public:
    PixelClassification();
    PixelClassification(const PixelClassification& orig);
    virtual ~PixelClassification();

    void classify(Pixel& pixel) const;
private:

};

#endif	/* PIXELCLASSIFICATION_H */

