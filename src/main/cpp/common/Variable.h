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
 * File:   Variable.h
 * Author: thomass
 *
 * Created on November 11, 2010, 10:00 AM
 */

#ifndef VARIABLE_H
#define	VARIABLE_H

#include <ncvalues.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

template <class T> class Attribute;
class Dimension;

class Variable {
public:
    Variable(string id) : id(id) {};
    virtual ~Variable() {};

    virtual void addAttribute(Attribute<void*>* attribute) = 0;
    virtual string getId() = 0;

protected:
    vector<Attribute<void*>*> attributes;
    string id;
private:
    NcType type;
    vector<Dimension*> dims;
};

template <class T> class Attribute {
public:
    Attribute(string key) : key(key) {
    }

    void setValue(T value) {
        this->value = value;
    }
    T getValue() const {
        return value;
    }
    void setUnit(string unit) {
        this->unit = unit;
    }
    string getUnit() const {
        return unit;
    }

private:
    string key;
    string unit;
    T value;
};

class Dimension {
public:

    Dimension(string name, NcType type) : key(key), type(type), range(range) {
    }

private:
    string key;
    NcType type;
    int range;
};

#endif	/* VARIABLE_H */

