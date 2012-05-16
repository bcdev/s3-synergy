/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
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
 */

#ifndef DICTIONARY_H
#define	DICTIONARY_H

#include <map>
#include <stdexcept>
#include <string>
#include <valarray>
#include <vector>

#include "Boost.h"
#include "Constants.h"

using std::logic_error;
using std::map;
using std::out_of_range;
using std::pair;
using std::runtime_error;
using std::string;
using std::valarray;
using std::vector;

/**
 * The base class for all descriptors. This is non-public API.
 */
template<class A, class E>
class Descriptor {
public:
    void addAttribute(const A& a) {
        addAttribute(a.getType(), a.getName(), a.getValue());
    }

    A& addAttribute(int type, const string& name, const string& value) {
    	A* a;
        if (hasAttribute(name)) {
        	A* a = attributeMap[name];
        	a->setValue(value);
        } else {
        	A* a = new A(type, name, value);
        	attributeMap[name] = a;
        }
        return *a;
    }

    A& getAttribute(const string& name) const {
        if (!hasAttribute(name)) {
            BOOST_THROW_EXCEPTION( out_of_range("Descriptor '" + this->name + "' contains no attribute '" + name + "'."));
        }
        return *attributeMap[name];
    }

    vector<A*> getAttributes() const {
        vector<A*> attributes;
        std::pair<string, A*> pair;

        foreach(pair, attributeMap)
                {
                    attributes.push_back(pair.second);
                }
        return attributes;
    }

    const string& getName() const {
        return name;
    }

    bool hasAttribute(const string& name) const {
        return attributeMap.find(name) != attributeMap.end();
    }

protected:
    Descriptor(const string& name) :
            name(name), attributeMap(), elementMap() {
    }

    virtual ~Descriptor() {
        pair<string, A*> apair;
        pair<string, E*> tpair;

        reverse_foreach(apair, attributeMap)
                {
                    delete apair.second;
                }

        reverse_foreach(tpair, elementMap)
                {
                    delete tpair.second;
                }
    }

    E& addElement(const string& name) {
        if (hasElement(name)) {
            BOOST_THROW_EXCEPTION( logic_error("Element '" + name + "' already exists."));
        }
        E* e = new E(name);
        elementMap[name] = e;
        elementList.push_back(e);
        return *e;
    }

    E& getElement(const string& name) const {
        if (!hasElement(name)) {
            BOOST_THROW_EXCEPTION( out_of_range("Descriptor '" + this->name + "' contains no element '" + name + "'."));
        }
        return *elementMap.at(name);
    }

    bool hasElement(const string& name) const {
        return elementMap.find(name) != elementMap.end();
    }

    vector<E*> getElements() const {
        vector<E*> elements;

        foreach(E* e, elementList)
                {
                    elements.push_back(e);
                }
        return elements;
    }

private:
    Descriptor(const Descriptor& descriptor) {
    }

    const string name;
    map<string, A*> attributeMap;
    map<string, E*> elementMap;
    vector<E*> elementList;
};

/**
 * Describes a netCDF attribute.
 */
class Attribute {
public:

    /**
     * Constructor.
     *
     * @param type The attribute's type.
     * @param name The attribute's name.
     * @param value The attribute's value.
     */
    Attribute(int type, const string& name, const string& value);

    /**
     * Constructor.
     *
     * @param type The attribute's type.
     * @param name The attribute's name.
     * @param data The attribute's value.
     */
    Attribute(int type, const string& name, const valarray<int8_t>& data) :
            name(name), type(type) {
    	using boost::lexical_cast;
    	using boost::numeric_cast;

    	string s;
        for (size_t i = 0; i < data.size(); i++) {
            if (i > 0) {
                s.append(" ");
            }
            s.append(lexical_cast<string>(numeric_cast<int16_t>(data[i])));
        }
        value = s;
    }

    /**
     * Constructor.
     *
     * @param type The attribute's type.
     * @param name The attribute's name.
     * @param data The attribute's value.
     */
    Attribute(int type, const string& name, const valarray<uint8_t>& data) :
            name(name), type(type) {
    	using boost::lexical_cast;
    	using boost::numeric_cast;

    	string s;
        for (size_t i = 0; i < data.size(); i++) {
            if (i > 0) {
                s.append(" ");
            }
            s.append(lexical_cast<string>(numeric_cast<uint16_t>(data[i])));
        }
        value = s;
    }

    /**
     * Constructor.
     *
     * @param type The attribute's type.
     * @param name The attribute's name.
     * @param data The attribute's value.
     */
    template<class T>
    Attribute(int type, const string& name, const valarray<T>& data) :
            name(name), type(type) {
    	using boost::lexical_cast;

    	string s;
        for (size_t i = 0; i < data.size(); i++) {
            if (i > 0) {
                s.append(" ");
            }
            s.append(lexical_cast<string>(data[i]));
        }
        value = s;
    }

    /**
     * Destructor
     */
    ~Attribute();

