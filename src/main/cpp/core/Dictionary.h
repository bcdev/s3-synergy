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
 * File:   Dictionary.h
 * Author: thomass
 *
 * Created on December 21, 2010, 1:55 PM
 */

#ifndef DICTIONARY_H
#define	DICTIONARY_H

#include <stdexcept>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Boost.h"
#include "../util/XmlParser.h"

using std::logic_error;
using std::map;
using std::out_of_range;
using std::pair;
using std::set;
using std::string;
using std::vector;

template<class A, class E>
class Descriptor {
public:

    Descriptor(const string& name) : name(name), attributeMap(), elementMap() {
    }

    virtual ~Descriptor() {
        pair<string, A*> apair;
        pair<string, E*> tpair;

        reverse_foreach(apair, attributeMap) {
            delete apair.second;
        }

        reverse_foreach(tpair, elementMap) {
            delete tpair.second;
        }
    }

    A& addAttribute(int type, const string& name, const string& value = "") {
        if (hasAttribute(name)) {
            BOOST_THROW_EXCEPTION(logic_error("Attribute '" + name + "' already exists."));
        }
        A* a = new A(type, name, value);
        attributeMap[name] = a;
        return *a;
    }

    E& addElement(const string& name) {
        if (hasElement(name)) {
            BOOST_THROW_EXCEPTION(logic_error("Element '" + name + "' already exists."));
        }
        E* e = new E(name);
        elementMap[name] = e;
        return *e;
    }

    // TODO - remove method
    void clearAttributes() {
        pair<string, A*> apair;

        reverse_foreach(apair, attributeMap) {
            delete apair.second;
        }

        attributeMap.clear();
    }

    A& getAttribute(const string& name) const {
        if (!hasAttribute(name)) {
            BOOST_THROW_EXCEPTION(out_of_range("Descriptor '" + this->name + "' contains no attribute '" + name + "'."));
        }
        return *attributeMap.at(name);
    }

    vector<A*> getAttributes() const {
        vector<A*> attributes;
        std::pair<string, A*> pair;

        foreach(pair, attributeMap) {
            attributes.push_back(pair.second);
        }
        return attributes;
    }

    E& getElement(const string& name) const {
        if (!hasElement(name)) {
            BOOST_THROW_EXCEPTION(out_of_range("Descriptor '" + this->name + "' contains no element '" + name + "'."));
        }
        return *elementMap.at(name);
    }

    vector<E*> getElements() const {
        vector<E*> elements;
        pair<string, E*> pair;

        foreach(pair, elementMap) {
            elements.push_back(pair.second);
        }
        return elements;
    }

    const string& getName() const {
        return name;
    }

    bool hasAttribute(const string& name) const {
        return attributeMap.find(name) != attributeMap.end();
    }

    bool hasElement(const string& name) const {
        return elementMap.find(name) != elementMap.end();
    }

private:
    const string name;
    map<string, A*> attributeMap;
    map<string, E*> elementMap;
};

/**
 * Represents the static information about an attribute of a netCDF-variable.
 */
class Attribute {
public:

    /**
     * Constructor.
     * @param type The attribute's type.
     * @param name The attribute's name.
     * @param value The attribute's value.
     */
    Attribute(int type, const string& name, const string& value);

    /**
     * Destructor
     */
    ~Attribute();

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
     * Returns the attribute's type.
     * @return the attribute's type.
     */
    int getType() const {
        return type;
    }

    /**
     * Returns the attribute's name.
     * @return The attribute's name.
     */
    const string& getName() const {
        return name;
    }

    /**
     * Returns the attribute's value.
     * @return the attribute's value.
     */
    const string& getValue() const {
        return value;
    }

    /**
     * Sets the attribute's value.
     * @param value The attribute's value.
     */
    void setValue(const string& value) {
        this->value = value;
    }

    /**
     * Returns a string representation of the attribute.
     * @return a string representation of the attribute.
     */
    string toString() const;

private:
    const string name;
    const int type;
    string value;
};

/**
 * Represents a variable dimension.
 *
 * @param name The dimension's name.
 * @param size The dimension's size, i.e. the number of values the dimension has.
 */
class Dimension {
public:

    /**
     * Constructor.
     * @param name The dimension's name.
     */
    Dimension(const string& name, size_t size = 0);

    /**
     * Destructor.
     */
    ~Dimension();

    /**
     * Returns the dimension's name.
     * @return the dimension's name.
     */
    const string& getName() const {
        return name;
    }

    /**
     * Returns the dimension's size.
     * @return The dimension's size.
     */
    size_t getSize() const {
        return size;
    }

    /**
     * Sets the dimension's size.
     * @param size The dimension's size.
     */
    void setSize(size_t size) {
        this->size = size;
    }

    /**
     * Returns a string representation of the dimension.
     * @return a string representation of the dimension.
     */
    string toString() const;

private:
    const string name;
    size_t size;
};

class SegmentDescriptor;

