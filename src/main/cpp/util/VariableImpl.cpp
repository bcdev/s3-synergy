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

#include <string>

#include "VariableImpl.h"

VariableImpl::VariableImpl(string id, NcType type) : Variable(id, type) {}

VariableImpl::~VariableImpl() {}

void VariableImpl::addAttribute(Attribute<void*>* attribute) {
    attributes.push_back(attribute);
}

void VariableImpl::addDimension(Dimension* dimension) {
    dims.push_back(dimension);
}

string VariableImpl::getId() const {
    return id;
}

string VariableImpl::getNcName() const {
    return ncName;
}

NcType VariableImpl::getType() const {
    return type;
}

vector<Dimension*> VariableImpl::getDimensions() const {
    return dims;
}

vector<Attribute<void*>*> VariableImpl::getAttributes() const {
    return attributes;
}