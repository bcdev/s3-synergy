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

class Attribute {
public:

    Attribute(string type, string key, string value) : type(type), key(key), value(value) {
    }

    void setValue(string& value) {
        this->value = value;
    }

    string getValue() const {
        return value;
    }

    string getType() const {
        return type;
    }

    string getKey() const {
        return key;
    }

    string toString() {
        std::ostringstream oss;
        oss << "Attribute: " << "[";
        oss << "key = " << key << ", ";
        oss << "type = " << type << "]";
        return oss.str();
    }

private:
    string type;
    string key;
    string value;
};

// TODO - check attributes

class Variable {
public:

    Variable(string ncName, string symbolicName, NcType type) : ncName(ncName), symbolicName(symbolicName), type(type) {
    };

    virtual ~Variable() {
    };

    virtual void addAttribute(Attribute* attribute) = 0;
    virtual void addDimension(Dimension* dimension) = 0;

    virtual void setFileName(string fileName) = 0;
    virtual string getFileName() const = 0;
    /**
     * Returns the name of the variable, by which it is addressed within its
     * netCDF-file.
     * @return the netCDF-name of the variable.
     */
    virtual string getNcName() const = 0;
    /**
     * Returns the name of the variable, by which it is addressed within the
     * SYN processing chain.
     * @return the symbolic name of the variable.
     */
    virtual string getSymbolicName() const = 0;
    virtual NcType getType() const = 0;
    virtual vector<Dimension*> getDimensions() const = 0;
    virtual vector<Attribute*> getAttributes() const = 0;
    virtual Attribute& getAttribute(string& name) const = 0;

    virtual string toString() const = 0;

protected:
    vector<Attribute*> attributes;
    vector<Dimension*> dims;
    /**
     * The name of the variable, by which it is addressed within its netCDF-file.
     */
    string ncName;
    /**
     * The name of the variable, by which it is addressed within the SYN
     * processing chain.
     */
    string symbolicName;
    string fileName;
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

    string toString() {
        std::ostringstream oss;
        oss << "Dimension: " << "[";
        oss << "name = " << name << ", ";
        oss << "range = " << range << "]";
        return oss.str();
    }

private:
    string name;
    size_t range;
};

#endif	/* VARIABLE_H */