/**
 * Represents the static information about an (input or output) variable. All
 * variables are instantiated using the Dictionary. The pixel values,
 * however, are not stored in the variables but in the current Segment.
 */
class VariableDescriptor : public Descriptor<Attribute, Dimension> {
public:

    /**
     * Constructor.
     * @param name The symbolic name of the variable, by which it is
     * addresses within the processing.
     */
    VariableDescriptor(const string& name);

    /**
     * Destructor.
     */
    ~VariableDescriptor();

    /**
     * Getter for the variable's type.
     * @return the variable's type.
     */
    int getType() const {
        return type;
    }

    /**
     * Setter for the variable's type.
     * @param type The variable type to be set.
     */
    void setType(int type) {
        this->type = type;
    }

    Dimension& addDimension(const string& name) {
        return addElement(name);
    }

    Dimension& getDimension(const string& name) const {
        return getElement(name);
    }

    vector<Dimension*> getDimensions() const {
        return getElements();
    }

    bool hasDimension(const string& name) const {
        return hasElement(name);
    }

    /**
     * Getter for the variable's file name.
     * @return The variable's file name.
     */
    string getNcFileBasename() const {
        return ncFileName;
    }

    /**
     * Sets the variable's file name.
     * @param fileName The variable's file name.
     */
    void setNcFileName(const string& ncFileName) {
        this->ncFileName = ncFileName;
    }

    /**
     * Returns the name of the variable, by which it is addressed within its
     * netCDF-file.
     * @return the netCDF-name of the variable.
     */
    string getNcVarName() const {
        return ncVarName;
    }

    /**
     * Sets the name of the variable, by which it is addressed within its
     * netCDF-file.
     * @param ncVariableName The name to set.
     */
    void setNcVarName(const string& ncVarName) {
        this->ncVarName = ncVarName;
    }

    /**
     * Returns the name of the segment this variable is associated with during
     * the processing.
     * @return The segment's name.
     */
    string getSegmentName() const {
        return segmentName;
    }

    /**
     * Sets the name of the segment this variable is associated with during
     * the processing.
     * @param segmentName The segment's name.
     */
    void setSegmentName(const string& segmentName) {
        this->segmentName = segmentName;
    }

    /**
     * Returns a string representation of the variable.
     * @return A string representation of the variable.
     */
    string toString() const;

private:
    int type;
    string ncVarName;
    string ncFileName;
    string segmentName;
};

class SegmentDescriptor : public Descriptor<Attribute, VariableDescriptor> {
public:

    SegmentDescriptor(const string& name) : Descriptor<Attribute, VariableDescriptor>(name) {
    }

    ~SegmentDescriptor() {
    }

    VariableDescriptor& addVariableDescriptor(const string& name) {
        return addElement(name);
    }

    VariableDescriptor& getVariableDescriptor(const string& name) const {
        return getElement(name);
    }

    vector<VariableDescriptor*> getVariableDescriptors() const {
        return getElements();
    }

    bool hasVariableDescriptor(const string& name) const {
        return hasElement(name);
    }
};

class ProductDescriptor : public Descriptor<Attribute, SegmentDescriptor> {
public:

    ProductDescriptor(const string& name) : Descriptor<Attribute, SegmentDescriptor>(name) {
    }

    ~ProductDescriptor() {
    }

    SegmentDescriptor& addSegmentDescriptor(const string& name) {
        return addElement(name);
    }

    SegmentDescriptor& getSegmentDescriptor(const string& name) const {
        return getElement(name);
    }

    vector<SegmentDescriptor*> getSegmentDescriptors() const {
        return getElements();
    }

    bool hasSegmentDescriptor(const string& name) const {
        return hasElement(name);
    }

    vector<VariableDescriptor*> getVariables() const;

    VariableDescriptor* getVariable(const string& name) const;
};

/**
 * A dictionary serves as source for static information about variables, i.e. 
 * the name, type, dimensions, and attributes of variables both to be written
 * and read.
 *
 * @param configFile The file comprising the path to the output variable
 * definitions.
 */
class Dictionary : public Descriptor<Attribute, ProductDescriptor> {
public:
    Dictionary();
    Dictionary(string configFile);
    virtual ~Dictionary();

    ProductDescriptor& addProductDescriptor(const string& name) {
        return addElement(name);
    }

    ProductDescriptor& getProductDescriptor(const string& name) const {
        return getElement(name);
    }

    vector<ProductDescriptor*> getProductDescriptors() const {
        return getElements();
    }

    bool hasProductDescriptor(const string& name) const {
        return hasElement(name);
    }

private:
    void init();
    string filePath;
    void parseVariablesFile(string& variableDefPath, string& file, ProductDescriptor& vol);
    void parseAttributes(string& file, string& variableName, VariableDescriptor& var);
    int mapToNcType(const string& typeString);

    XmlParser xmlParser;
    const string configFile;
};

#endif	/* DICTIONARY_H */

