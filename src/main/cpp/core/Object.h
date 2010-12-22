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
 * File:   Object.h
 * Author: ralf
 *
 * Created on December 20, 2010, 12:42 PM
 */

#ifndef OBJECT_H
#define	OBJECT_H

#include <string>

using std::string;

/**
 * Interface for a general object exhibiting an ID. Implementing
 * classes shall implement the copy contructor.
 */
class Object {
public:

    /**
     * Destructor.
     */
    virtual ~Object() {
    }

    /**
     * Returs the ID of this object.
     * @return the ID of this object.
     */
    virtual const string& getId() const = 0;

    /**
     * Equal to operator.
     * @param that An object.
     * @return {@code true} if {@code this} and {@code that} object are the
     *         same, {@code false} otherwise.
     */
    virtual bool operator==(const Object& that) const {
        return this == &that;
    }
};

#endif	/* OBJECT_H */

