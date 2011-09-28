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
 * File:   Dictionary.cpp
 * Author: thomass
 *
 * Created on December 21, 2010, 1:55 PM
 */

#include "Dictionary.h"

Attribute::Attribute(int type, const string& name, const string& value) : name(name), type(type) {
    this->value = value;
}

Attribute::~Attribute() {
}

string Attribute::toString() const {
    std::ostringstream oss;
    oss << "Attribute " << "[";
    oss << "type = " << type << ", ";
    oss << "name = " << name << ", ";
    oss << "value = " << value << "]";
    return oss.str();
}

Dimension::Dimension(const string& name, size_t size) : name(name) {
    this->size = size;
}

Dimension::~Dimension() {
}

string Dimension::toString() const {
    std::ostringstream oss;
    oss << "Dimension: " << "[";
    oss << "name = " << name << ", ";
    oss << "size = " << size << "]";
    return oss.str();
}

VariableDescriptor::VariableDescriptor(const string& name) : Descriptor<Attribute, Dimension>(name) {
    type = 0;
    ncFileName = "";
    ncVarName = "";
    segmentName = "";
}

VariableDescriptor::~VariableDescriptor() {
}

string VariableDescriptor::toString() const {
    std::ostringstream oss;
    oss << "Variable " << "[";
    oss << "name = " << getName() << ", ";
    oss << "type = " << getTypeString(type) << ", ";
    oss << "scale factor = " << lexical_cast<string>(getScaleFactor()) << ", ";
    oss << "add offset = " << lexical_cast<string>(getAddOffset()) << ", ";
    oss << "ncFileName = " << ncFileName << ", ";
    oss << "ncVarName = " << ncVarName << "]";

    return oss.str();
};

Dictionary::Dictionary() : Descriptor<Attribute, ProductDescriptor>("Dictionary") {
}

Dictionary::~Dictionary() {
}
