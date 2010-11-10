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
 */
#ifndef PIXEL_H
#define PIXEL_H

#include <string>

/**
 * \mainpage
 *
 * Pixel interface.
 */
class Pixel
{
public:
    /**
     * Getter.
     */
    virtual double getDouble(const std::string& name) const = 0;

    /**
     * Setter.
     */
    virtual double setDouble(const std::string& name, double value) = 0;

    /**
     * Default destructor
     */
    virtual ~Pixel() { };
};

#endif // PIXEL_H