    /**
     * Returns the attribute value as 'byte' array.
     * @return the attribute value as 'byte' array.
     */
    valarray<int8_t> getBytes() const {
    	using boost::is_any_of;
    	using boost::lexical_cast;
    	using boost::numeric_cast;
    	using boost::algorithm::split;

        vector<string> tokens;
        split(tokens, value, is_any_of(" "));
        valarray<int8_t> result(tokens.size());
        for (size_t i = 0; i < tokens.size(); i++) {
            result[i] = numeric_cast<int8_t>(lexical_cast<int16_t>(tokens[i]));
        }
        return result;
    }

    /**
     * Returns the attribute value as 'unsigned byte' array.
     * @return the attribute value as 'unsigned byte' array.
     */
    valarray<uint8_t> getUBytes() const {
    	using boost::is_any_of;
    	using boost::lexical_cast;
    	using boost::numeric_cast;
    	using boost::algorithm::split;

    	vector<string> tokens;
        split(tokens, value, is_any_of(" "));
        valarray<uint8_t> result(tokens.size());
        for (size_t i = 0; i < tokens.size(); i++) {
            result[i] = numeric_cast<uint8_t>(lexical_cast<uint16_t>(tokens[i]));
        }
        return result;
    }

    /**
     * Returns the attribute value as 'short' array.
     * @return the attribute value as 'short' array.
     */
    valarray<int16_t> getShorts() const {
        return getComponents<int16_t>();
    }

    /**
     * Returns the attribute value as 'unsigned short' array.
     * @return the attribute value as 'unsigend short' array.
     */
    valarray<uint16_t> getUShorts() const {
        return getComponents<uint16_t>();
    }

    /**
     * Returns the attribute value as 'int' array.
     * @return the attribute value as 'int' array.
     */
    valarray<int32_t> getInts() const {
        return getComponents<int32_t>();
    }

    /**
     * Returns the attribute value as 'unsigned int' array.
     * @return the attribute value as 'unsigned int' array.
     */
    valarray<uint32_t> getUInts() const {
        return getComponents<uint32_t>();
    }

    /**
     * Returns the attribute value as 'long' array.
     * @return the attribute value as 'long' array.
     */
    valarray<int64_t> getLongs() const {
        return getComponents<int64_t>();
    }

    /**
     * Returns the attribute value as 'unsigned long' array.
     * @return the attribute value as 'unsigned long' array.
     */
    valarray<uint64_t> getULongs() const {
        return getComponents<uint64_t>();
    }

    /**
     * Returns the attribute value as 'float' array.
     * @return the attribute value as 'float' array.
     */
    valarray<float> getFloats() const {
        return getComponents<float>();
    }

    /**
     * Returns the attribute value as 'double' array.
     * @return the attribute value as 'double' array.
     */
    valarray<double> getDoubles() const {
        return getComponents<double>();
    }

    /**
     * Returns the attribute value as 'string' array.
     * @return the attribute value as 'string' array.
     */
    valarray<string> getTokens() const {
        return getComponents<string>();
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

    template<class T>
    valarray<T> getComponents() const {
    	using boost::is_any_of;
    	using boost::lexical_cast;
    	using boost::algorithm::split;

        vector<string> tokens;
        split(tokens, value, boost::is_any_of(" "));
        valarray<T> result(tokens.size());
        for (size_t i = 0; i < tokens.size(); i++) {
            result[i] = lexical_cast<T>(tokens[i]);
        }
        return result;
    }

private:
    const string name;
    const int type;
    string value;
};

/**
 * Describes a netCDF dimension.
 *
 * @param name The dimension's name.
 * @param size The dimension's size.
 */
class Dimension {
public:

    /**
     * Constructor.
     * @param name The dimension's name.
     * @param size The dimension's size.
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

/**
 * Describes a netCDF variable.
 */
class VariableDescriptor: public Descriptor<Attribute, Dimension> {
public:

    VariableDescriptor(const string& name);

    /**
     * Destructor.
     */
    ~VariableDescriptor();

    /**
     * Returns the variable's type.
     * @return the variable's type.
     */
    int getType() const {
        return type;
    }

    /**
     * Sets the variable's type.
     * @param type The variable type to be set.
     */
    void setType(int type) {
        this->type = type;
    }

    /**
     * Returns the variable's fill value.
     * @return the variable's fill value.
     */
    template<class T>
    T getFillValue() const {
        return hasAttribute("_FillValue") ? getAttribute("_FillValue").getComponents<T>()[0] : T(0);
    }

    /**
     * Returns the variable's scale factor.
     * @return the variable's scale factor.
     */
    double getScaleFactor() const {
        return hasAttribute("scale_factor") ? getAttribute("scale_factor").getDoubles()[0] : 1.0;
    }

    /**
     * Returns the variable's add offset.
     * @return the variable's add offset.
     */
    double getAddOffset() const {
        return hasAttribute("add_offset") ? getAttribute("add_offset").getDoubles()[0] : 0.0;
    }

    /**
     * Adds a dimension to the variable.
     * @param name The name of the dimension.
     * @return a reference to the dimension added.
     */
    Dimension& addDimension(const string& name) {
        return addElement(name);
    }

