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
 * File:   TestObject.h
 * Author: ralf
 *
 * Created on December 21, 2010, 4:49 PM
 */

#ifndef TESTOBJECT_H
#define	TESTOBJECT_H

#include "../../../main/cpp/core/Object.h"

class TestObject : public Object {
public:

    TestObject(const string& objectId) : Object(), id(objectId) {
    }

    ~TestObject() {
    }

    const string& getId() const {
        return id;
    }

private:
    const string id;
};

#endif	/* TESTOBJECT_H */
