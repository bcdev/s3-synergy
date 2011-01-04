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
 * File:   VariableImpl.cpp
 * Author: thomass
 * 
 * Created on November 11, 2010, 11:33 AM
 */

#include <iosfwd>
#include <string>
#include <stdexcept>

#include "VariableImpl.h"

VariableImpl::VariableImpl(string ncName, string symbolicName, NcType type) : Variable(ncName, symbolicName, type) {}

VariableImpl::~VariableImpl() {
    for( size_t i = 0; i < attributes.size(); i++ ) {
        delete attributes[i];
    }
    for( size_t j = 0; j < dims.size(); j++ ) {
        delete dims[j];
    }
}

void VariableImpl::addAttribute(Attribute* attribute) {
    attributes.push_back(attribute);
}

void VariableImpl::addDimension(Dimension* dimension) {
    dims.push_back(dimension);
}

void VariableImpl::setFileName(string fileName) {
    this->fileName = fileName;
}

string VariableImpl::getFileName() const {
    return fileName;
}

string VariableImpl::getNcName() const {
    return ncName;
}

string VariableImpl::getSymbolicName() const {
    return symbolicName;
}

NcType VariableImpl::getType() const {
    return type;
}

vector<Dimension*> VariableImpl::getDimensions() const {
    return dims;
}

vector<Attribute*> VariableImpl::getAttributes() const {
    return attributes;
}

Attribute& VariableImpl::getAttribute(string& name) const {
    for( size_t i = 0; i < attributes.size(); i++ ) {
        if( attributes[i]->getKey().compare( name ) == 0 ) {
            return *attributes[i];
        }
    }
    throw std::invalid_argument( "No attribute with name " + name + "." );
}

string VariableImpl::toString() const {
    std::ostringstream oss;
    oss << "VariableImpl " << "[";
    oss << "symbolicName = " << symbolicName << ", ";
    oss << "ncName = " << ncName << ", ";
    oss << "type = " << getType() << ", ";
    oss << "fileName = " << getFileName() << ", ";
    oss << "dimensions = [";
    for (size_t i = 0; i < dims.size(); i++) {
        oss << dims[i]->toString() << ", ";
    }
    oss << "], ";
    oss << "attributes = [";
    for( size_t i = 0; i < attributes.size(); i++ ) {
        oss << attributes[i]->toString() << ", ";
    }
    oss << "]";
    oss << "]";

    return oss.str();
};