    /**
     * Returns a dimension of the variable.
     * @param name The name of the dimension.
     * @return a reference to the dimension with the requested name.
     */
    Dimension& getDimension(const string& name) const {
        return getElement(name);
    }

    /**
     * Returns the dimensions of the variable.
     * @return the dimensions of the variable.
     */
    vector<Dimension*> getDimensions() const {
        return getElements();
    }

    /**
     * Inquires the variable about a dimension.
     * @param name The name of the dimension.
     * @return true if the variable has a dimension with the requested name, false otherwise.
     */
    bool hasDimension(const string& name) const {
        return hasElement(name);
    }

    /**
     * Returns the name of the netCDF file the variable is or shall be stored in.
     * @return the name of the netCDF file the variable is or shall be stored in.
     */
    string getNcFileBasename() const {
        return ncFileName;
    }

    /**
     * Set the name of the netCDF file the variable is or shall be stored in.
     * @param ncFileName The name of the netCDF file the variable is or shall be stored in.
     */
    void setNcFileName(const string& ncFileName) {
        this->ncFileName = ncFileName;
    }

    /**
     * Returns the name of the variable, which is used or shall be used in its netCDF file.
     * @return the name of the variable used in its netCDF file.
     */
    string getNcVarName() const {
        return ncVarName;
    }

    /**
     * Sets the name of the variable, which is used or shall be used in its netCDF file.
     * @param ncVarName The name of the variable used in its netCDF file.
     */
    void setNcName(const string& ncVarName) {
        this->ncVarName = ncVarName;
    }

    /**
     * Returns the name of the segment this variable is or shall be associated with.
     * @return the name of the associated segment.
     */
    string getSegmentName() const {
        return segmentName;
    }

    /**
     * Sets the name of the segment this variable is or shall be associated with.
     * @param segmentName The name of the associated segment.
     */
    void setSegmentName(const string& segmentName) {
        this->segmentName = segmentName;
    }

    /**
     * Returns a string representation of the variable.
     * @return a string representation of the variable.
     */
    string toString() const;

private:
    int type;
    string ncVarName;
    string ncFileName;
    string segmentName;

    static const string getTypeString(const int type) {
        switch (type) {
        case Constants::TYPE_BYTE:
            return "byte";
        case Constants::TYPE_SHORT:
            return "short";
        case Constants::TYPE_INT:
            return "int";
        case Constants::TYPE_LONG:
            return "long";
        case Constants::TYPE_UBYTE:
            return "ubyte";
        case Constants::TYPE_USHORT:
            return "ushort";
        case Constants::TYPE_UINT:
            return "unit";
        case Constants::TYPE_ULONG:
            return "ulong";
        case Constants::TYPE_FLOAT:
            return "float";
        case Constants::TYPE_DOUBLE:
            return "double";
        case Constants::TYPE_CHAR:
            return "char";
        case Constants::TYPE_STRING:
            return "string";
        default:
            BOOST_THROW_EXCEPTION(runtime_error("Unknown attribute type."));
            break;
        }
    }
};

/**
 * Describes a segment. A segment descriptor is a collection of variable descriptors and attributes.
 */
class SegmentDescriptor: public Descriptor<Attribute, VariableDescriptor> {
public:

	/**
	 * Constructor.
	 * @param name The segment's name.
	 */
    SegmentDescriptor(const string& name) :
            Descriptor<Attribute, VariableDescriptor>(name) {
    }

    /**
     * Destructor.
     */
    ~SegmentDescriptor() {
    }

    /**
     * Adds a variable descriptor.
     * @param name The name of the variable descriptor to be added.
     * @return a reference to the variable descriptor added.
     */
    VariableDescriptor& addVariableDescriptor(const string& name) {
        return addElement(name);
    }

    /**
     * Returns a variable descriptor that has been added .
     * @param name The name of the variable descriptor.
     * @return a reference to the variable descriptor.
     */
    VariableDescriptor& getVariableDescriptor(const string& name) const {
        return getElement(name);
    }

    /**
     * Returns all variable descriptors.
     * @return the variable descriptors.
     */
    vector<VariableDescriptor*> getVariableDescriptors() const {
        return getElements();
    }

    /**
     * Inquires the segment descriptor about a variable descriptor.
     * @param name The name of the variable descriptor.
     * @return true if the segment descriptor has a variable descriptor with the requested name, false otherwise.
     */
    bool hasVariableDescriptor(const string& name) const {
        return hasElement(name);
    }
};

/**
 * Describes a product. A product descriptor is a collection of segment descriptors and attributes.
 */
class ProductDescriptor: public Descriptor<Attribute, SegmentDescriptor> {
public:

    ProductDescriptor(const string& name) :
            Descriptor<Attribute, SegmentDescriptor>(name) {
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
};

/**
 * A dictionary is a collection of segment descriptors and attributes.
 */
class Dictionary: public Descriptor<Attribute, ProductDescriptor> {
public:
    Dictionary();
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
};

#endif	/* DICTIONARY_H */

