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

#include <sstream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <netcdf.h>

using std::string;
using std::vector;

class Dimension;

/**
 * Represents the static information about an attribute of a netCDF-variable.
 *
 * @param type The attribute's type.
 * @param key The attribute's name.
 * @param value The attribute's value.
 */
class Attribute {
public:

    /**
     * Constructor.
     * @param type The attribute's type.
     * @param key The attribute's name.
     * @param value The attribute's value.
     */
    Attribute(string type, string key, string value) : type(type), key(key), value(value) {
    }

    /**
     * Destructor
     */
    ~Attribute() {
    }

    /**
     * Getter for the attribute's value.
     * @return The attribute's value.
     */
    string getValue() const {
        return value;
    }

    int8_t getByte() const {
        return boost::numeric_cast<int8_t > (boost::lexical_cast<int16_t > (value));
    }

    int8_t getUByte() const {
        return boost::numeric_cast<uint8_t > (boost::lexical_cast<uint16_t > (value));
    }

    int32_t getShort() const {
        return boost::lexical_cast<int32_t > (value);
    }

    uint32_t getUShort() const {
        return boost::lexical_cast<uint32_t > (value);
    }

    int32_t getInt() const {
        return boost::lexical_cast<int32_t > (value);
    }

    uint32_t getUInt() const {
        return boost::lexical_cast<uint32_t > (value);
    }

    int64_t getLong() const {
        return boost::lexical_cast<int64_t > (value);
    }

    uint64_t getULong() const {
        return boost::lexical_cast<uint64_t > (value);
    }

    float getFloat() const {
        return boost::lexical_cast<float> (value);
    }

    double getDouble() const {
        return boost::lexical_cast<double> (value);
    }

    /**
     * Getter for the attribute's type.
     * @return The attribute's type.
     */
    string getType() const {
        return type;
    }

    /**
     * Getter for the attribute's name.
     * @return The attribute's name.
     */
    string getKey() const {
        return key;
    }

    /**
     * Returns a string representation of the attribute.
     * @return A string representation of the attribute.
     */
    string toString() const {
        std::ostringstream oss;
        oss << "Attribute: " << "[";
        oss << "key = " << key << ", ";
        oss << "value = " << value << ", ";
        oss << "type = " << type << "]";
        return oss.str();
    }

private:
    string type;
    string key;
    string value;
};

/**
 * Represents the static information about an (input or output) variable. All 
 * variables are instantiated using the Dictionary. The pixel values,
 * however, are not stored in the variables but in the current Segment.
 */
class Variable {
public:

    /**
     * Constructor.
     * @param ncName The name of the variable, by which it is addressed within
     * its physical netCDF-file.
     * @param symbolicName The symbolic name of the variable, by which it is
     * addresses within the processing.
     */
    Variable(string ncName, string symbolicName) : ncName(ncName), symbolicName(symbolicName) {
    };

    /**
     * Destructor.
     */
    virtual ~Variable() {
    };

    /**
     * Adds an attribute to the variable.
     * @param attribute Pointer to the attribute to be added to the variable.
     */
    virtual void addAttribute(Attribute* attribute) = 0;

    /**
     * Adds a dimension to the variable.
     * @param dimension Pointer to the dimension to be added to the variable.
     */
    virtual void addDimension(Dimension* dimension) = 0;

    /**
     * Sets the variable's file name.
     * @param fileName The variable's file name.
     */
    virtual void setFileName(const string& fileName) = 0;

    /**
     * Getter for the variable's file name.
     * @return The variable's file name.
     */
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

    /**
     * Returns the name of the segment this variable is associated with during
     * the processing.
     * @return The segment's name.
     */
    virtual const string getSegmentName() const = 0;

    /**
     * Sets the name of the segment this variable is associated with during
     * the processing.
     * @param segmentName The segment's name.
     */
    virtual void setSegmentName(const string& segmentName) = 0;

    /**
     * Getter for the variable's type.
     * @return the variable's type.
     */
    virtual nc_type getType() const = 0;

    /**
     * Setter for the variable's type.
     * @param type The variable type to be set.
     */
    virtual void setType(nc_type type) = 0;

    /**
     * Getter for the variable's dimensions.
     * @return the variable's dimensions.
     */
    virtual vector<Dimension*> getDimensions() const = 0;

    /**
     * Getter for the variable's attributes.
     * @return the variable's attributes.
     */
    virtual vector<Attribute*> getAttributes() const = 0;

    /**
     * Getter for a single attribute of the variable.
     * @param name The attribute's name.
     * @return The specified attribute.
     */
    virtual Attribute& getAttribute(string& name) const = 0;

    /**
     * Returns a string representation of the variable.
     * @return A string representation of the variable.
     */
    virtual string toString() const = 0;

protected:
    vector<Attribute*> attributes;
    vector<Dimension*> dims;

    /**
     * The name of the variable, by which it is addressed within its netCDF-file.
     */
    string ncName;

    /**
     * The name of the variable, by which it is addressed within the processing
     * chain.
     */
    string symbolicName;
    string segmentName;
    string fileName;
    nc_type type;
};

/**
 * Represents the static information about a variable's dimension. To be created
 * by the Reader in order to allow the Writer to create the correct dimensions
 * within the output file.
 * @param name The dimension's name.
 * @param range The dimension's range, i.e. the number of values the dimension
 * has.
 */
class Dimension {
public:

    /**
     * Constructor.
     * @param name The dimension's name.
     * @param range The dimension's range, i.e. the number of values the dimension
     * has.
     */
    Dimension(string name, size_t range) : name(name), range(range) {
    }

    /**
     * Getter for the dimension's range.
     * @return The dimension's range.
     */
    size_t getRange() const {
        return range;
    }

    /**
     * Getter for the dimension's name.
     * @return The dimension's name.
     */
    string getName() const {
        return name;
    }

    /**
     * Returns a string representation of the dimension.
     * @return A string representation of the dimension.
     */
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