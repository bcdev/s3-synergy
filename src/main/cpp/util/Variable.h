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

class Dimension;

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

// TODO - check attributes
class Variable {
public:

    Variable(string id, NcType type) : id(id), type(type) {
    };

    virtual ~Variable() {
    };

    virtual void addAttribute(Attribute<void*>* attribute) = 0;
    virtual void addDimension(Dimension* dimension) = 0;
    virtual string getId() const = 0;
    virtual NcType getType() const = 0;
    virtual vector<Dimension*> getDimensions() const = 0;
    virtual vector<Attribute<void*>*> getAttributes() const = 0;

    static Attribute<void*>* createIntAttribute(string key, int value) {
        Attribute<void*>* attribute = new Attribute<void*>(key);
        int valueArray[1];
        valueArray[0] = value;
        attribute->setValue(valueArray);
        return attribute;
    }

    static Attribute<void*>* createShortAttribute(string key, int value) {
        Attribute<void*>* attribute = new Attribute<void*>(key);
        short valueArray[1];
        valueArray[0] = value;
        attribute->setValue(valueArray);
        return attribute;
    }

    static Attribute<void*>* createFloatAttribute(string key, float value) {
        Attribute<void*>* attribute = new Attribute<void*>(key);
        float valueArray[1];
        valueArray[0] = value;
        attribute->setValue(valueArray);
        return attribute;
    }

    static Attribute<void*>* createStringAttribute(string key, string value) {
        Attribute<void*>* attribute = new Attribute<void*>(key);
        string valueArray[1];
        valueArray[0] = value;
        attribute->setValue(valueArray);
        return attribute;
    }

protected:
    vector<Attribute<void*>*> attributes;
    vector<Dimension*> dims;
    string id;
    string ncName;
    NcType type;
};

class Dimension {
public:

    Dimension(string name, size_t range) : name(name), range(range) {
    }

    size_t getRange() const {
        return range;
    }

    string getName() const {
        return name;
    }

private:
    string name;
    size_t range;
};

#endif	/* VARIABLE_H */

